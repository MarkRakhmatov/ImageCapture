#pragma once
#include "CodeGeneration.h"
#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include "ObjectDescriptor.h"

#include <string>

namespace Parser
{
	template<typename Source, typename Char>
	EStatus ReadTypeDecl(Source& src, ParserConfiguration<Source, Char>& config, ObjectDescriptor<Char>& objDesc)
	{
		return EStatus::FAIL;
	}
}
