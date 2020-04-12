#pragma once
#include "ParserConfiguration.h"
#include "ObjectHandler.h"

namespace Parser
{
	template<typename Source, typename Token>
	bool ReadTypeDecl(Source& src, ParserConfiguration<Token>& config, ObjectDescriptor<Token>& objDesc)
	{
		//not implemented
		return false;
	}
}
