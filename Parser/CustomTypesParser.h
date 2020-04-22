#pragma once
#include "ParserTypes.h"
#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include "ObjectHandler.h"

#include <vector>
#include <algorithm>


namespace Parser
{
	template<typename Source, typename Token=char>
	EStatus ReadCustomType(Source& src, const ParserConfiguration<Token>& config, const ObjectDescriptor<Token>& typeObj, ObjectDescriptor<Token>& obj, bool isArray)
	{
		return EStatus::FAIL;
	}
}
