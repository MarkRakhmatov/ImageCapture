#pragma once
#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include "ReservedTypesParser.h"
#include "CustomTypesParser.h"
#include "ObjectHandler.h"

#include <vector>
#include <algorithm>

namespace Parser
{
	template<typename Source, typename Token=char>
	class TypeDataParser
	{
	public:
		EStatus ReadObject(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj)
		{
			ObjectDescriptor<Token> typeObj;
			if(!config.GetType(obj.typeName, typeObj))
			{
				return EStatus::FAIL;
			}
			auto status = ReadObject(src, config, typeObj, obj);
			RET_ON_FAIL(status == EStatus::SUCCESS, status);
			return EStatus::SUCCESS;
		}
	private:
		EStatus ReadObject(Source& src, const ParserConfiguration<Token>& config, const ObjectDescriptor<Token>& typeObj, ObjectDescriptor<Token>& obj)
		{
			bool isArray = false;
			auto status = ReadArrayDecl(src, config, isArray);
			RET_ON_FAIL(status == EStatus::SUCCESS, status);
			status = ReadBlockStart(src, config);
			RET_ON_FAIL(status == EStatus::SUCCESS, status);
			if(config.IsReservedType(typeObj.objName))
			{
				status = ReadReservedType(src, config, typeObj, obj, isArray);
				RET_ON_FAIL(status == EStatus::SUCCESS, status);
			}
			else
			{
				status = ReadCustomType(src, config, typeObj, obj, isArray);
				RET_ON_FAIL(status == EStatus::SUCCESS, status);
			}
			status = ReadBlockEnd(src, config);
			RET_ON_FAIL(status == EStatus::SUCCESS, status);
			return EStatus::SUCCESS;
		}
	};
}
