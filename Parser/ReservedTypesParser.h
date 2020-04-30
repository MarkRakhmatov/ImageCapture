#pragma once
#include <vector>
#include <sstream>
#include <algorithm>

#include "TypeInfo.h"
#include "ParserUtils.h"
#include "ObjectHandler.h"

namespace Parser
{
	template<typename Num, typename Source, typename Token>
	EStatus ReadNumericObject(Source& src, const ParserConfiguration<Source, Token>& config, ObjectDescriptor<Token>& obj)
	{
		auto status = SkipTokens(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		return ReadObjectData<Num, Source, Token>(src, obj);
	}

	template<typename Source, typename Token>
	EStatus ReadCharObject(Source& src, const ParserConfiguration<Source, Token>& config, ObjectDescriptor<Token>& obj)
	{
		auto status = SkipTokens(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		Token token;
		status = src.GetToken(token);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		if(!config.IsCharStart(token))
		{
			return EStatus::FAIL;
		}

		status = src.GetToken(token);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);

		obj.objectData.push_back(token);

		status = src.GetToken(token);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);

		if(!config.IsCharEnd(token))
		{
			return EStatus::FAIL;
		}
		return EStatus::SUCCESS;
	}

	template<typename Source, typename Token>
	EStatus ReadStringObject(Source& src, const ParserConfiguration<Source, Token>& config, ObjectDescriptor<Token>& obj)
	{
		auto status = SkipTokens(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);

		Token token;
		status = src.GetToken(token);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);

		if(!config.IsStringStart(token))
		{
			return EStatus::FAIL;
		}
		for(;;)
		{
			status = src.GetToken(token);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			if(config.IsStringEnd(token))
			{
				break;
			}
			obj.objectData.push_back(token);
		}
		return EStatus::SUCCESS;
	}

	template<typename Source, typename Token, ReaderFunc Reader>
	EStatus ReadArrayBottom(Source& src, const ParserConfiguration<Source, Token>& config, ObjectDescriptor<Token>& obj)
	{
		auto status = ReadBlockStart(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);

		for(;;)
		{
			status = ReaderFunc(src, config, obj);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			status = SkipTokens(src, config);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			Token token;
			status = src.PeekToken(token);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			RET_ON_TRUE(!config.IsSeparator(token), EStatus::SUCCESS);

			status = src.GetToken(token);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
		}

		return ReadBlockEnd(src, config);
	}

	template<typename Source, typename Token, ReaderFunc Reader>
	EStatus ReadArrayImpl(Source& src, const ParserConfiguration<Source, Token>& config, ObjectDescriptor<Token>& obj, uint8_t arrayDepth)
	{
		auto status = EStatus::FAIL;
		if(arrayDepth == 1)
		{
			return ReadArrayBottom(src, config, obj);
		}

		status = ReadBlockStart(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		--arrayDepth;
		for(;;)
		{
			ReadArrayImpl(src, config, obj, arrayDepth);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			status = SkipTokens(src, config);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			Token token;
			status = src.PeekToken(token);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			RET_ON_TRUE(!config.IsSeparator(token), EStatus::SUCCESS);

			status = src.GetToken(token);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
		}

		return ReadBlockEnd(src, config);
	}


	template<typename Source, typename Token, ReaderFunc Reader>
	EStatus ReadArray(Source& src, const ParserConfiguration<Source, Token>& config, ObjectDescriptor<Token>& obj)
	{

		auto status = EStatus::FAIL;
		if(obj.arrayDepth == 0)
		{
			return ReaderFunc(src, config, obj);
		}

		return ReadArrayImpl(src, config, obj, obj.arrayDepth);
	}
}
