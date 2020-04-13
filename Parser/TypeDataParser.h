#pragma once
#include <vector>
#include <algorithm>

#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include "ReservedTypesParser.h"
#include "CustomTypesParser.h"
#include "ObjectHandler.h"

namespace Parser
{
	template<typename Source, typename Token=char>
	bool ReadReservedType(Source& src, const ParserConfiguration<Token>& config, const ObjectDescriptor<Token>& typeObj, ObjectDescriptor<Token>& obj)
	{
		if(typeObj.objName=="int32")
		{
			return ReadNumericObject<int32_t>(src, config, obj);
		}
		if(typeObj.objName=="char")
		{
			return ReadCharObject(src, config, obj);
		}
		if(typeObj.objName=="string")
		{
			return ReadStringObject(src, config, obj);
		}
		return false;
	}


	template<typename Source, typename Token=char>
	class TypeDataParser
	{
	public:
		bool ReadObject(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj)
		{
			ObjectDescriptor<Token> typeObj;
			if(!config.GetType(obj.typeName, typeObj))
			{
				return false;
			}
			if(!ReadObject(src, config, typeObj, obj))
			{
				return false;
			}
			return true;
		}
	private:
		bool ReadObject(Source& src, const ParserConfiguration<Token>& config, const ObjectDescriptor<Token>& typeObj, ObjectDescriptor<Token>& obj)
		{
			if(config.IsReservedType(typeObj.objName))
			{
				return ReadReservedType(src, config, typeObj, obj);
			}
			return ReadCustomType(src, config, typeObj, obj);
		}
	};
}
