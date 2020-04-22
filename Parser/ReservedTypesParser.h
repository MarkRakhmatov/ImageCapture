#pragma once
#include <vector>
#include <sstream>
#include <algorithm>

#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include "ObjectHandler.h"

namespace Parser
{

	template<typename Source, typename Token>
	using ReaderFunc = EStatus(*)(Source&, const ParserConfiguration<Token>&, ObjectDescriptor<Token>& obj);

	template<typename Num, typename Source, typename Token=char>
	EStatus ReadNumericObject(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj)
	{
		auto status = SkipTokens(src, config);
		RET_ON_FAIL(status == EStatus::SUCCESS, status);
		return ReadObjectData<Num, Source, Token>(src, obj);
	}

	template<typename Source, typename Token=char>
	EStatus ReadCharObject(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj)
	{
		auto status = SkipTokens(src, config);
		RET_ON_FAIL(status == EStatus::SUCCESS, status);
		Token token;
		status = src.GetToken(token);
		RET_ON_FAIL(status == EStatus::SUCCESS, status);
		if(!config.IsCharStart(token))
		{
			return EStatus::FAIL;
		}

		status = src.GetToken(token);
		RET_ON_FAIL(status == EStatus::SUCCESS, status);

		obj.objectData.push_back(token);

		status = src.GetToken(token);
		RET_ON_FAIL(status == EStatus::SUCCESS, status);

		if(!config.IsCharEnd(token))
		{
			return EStatus::FAIL;
		}
		return EStatus::SUCCESS;
	}

	template<typename Source, typename Token=char>
	EStatus ReadStringObject(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj)
	{
		auto status = SkipTokens(src, config);
		RET_ON_FAIL(status == EStatus::SUCCESS, status);

		Token token;
		status = src.GetToken(token);
		RET_ON_FAIL(status == EStatus::SUCCESS, status);

		if(!config.IsStringStart(token))
		{
			return EStatus::FAIL;
		}
		for(;;)
		{
			status = src.GetToken(token);
			RET_ON_FAIL(status == EStatus::SUCCESS, status);

			if(config.IsStringEnd(token))
			{
				break;
			}
			obj.objectData.push_back(token);
		}
		return EStatus::SUCCESS;
	}
	template<typename Source, typename Token>
	EStatus HandleReadArray(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj, bool isArray, ReaderFunc<Source, Token> reader)
	{
		if(!isArray)
		{
			return reader(src, config, obj);
		}
		auto status = EStatus::FAIL;
		for(;;)
		{
			ObjectDescriptor<Token> suboOj;
			status = reader(src, config, suboOj);
			RET_ON_FAIL(status == EStatus::SUCCESS, status);
			obj.subObjects.push_back(suboOj);
			status = SkipTokens(src, config);
			RET_ON_FAIL(status == EStatus::SUCCESS, status);
			Token token;
			status = src.PeekToken(token);
			RET_ON_FAIL(status == EStatus::SUCCESS, status);
			if(!config.IsSeparator(token))
			{
				return EStatus::SUCCESS;
			}

			status = src.GetToken(token);
			RET_ON_FAIL(status == EStatus::SUCCESS, status);
		}
		return EStatus::FAIL;
	}

	template<typename Source, typename Token>
	EStatus ReadReservedType(Source& src, const ParserConfiguration<Token>& config, const ObjectDescriptor<Token>& typeObj, ObjectDescriptor<Token>& obj, bool isArray)
	{
		if(typeObj.objName=="int32")
		{
			return HandleReadArray<Source, Token>(src, config, obj, isArray, &ReadNumericObject<int32_t>);
		}
		if(typeObj.objName=="char")
		{
			return HandleReadArray<Source, Token>(src, config, obj, isArray, &ReadCharObject);
		}
		if(typeObj.objName=="string")
		{
			return HandleReadArray<Source, Token>(src, config, obj, isArray, &ReadStringObject);
		}
		return EStatus::FAIL;
	}
}
