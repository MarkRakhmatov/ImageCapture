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
	using ReaderFunc = bool(*)(Source&, const ParserConfiguration<Token>&, ObjectDescriptor<Token>& obj);

	template<typename Num, typename Source, typename Token=char>
	bool ReadNumericObject(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj)
	{
		SkipTokens(src, config);
		if(!ReadObjectData<Num, Source, Token>(src, obj))
		{
			return false;
		}
		return true;
	}

	template<typename Source, typename Token=char>
	bool ReadCharObject(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj)
	{
		SkipTokens(src, config);
		Token token;
		if(!src.GetToken(token))
		{
			return false;
		}
		if(!config.IsCharStart(token))
		{
			return false;
		}

		if(!src.GetToken(token))
		{
			return false;
		}
		obj.objectData.push_back(token);

		if(!src.GetToken(token))
		{
			return false;
		}
		if(!config.IsCharEnd(token))
		{
			return false;
		}
		return true;
	}

	template<typename Source, typename Token=char>
	bool ReadStringObject(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj)
	{
		SkipTokens(src, config);

		Token token;
		if(!src.GetToken(token))
		{
			return false;
		}
		if(!config.IsStringStart(token))
		{
			return false;
		}
		for(;;)
		{
			if(!src.GetToken(token))
			{
				return false;
			}
			if(config.IsStringEnd(token))
			{
				break;
			}
			obj.objectData.push_back(token);
		}
		return true;
	}
	template<typename Source, typename Token>
	bool HandleReadArray(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj, bool isArray, ReaderFunc<Source, Token> reader)
	{
		if(!isArray)
		{
			return reader(src, config, obj);
		}
		for(;;)
		{
			ObjectDescriptor<Token> suboOj;
			if(!reader(src, config, suboOj))
			{
				return false;
			}
			obj.subObjects.push_back(suboOj);
			if(!SkipTokens(src, config))
			{
				return false;
			}
			Token token;
			if(!src.PeekToken(token))
			{
				return false;
			}
			if(!config.IsSeparator(token))
			{
				return true;
			}

			if(!src.GetToken(token))
			{
				return false;
			}
		}
		return false;
	}

	template<typename Source, typename Token>
	bool ReadReservedType(Source& src, const ParserConfiguration<Token>& config, const ObjectDescriptor<Token>& typeObj, ObjectDescriptor<Token>& obj, bool isArray)
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
		return false;
	}
}
