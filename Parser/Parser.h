#pragma once
#include "CodeGeneration.h"
#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include "TypeDeclarationParser.h"
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

		EStatus Parse(Source& src, ParserConfiguration<Source, Token>& config, ObjectDescriptor<Token>& objDesc)
		{
			bool isTypeDecl = false;
			EStatus status = IsTypeDeclaration(src, config, objDesc, isTypeDecl);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			if(isTypeDecl)
			{
				return ReadTypeDecl(src, config, objDesc);
			}
			return ParseObject(src, config, objDesc);
		}
	private:
		EStatus IsTypeDeclaration(Source& src, const ParserConfiguration<Source, Token>& config, ObjectDescriptor<Token>& objDesc, bool& isTypeDecl)
		{
			std::basic_string<Token> str;
			EStatus status = ReadWord<Source, std::basic_string<Token>, Token>(src, config, str);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			if(config.IsTypeDecl(str))
			{
				isTypeDecl = true;
				return EStatus::SUCCESS;
			}

			objDesc.type = config.GetTypeID(str);
			RET_ON_FALSE(objDesc.type != 0, status);

			isTypeDecl = false;
			return EStatus::SUCCESS;
		}

		EStatus ParseObject(Source& src, const ParserConfiguration<Source, Token>& config, ObjectDescriptor<Token>& objDesc)
		{
			EStatus status = ReadWord<Source, std::basic_string<Token>, Token>(src, config, objDesc.objName);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			uint8_t arrayDepth{0};
			status = ReadArrayDepth(src, config, arrayDepth);

			status = ReadBlockStart(src, config);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			auto TypeReader = config.GetTypeReader(objDesc.type);
			RET_ON_FALSE(TypeReader, EStatus::FAIL);

			status = TypeReader(src, config, objDesc);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			status = ReadBlockEnd(src, config);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			return EStatus::SUCCESS;
		}
	};
}
