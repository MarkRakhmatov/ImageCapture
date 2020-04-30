#pragma once
#include "CodeGeneration.h"
#include "ParserConfiguration.h"
#include "ParserTypes.h"
#include "ObjectHandler.h"

#include <vector>
#include <algorithm>

namespace Parser
{
	template<typename Container, typename ValueType>
	bool Contains(const Container& container, const ValueType& val)
	{
		return std::find(container.begin(), container.end(), val)!=container.end();
	}

	template<typename Source, typename Token=char>
	EStatus ReadTokens(Source& src, const Tokens<Token>& tokens)
	{
		for(auto& token : tokens)
		{
			auto status = src.GetToken(token);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
		}
		return EStatus::SUCCESS;
	}

	template<typename Source, typename Token>
	EStatus SkipTokens(Source& src, const ParserConfiguration<Source, Token>& config)
	{
		Token t;
		for(;;)
		{
			auto status = src.PeekToken(t);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			if(!config.IsSkipToken(t))
			{
				break;
			}
			status = src.GetToken(t);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
		}
		return EStatus::SUCCESS;
	}

	template<typename Source, typename Container, typename Token>
	EStatus ReadWord(Source& src, const ParserConfiguration<Source, Token>& config, Container& tokens)
	{
		auto status = SkipTokens<Source, Token>(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		Token token;
		status = src.GetToken(token);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		if(!config.IsLetter(token))
		{
			return EStatus::FAIL;
		}
		tokens.push_back(token);
		for(;;)
		{
			status = src.PeekToken(token);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			if(!config.IsLetter(token)
					&& !config.IsDigit(token))
			{
				break;
			}
			status = src.GetToken(token);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			tokens.push_back(token);
		}
		RET_ON_FALSE(!tokens.empty(), EStatus::FAIL);

		return EStatus::SUCCESS;
	}

	template<typename Source, typename Token>
	EStatus ReadBlockStart(Source& src, const ParserConfiguration<Source, Token>& config)
	{
		auto status = SkipTokens<Source, Token>(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);

		Token token;
		status = src.GetToken(token);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		if(!config.IsBlockStart(token))
		{
			return EStatus::FAIL;
		}
		return EStatus::SUCCESS;
	}

	template<typename Source, typename Token>
	EStatus ReadBlockEnd(Source& src, const ParserConfiguration<Source, Token>& config)
	{
		auto status = SkipTokens<Source, Token>(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		Token token;
		status = src.GetToken(token);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		if(!config.IsBlockEnd(token))
		{
			return EStatus::FAIL;
		}
		return EStatus::SUCCESS;
	}
	template<typename Source, typename Token>
	EStatus ReadArrayStart(Source& src, const ParserConfiguration<Source, Token>& config, bool& isArrayStart)
	{
		auto status = SkipTokens<Source, Token>(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		Token token;
		status = src.PeekToken(token);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		isArrayStart = config.IsArrayStart(token);
		if(!isArrayStart)
		{
			return EStatus::SUCCESS;
		}
		status = src.GetToken(token);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		return EStatus::SUCCESS;
	}
	template<typename Source, typename Token>
	EStatus ReadArrayEnd(Source& src, const ParserConfiguration<Source, Token>& config, bool& isArrayEnd)
	{
		auto status = SkipTokens<Source, Token>(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		Token token;
		status = src.PeekToken(token);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		isArrayEnd = config.IsArrayEnd(token);
		if(!isArrayEnd)
		{
			return EStatus::SUCCESS;
		}
		status = src.GetToken(token);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		return EStatus::SUCCESS;
	}


	template<typename Source, typename Token>
	EStatus ReadArrayDecl(Source& src, const ParserConfiguration<Source, Token>& config, bool& isArray)
	{
		bool isArrayStart = false;
		EStatus status = ReadArrayStart(src, config, isArrayStart);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		if(!isArrayStart)
		{
			isArray = false;
			return EStatus::SUCCESS;
		}
		bool isArrayEnd = false;
		status = ReadArrayEnd(src, config, isArrayEnd);
		if(status != EStatus::SUCCESS || !isArrayEnd)
		{
			return status;
		}

		isArray = true;
		return EStatus::SUCCESS;
	}

	template<typename Source, typename Token>
	EStatus ReadArrayDepth(Source& src, const ParserConfiguration<Source, Token>& config, uint8_t& depth)
	{
		depth = 0;
		bool isArray = false;
		EStatus status = EStatus::FAIL;
		for(;;)
		{
			status = ReadArrayDecl(src, config, isArray);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			RET_ON_FALSE(isArray, EStatus::SUCCESS);
			++depth;
		}
		return EStatus::FAIL;
	}
}
