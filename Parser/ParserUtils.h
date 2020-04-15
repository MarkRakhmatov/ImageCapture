#pragma once
#include <vector>
#include <algorithm>

#include "ParserConfiguration.h"
#include "ObjectHandler.h"

namespace Parser
{
	template<typename Token=char>
	using Tokens = std::vector<Token>;


	template<typename Token=char>
	using Brackets = std::pair<Token, Token>;


	template<typename Container, typename ValueType>
	bool Contains(const Container& container, const ValueType& val)
	{
		return std::find(container.begin(), container.end(), val)!=container.end();
	}

	template<typename Source, typename Token=char>
	bool ReadTokens(Source& src, const Tokens<Token>& tokens)
	{
		for(auto& token : tokens)
		{
			if(!src.GetToken(token))
			{
				return false;
			}
		}
		return true;
	}

	template<typename Source, typename Token=char>
	bool SkipTokens(Source& src, const ParserConfiguration<Token>& config)
	{
		Token t;
		for(;;)
		{
			if(!src.PeekToken(t))
			{
				return false;
			}
			if(!config.IsSkipToken(t))
			{
				break;
			}
			src.GetToken(t);
		}
		return true;
	}

	template<typename Source, typename Container, typename Token=char>
	bool ReadWord(Source& src, const ParserConfiguration<Token>& config, Container& tokens)
	{
		if(!SkipTokens<Source, Token>(src, config))
		{
			return false;
		}
		Token token;
		if(!src.GetToken(token))
		{
			return false;
		}
		if(!config.IsLetter(token))
		{
			return false;
		}
		tokens.push_back(token);
		for(;;)
		{
			if(!src.PeekToken(token))
			{
				return false;
			}
			if(!config.IsLetter(token)
					&& !config.IsDigit(token))
			{
				break;
			}
			if(!src.GetToken(token))
			{
				return false;
			}
			tokens.push_back(token);
		}
		if(tokens.empty())
		{
			return false;
		}

		return true;
	}

	template<typename Source, typename Token>
	bool ReadBlockStart(Source& src, const ParserConfiguration<Token>& config)
	{
		if(!SkipTokens<Source, Token>(src, config))
		{
			return false;
		}
		Token token;
		if(!src.GetToken(token) || !config.IsBlockStart(token))
		{
			return false;
		}
		return true;
	}

	template<typename Source, typename Token>
	bool ReadBlockEnd(Source& src, const ParserConfiguration<Token>& config)
	{
		if(!SkipTokens<Source, Token>(src, config))
		{
			return false;
		}
		Token token;
		if(!src.GetToken(token) || !config.IsBlockEnd(token))
		{
			return false;
		}
		return true;
	}
	template<typename Source, typename Token>
	bool ReadArrayStart(Source& src, const ParserConfiguration<Token>& config, bool& isArrayStart)
	{
		if(!SkipTokens<Source, Token>(src, config))
		{
			return false;
		}
		Token token;
		if(!src.PeekToken(token))
		{
			return false;
		}
		isArrayStart = config.IsArrayStart(token);
		if(!isArrayStart)
		{
			return true;
		}
		if(!src.GetToken(token))
		{
			return false;
		}
		return true;
	}
	template<typename Source, typename Token>
	bool ReadArrayEnd(Source& src, const ParserConfiguration<Token>& config, bool& isArrayEnd)
	{
		if(!SkipTokens<Source, Token>(src, config))
		{
			return false;
		}
		Token token;
		if(!src.PeekToken(token))
		{
			return false;
		}
		isArrayEnd = config.IsArrayEnd(token);
		if(!isArrayEnd)
		{
			return true;
		}
		if(!src.GetToken(token))
		{
			return false;
		}
		return true;
	}
	template<typename Source, typename Token>
	bool ReadArrayDecl(Source& src, const ParserConfiguration<Token>& config, bool& isArray)
	{
		bool isArrayStart = false;
		if(!ReadArrayStart(src, config, isArrayStart))
		{
			isArray = false;
			return false;
		}
		if(!isArrayStart)
		{
			isArray = false;
			return true;
		}
		bool isArrayEnd = false;
		if(!ReadArrayEnd(src, config, isArrayEnd) || !isArrayEnd)
		{
			return false;
		}

		isArray = true;
		return true;
	}
}
