#pragma once
#include <vector>
#include <sstream>
#include <algorithm>

#include "ParserTypes.h"
#include "ParserUtils.h"
#include "ObjectHandler.h"

namespace Parser
{
	template<typename Num, typename Source, typename Char>
	EStatus ReadNumericObject(Source& src, const ParserConfiguration<Source, Char>& config, ObjectData& objectData)
	{
		auto status = SkipChars(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		Num num{};
		status = src.Read(&num);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);

		return objectData.AppendData(num);
	}

	template<typename Source, typename Char>
	EStatus ReadChar(Source& src, const ParserConfiguration<Source, Char>& config, Char& ch)
	{
		auto status = src.GetChar(ch);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		if(config.IsEscapeSequenceStart(ch))
		{
			status = src.GetChar(ch);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			RET_ON_FALSE(config.IsSpecCharacter(ch), EStatus::FAIL);
		}
		return EStatus::SUCCESS;
	}
	template<typename Source, typename Char>
	EStatus ReadCharObject(Source& src, const ParserConfiguration<Source, Char>& config, ObjectData& objectData)
	{
		auto status = SkipChars(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		Char ch;
		status = src.GetChar(ch);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		if(!config.IsCharStart(ch))
		{
			return EStatus::FAIL;
		}

		status = ReadChar(src, config, ch);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);

		status = objectData.AppendData(ch);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);

		if(!config.IsCharEnd(ch))
		{
			return EStatus::FAIL;
		}
		return EStatus::SUCCESS;
	}

	template<typename Source, typename Char>
	EStatus ReadStringObject(Source& src, const ParserConfiguration<Source, Char>& config, ObjectData& objectData)
	{
		auto status = SkipChars(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);

		Char ch;
		status = src.GetChar(ch);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);

		if(!config.IsStringStart(ch))
		{
			return EStatus::FAIL;
		}
		for(;;)
		{
			status = ReadChar(src, config, ch);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			if(config.IsStringEnd(ch))
			{
				break;
			}

			status = objectData.AppendData(ch);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
		}
		return EStatus::SUCCESS;
	}
}
