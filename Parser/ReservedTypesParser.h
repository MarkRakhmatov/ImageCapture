#pragma once
#include <vector>
#include <sstream>
#include <algorithm>

#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include "ObjectHandler.h"

namespace Parser
{
	template<typename Num, typename Source, typename Token=char>
	bool ReadNumericObject(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj)
	{
		if(!ReadObjectStart(src, config))
		{
			return false;
		}
		SkipTokens(src, config);
		if(!ReadObjectData<Num, Source, Token>(src, obj))
		{
			return false;
		}
		SkipTokens(src, config);
		Token token;
		if(!src.GetToken(token))
		{
			return false;
		}

		if(config.IsBlockEnd(token))
		{
			return true;
		}
		return false;
	}

	template<typename Source, typename Token=char>
	bool ReadCharObject(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj)
	{
		if(!ReadObjectStart(src, config))
		{
			return false;
		}
		SkipTokens(src, config);
		Token token;
		if(!src.GetToken(token))
		{
			return false;
		}
		if(!config.IsCharStart(token))
		{
			return false;
		}

		if(!src.GetToken(token))
		{
			return false;
		}
		obj.objectData.push_back(token);

		if(!src.GetToken(token))
		{
			return false;
		}
		if(!config.IsCharEnd(token))
		{
			return false;
		}

		SkipTokens(src, config);
		if(!src.GetToken(token))
		{
			return false;
		}
		if(config.IsBlockEnd(token))
		{
			return true;
		}
		return false;
	}

	template<typename Source, typename Token=char>
	bool ReadStringObject(Source& src, const ParserConfiguration<Token>& config, ObjectDescriptor<Token>& obj)
	{
		if(!ReadObjectStart(src, config))
		{
			return false;
		}
		SkipTokens(src, config);

		Token token;
		if(!src.GetToken(token))
		{
			return false;
		}
		if(!config.IsStringStart(token))
		{
			return false;
		}
		for(;;)
		{
			if(!src.GetToken(token))
			{
				return false;
			}
			if(config.IsStringEnd(token))
			{
				break;
			}
			obj.objectData.push_back(token);
		}
		SkipTokens(src, config);

		if(!src.GetToken(token))
		{
			return false;
		}

		if(config.IsBlockEnd(token))
		{
			return true;
		}
		return false;
	}
}
