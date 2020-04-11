#pragma once
#include "ParserConfiguration.h"
#include "TypeHandler.h"
#include "ObjectHandler.h"

#include <string>
#include <vector>
#include <array>
#include <cstdint>

namespace Parser
{
	template<typename Source, typename Token=char>
	class Parser
	{
	public:
		using TypeID = int;
		using TypeName = std::basic_string<Token>;
		using ObjectName = std::basic_string<Token>;

		Parser(const ParserConfiguration<Token>& config)
		:mParserConfig(config)
		{}
		bool Parse(Source& src, ParserConfiguration<Token>& config, ObjectDescriptor<Token>& objDesc)
		{
			bool isTypeDecl = false;
			if(!IsTypeDeclaration(src, config, objDesc, isTypeDecl))
			{
				return false;
			}
			if(isTypeDecl)
			{
				return ReadTypeDecl(src, objDesc);
			}
			return ParseObject(src, objDesc);
		}
		bool IsTypeDeclaration(Source& src, ParserConfiguration<Token> config, ObjectDescriptor<Token>& objDesc, bool& isTypeDecl)
		{
			if(!ReadWord(src, config, objDesc.typeName))
			{
				return false;
			}
			if(config.IsTypeDecl(objDesc.typeName))
			{
				return true;
			}
			return true;
		}
		bool ParseObject(Source& src, const ParserConfiguration<Token> config, ObjectDescriptor<Token>& objDesc)
		{
			if(!ReadWord(src, config, objDesc.objName))
			{
				return false;
			}
			return mDataParser.ReadObjectData(src, config, objDesc);
		}
	private:
		ObjectName ReadObjectName(Source& src)
		{
			SkipIgnoreTokens(src);
			ObjectName objName;
			if(!ReadToFirstSkipToken(src, objName))
			{
				return ObjectName{};
			}
			return objName;
		}
	private:
		ParserConfiguration<Token>& mParserConfig;
		TypeDataParser<Source, Token> mDataParser;
	};
}
