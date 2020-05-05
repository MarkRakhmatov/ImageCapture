#pragma once
#include "CodeGeneration.h"
#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include "TypeDeclarationParser.h"
#include "ObjectDescriptor.h"
#include "ArrayReader.h"

#include <string>
#include <vector>
#include <array>

namespace Parser
{
	template<typename Source, typename Char>
	class ObjectParser
	{
	public:
		using TypeName = std::basic_string<Char>;
		using ObjectName = std::basic_string<Char>;

		EStatus Parse(Source& src, ParserConfiguration<Source, Char>& config, ObjectDescriptor<Char>& objDesc)
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
		EStatus IsTypeDeclaration(Source& src, const ParserConfiguration<Source, Char>& config, ObjectDescriptor<Char>& objDesc, bool& isTypeDecl)
		{
			std::basic_string<Char> str;
			EStatus status = ReadWord<Source, std::basic_string<Char>, Char>(src, config, str);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			if(config.IsTypeDecl(str))
			{
				isTypeDecl = true;
				return EStatus::SUCCESS;
			}

			objDesc.type = config.GetTypeID(str);
			RET_ON_TRUE(objDesc.type == EType::INVALID, status);

			isTypeDecl = false;
			return EStatus::SUCCESS;
		}

		EStatus ParseObject(Source& src, const ParserConfiguration<Source, Char>& config, ObjectDescriptor<Char>& objDesc)
		{
			EStatus status = ReadWord<Source, std::basic_string<Char>, Char>(src, config, objDesc.name);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			// TODO: check that type name is unique
			status = ReadArrayDepth(src, config, objDesc.arrayDepth);

			auto TypeReader = config.GetTypeReader(objDesc.type);
			RET_ON_FALSE(TypeReader, EStatus::FAIL);

			return ReadArray(src, config, TypeReader, objDesc);
		}
	};
}
