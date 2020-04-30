#pragma once
#include "ParserTypes.h"
#include "ObjectHandler.h"

namespace Parser
{

	template<typename Source, typename Token>
	class ParserConfiguration;

	template<typename Source, typename Token>
	using ReaderFunc = EStatus(*)(Source& src, const ParserConfiguration<Source, Token>& config, ObjectDescriptor<Token>& obj);

	template<typename Source, typename Token>
	struct TypeInfo
	{
		std::basic_string<Token> name;
		EType type;
		ReaderFunc<Source, Token> reader;
	};
}
