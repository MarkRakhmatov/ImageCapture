#pragma once
#include "CodeGeneration.h"
#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include "ObjectHandler.h"

#include <string>

namespace Parser
{
	template<typename Source, typename Char>
	EStatus ReadTypeDecl(Source& src, ParserConfiguration<Source, Char>& config, ObjectDescriptor<Char>& objDesc)
	{
		// read typename
		/*EStatus status = ReadWord<Source, std::basic_string<Token>, Token>(src, config, objDesc.objName);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		// read block start
		status = ReadBlockStart<Source, Token>(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		// read subobjects
		for(;;)
		{
			ObjectDescriptor<Token> subObj;

			status = ReadWord<Source, std::basic_string<Token>, Token>(src, config, subObj.type);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			status = ReadWord<Source, std::basic_string<Token>, Token>(src, config, subObj.name);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			Token token;
			status = SkipTokens<Source, Token>(src, config);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			status = src.PeekToken(token);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			if(config.IsBlockEnd(token))
			{
				break;
			}
		}
		return ReadBlockEnd<Source, Token>(src, config);*/
		return EStatus::FAIL;
	}
}
