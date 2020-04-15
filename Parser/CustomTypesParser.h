#pragma once
#include <vector>
#include <algorithm>

#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include "ObjectHandler.h"

namespace Parser
{
	template<typename Source, typename Token=char>
	bool ReadCustomType(Source& src, const ParserConfiguration<Token>& config, const ObjectDescriptor<Token>& typeObj, ObjectDescriptor<Token>& obj, bool isArray)
	{
		return false;
	}
}
