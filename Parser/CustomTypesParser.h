#pragma once
#include "ParserTypes.h"
#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include "ObjectHandler.h"

#include <vector>
#include <algorithm>


namespace Parser
{
	template<typename Source, typename Char>
	EStatus ReadCustomType(Source& src, const ParserConfiguration<Source, Char>& config, const ObjectDescriptor<Char>& typeObj, ObjectDescriptor<Char>& obj, bool isArray)
	{
		return EStatus::FAIL;
	}
}
