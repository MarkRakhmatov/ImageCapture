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
		if(typeObj.objName=="Int")
		{
			return ReadIntObject(src, config, obj);
		}
		return false;
	}


	template<typename Source, typename Token=char>
	class TypeDataParser
	{
	public:
		bool ReadObjectData(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj)
		{
			ObjectDescriptor<Token> typeObj;
			if(!config.GetType(obj.typeName, typeObj))
			{
				return false;
			}
			if(!ReadObjectData(src, config, typeObj, obj))
			{
				return false;
			}
			return true;
		}
	private:
		bool ReadObjectData(Source& src, const ParserConfiguration<Token>& config, const ObjectDescriptor<Token>& typeObj, ObjectDescriptor<Token>& obj)
		{
			if(config.IsReservedType(typeObj.objName))
			{
				return ReadReservedType(src, config, typeObj, obj);
			}
			return ReadCustomType(src, config, typeObj, obj);
		}
	};
}
