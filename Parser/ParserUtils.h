#pragma once
#include "CodeGeneration.h"
#include "ParserConfiguration.h"
#include "ParserTypes.h"
#include "ObjectDescriptor.h"

#include <vector>
#include <algorithm>

namespace Parser
{
	template<typename Container, typename ValueType>
	bool Contains(const Container& container, const ValueType& val)
	{
		return std::find(container.begin(), container.end(), val)!=container.end();
	}

	template<typename Source, typename Char>
	EStatus SkipChars(Source& src, const ParserConfiguration<Source, Char>& config)
	{
		Char ch;
		for(;;)
		{
			auto status = src.PeekChar(ch);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			if(!config.IsSkipChar(ch))
			{
				break;
			}
			status = src.GetChar(ch);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
		}
		return EStatus::SUCCESS;
	}

	template<typename Source, typename Container, typename Char>
	EStatus ReadWord(Source& src, const ParserConfiguration<Source, Char>& config, Container& chars)
	{
		auto status = SkipChars<Source, Char>(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		Char ch;
		status = src.GetChar(ch);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		if(!config.IsLetter(ch))
		{
			return EStatus::FAIL;
		}
		chars.push_back(ch);
		for(;;)
		{
			status = src.PeekChar(ch);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			if(!config.IsLetter(ch)
					&& !config.IsDigit(ch))
			{
				break;
			}
			status = src.GetChar(ch);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			chars.push_back(ch);
		}
		RET_ON_FALSE(!chars.empty(), EStatus::FAIL);

		return EStatus::SUCCESS;
	}

	template<typename Source, typename Char>
	EStatus ReadBlockStart(Source& src, const ParserConfiguration<Source, Char>& config)
	{
		auto status = SkipChars<Source, Char>(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);

		Char ch;
		status = src.GetChar(ch);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		if(!config.IsBlockStart(ch))
		{
			return EStatus::FAIL;
		}
		return EStatus::SUCCESS;
	}

	template<typename Source, typename Char>
	EStatus ReadBlockEnd(Source& src, const ParserConfiguration<Source, Char>& config)
	{
		auto status = SkipChars<Source, Char>(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		Char ch;
		status = src.GetChar(ch);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		if(!config.IsBlockEnd(ch))
		{
			return EStatus::FAIL;
		}
		return EStatus::SUCCESS;
	}

	template<typename Source, typename Char>
	EStatus ReadArrayStart(Source& src, const ParserConfiguration<Source, Char>& config, bool& isArrayStart)
	{
		auto status = SkipChars<Source, Char>(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		Char ch;
		status = src.PeekChar(ch);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		isArrayStart = config.IsArrayStart(ch);
		if(!isArrayStart)
		{
			return EStatus::SUCCESS;
		}
		status = src.GetChar(ch);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		return EStatus::SUCCESS;
	}

	template<typename Source, typename Char>
	EStatus ReadArrayEnd(Source& src, const ParserConfiguration<Source, Char>& config, bool& isArrayEnd)
	{
		auto status = SkipChars<Source, Char>(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		Char ch;
		status = src.PeekChar(ch);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		isArrayEnd = config.IsArrayEnd(ch);
		if(!isArrayEnd)
		{
			return EStatus::SUCCESS;
		}
		status = src.GetChar(ch);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		return EStatus::SUCCESS;
	}


	template<typename Source, typename Char>
	EStatus ReadArrayDecl(Source& src, const ParserConfiguration<Source, Char>& config, bool& isArray)
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

	template<typename Source, typename Char>
	EStatus ReadArrayDepth(Source& src, const ParserConfiguration<Source, Char>& config, uint8_t& depth)
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
