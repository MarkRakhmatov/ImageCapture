#pragma once
#include "CodeGeneration.h"
#include "ParserTypes.h"
#include "ObjectData.h"

#include <vector>
#include <string>
#include <cstring>

namespace Parser
{
	template<typename Char>
	struct ObjectDescriptor
	{
		EType type;
		uint8_t arrayDepth;
		std::basic_string<Char> name;
		ObjectData objectData;
	};

	template<typename Char>
	bool operator < (const ObjectDescriptor<Char>& lhs, const ObjectDescriptor<Char>& rhs)
	{
		return lhs.name < rhs.name;
	}
}
