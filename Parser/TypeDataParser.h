#pragma once
#include <vector>
#include <algorithm>

#include "ParserConfiguration.h"
#include "ObjectHandler.h"


namespace
{
	template<typename Container, typename ValueType>
	bool Contains(const Container& container, const ValueType& val)
	{
		return std::find(container.begin(), container.end(), val)!=container.end();
	}
}

namespace Parser
{
	using TypeID = int;
	template<typename Token=char>
	using Tokens = std::vector<Token>;
	template<typename Token=char>
	using Brackets = std::pair<Token, Token>;

	enum EPrimitiveTypes : TypeID
	{
		INVALID = 0,
		CHAR,
		INTEGER,
		FLOAT
	};

	template<typename Token=char>
	struct ObjParsingRules
	{
		Brackets<Token> brackets;
		Tokens<Token> allowedTokens;
		Tokens<Token> deniedTokens;
		Tokens<Token> skipTokens;
	};

	template<typename Token=char>
	ObjParsingRules<Token> GetIntParsingRules()
	{
		ObjParsingRules<Token> rules;
		rules.brackets = {'{','}'};
		rules.allowedTokens = {'0','1','2','3','4','5','6','7','8','9'};
		rules.skipTokens = {' ', '\n'};
		return rules;
	}
	// int data parser
	// string data parser
	// array data parser
	// matrix data parser
	// ParseData(dataSource, ObjectDescriptor<Identifier, Token>)
	template<typename Source, typename Token=char>
	bool ReadTokens(Source& src, const Tokens<Token>& tokens)
	{
		for(auto& token : tokens)
		{
			if(!src.GetToken(token))
			{
				return false;
			}
		}
		return true;
	}
	template<typename Token=char>
	bool IsDataStart(Token token, const ObjParsingRules<Token>& rules)
	{
		if(rules.brackets.first == token)
		{
			return true;
		}
		return false;
	}
	template<typename Token=char>
	bool IsDataEnd(Token token, const ObjParsingRules<Token>& rules)
	{
		if(rules.brackets.second == token)
		{
			return true;
		}
		return false;
	}

	template<typename Source, typename Token=char>
	bool ReadObjectStart(Source& src, const ObjParsingRules<Token> rules, const Tokens<Token>& tokens)
	{
		for(auto& token : tokens)
		{
			if(!src.GetToken(token))
			{
				return false;
			}
			if(Contains(rules.skipTokens, token))
			{
				continue;
			}
			if(IsDataStart(token, rules))
			{
				return true;
			}
			break;
		}
		return false;
	}

	template<typename Source, typename Token=char>
	bool SkipTokens(Source& src, const ParserConfiguration<Token>& config)
	{
		Token t;
		for(;;)
		{
			if(!src.PeekToken(t))
			{
				return false;
			}
			if(!config.IsSkipToken(t))
			{
				break;
			}
			src.GetToken(t);
		}
		return true;
	}

	template<typename Source, typename Container, typename Token=char>
	bool ReadWord(Source& src, const ParserConfiguration<Token>& config, const Container& tokens)
	{
		if(!SkipTokens<Source, Token>(src, config))
		{
			return false;
		}
		Token token;
		for(;;)
		{
			if(!src.GetToken(token))
			{
				return false;
			}
			if(!config.IsSkipToken(token))
			{
				break;
			}
			tokens.push_back(token);
		}
		return false;
	}

	template<typename Source, typename Token=char>
	bool ReadObjectData(Source& src, ParserConfiguration<Token>& config, ObjectDescriptor<Token>& typeObj, ObjectDescriptor<Token>& obj)
	{
		if(config.IsReservedType(typeObj))
		{
			return ReadReservedType(src, config, typeObj, obj);
		}
		return ReadCustomType(src, config, typeObj, obj);
	}

	template<typename Source, typename Token=char>
	class TypeDataParser
	{
	public:
		bool ReadObjectData(Source& src, ParserConfiguration<Token>& config, ObjectDescriptor<TypeID>& obj)
		{
			if(!ReadObJectStart(src, obj.objectData))
			{
				return false;
			}
			ObjectDescriptor<Token> typeObj;
			if(!config.GetType(obj.objName, typeObj))
			{
				return false;
			}
			if(!ReadObjectData(src, config, typeObj, obj))
			{
				return false;
			}
			return true;
		}
	};
}
