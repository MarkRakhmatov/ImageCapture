#pragma once
#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include "TypeDeclarationParser.h"
#include "TypeDataParser.h"
#include "ObjectHandler.h"

#include <string>
#include <vector>
#include <array>

namespace Parser
{
	template<typename Source, typename Token>
	class ObjectParser
	{
	public:
		using TypeName = std::basic_string<Token>;
		using ObjectName = std::basic_string<Token>;

		EStatus Parse(Source& src, ParserConfiguration<Token>& config, ObjectDescriptor<Token>& objDesc)
		{
			bool isTypeDecl = false;
			EStatus status = IsTypeDeclaration(src, config, objDesc, isTypeDecl);
			RET_ON_FAIL(status == EStatus::SUCCESS, status);
			if(isTypeDecl)
			{
				return ReadTypeDecl(src, config, objDesc);
			}
			return ParseObject(src, config, objDesc);
		}
	private:
		EStatus IsTypeDeclaration(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& objDesc, bool& isTypeDecl)
		{
			EStatus status = ReadWord<Source, std::basic_string<Token>, Token>(src, config, objDesc.typeName);
			RET_ON_FAIL(status == EStatus::SUCCESS, status);
			if(config.IsTypeDecl(objDesc.typeName))
			{
				isTypeDecl = true;
				return EStatus::SUCCESS;
			}
			isTypeDecl = false;
			return EStatus::FAIL;
		}
		EStatus ParseObject(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& objDesc)
		{
			EStatus status = ReadWord<Source, std::basic_string<Token>, Token>(src, config, objDesc.objName);
			RET_ON_FAIL(status == EStatus::SUCCESS, status);
			return mDataParser.ReadObject(src, config, objDesc);
		}
	private:
		TypeDataParser<Source, Token> mDataParser;
	};
}
