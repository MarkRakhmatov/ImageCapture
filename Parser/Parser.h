#pragma once
#include "ParserConfiguration.h"
#include "TypeDeclarationParser.h"
#include "TypeDataParser.h"
#include "ObjectHandler.h"

#include <string>
#include <vector>
#include <array>

namespace Parser
{
	template<typename Source, typename Token=char>
	class Parser
	{
	public:
		using TypeName = std::basic_string<Token>;
		using ObjectName = std::basic_string<Token>;

		bool Parse(Source& src, ParserConfiguration<Token>& config, ObjectDescriptor<Token>& objDesc)
		{
			bool isTypeDecl = false;
			if(!IsTypeDeclaration(src, config, objDesc, isTypeDecl))
			{
				return false;
			}
			if(isTypeDecl)
			{
				return ReadTypeDecl(src, config, objDesc);
			}
			return ParseObject(src, config, objDesc);
		}
	private:
		bool IsTypeDeclaration(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& objDesc, bool& isTypeDecl)
		{
			if(!ReadWord(src, config, objDesc.typeName))
			{
				return false;
			}
			if(config.IsTypeDecl(objDesc.typeName))
			{
				isTypeDecl = true;
				return true;
			}
			isTypeDecl = false;
			return true;
		}
		bool ParseObject(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& objDesc)
		{
			if(!ReadWord(src, config, objDesc.objName))
			{
				return false;
			}
			return mDataParser.ReadObject(src, config, objDesc);
		}
	private:
		TypeDataParser<Source, Token> mDataParser;
	};
}
