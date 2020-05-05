#pragma once
#include "ParserTypes.h"
#include "ObjectDescriptor.h"

namespace Parser
{

	template<typename Source, typename Char>
	class ParserConfiguration;

	template<typename Source, typename Char>
	using ObjectDataReaderFunc = EStatus(*)(Source& src, const ParserConfiguration<Source, Char>& config, ObjectData& obj);

	template<typename Source, typename Char>
	struct TypeInfo
	{
		std::basic_string<Char> name;
		EType type;
		ObjectDataReaderFunc<Source, Char> reader;
		ObjectData data;
	};

	template<typename Source, typename Char>
	bool operator < (const TypeInfo<Source, Char>& lhs, const TypeInfo<Source, Char>& rhs)
	{
		return lhs.type < rhs.type;
	}
}
