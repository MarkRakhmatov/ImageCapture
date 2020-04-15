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
			bool isArray = false;
			if(!ReadArrayDecl(src, config, isArray))
			{
				return false;
			}
			if(!ReadBlockStart(src, config))
			{
				return false;
			}
			if(config.IsReservedType(typeObj.objName))
			{
				if(!ReadReservedType(src, config, typeObj, obj, isArray))
				{
					return false;
				}
			}
			else
			{
				if(!ReadCustomType(src, config, typeObj, obj, isArray))
				{
					return false;
				}
			}

			if(!ReadBlockEnd(src, config))
			{
				return false;
			}
			return true;
		}
	};
}
