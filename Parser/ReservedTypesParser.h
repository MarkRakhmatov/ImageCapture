#pragma once
#include <vector>
#include <algorithm>

#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include "ObjectHandler.h"

namespace Parser
{
	template<typename Source, typename Token=char>
	bool ReadIntObject(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj)
	{
		std::vector<Token> allowedTokens{'0','1','2','3','4','5','6','7','8','9'};
		if(!ReadObjectStart(src, config))
		{
			return false;
		}
		SkipTokens(src, config);
		Token token;
		for(;;)
		{
			if(!src.GetToken(token))
			{
				return false;
			}
			if(config.IsSkipToken(token))
			{
				break;
			}
			if(config.IsDataEndSymbol(token))
			{
				return true;
			}
			if(!Contains(allowedTokens, token))
			{
				return false;
			}
			obj.objectData.push_back(token);
		}
		SkipTokens(src, config);

		if(!src.GetToken(token))
		{
			return false;
		}

		if(config.IsDataEndSymbol(token))
		{
			return true;
		}
		return true;
	}
}
