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
	bool ReadObjectStart(Source& src,  const ParserConfiguration<Token>& config)
	{
		Token token;
		for(;;)
		{
			if(!src.GetToken(token))
			{
				return false;
			}
			if(config.IsSkipToken(token))
			{
				continue;
			}
			if(config.IsDataStartSymbol(token))
			{
				return true;
			}
			break;
		}
		return false;
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
		for(;;)
		{
			if(!src.GetToken(token))
			{
				return false;
			}
			if(config.IsSkipToken(token))
			{
				break;
			}
			tokens.push_back(token);
		}
		if(tokens.empty())
		{
			return false;
		}

		return true;
	}
}
