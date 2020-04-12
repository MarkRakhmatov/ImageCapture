#pragma once
#include "ObjectHandler.h"

#include <vector>
#include <string>
#include <set>
#include <algorithm>

namespace Parser
{
	template<typename Token=char>
	class ParserConfiguration
	{
	public:
		ParserConfiguration()=default;
		ParserConfiguration(const std::basic_string<Token>& typeName, const std::vector<ObjectDescriptor<Token>>& types, const std::vector<Token>& ignoreTokens, const std::pair<Token, Token>& objDataBrackets)
		: mTypeName(typeName)
		, mTypes(types.begin(), types.end())
		, mIgnoreTokens(ignoreTokens)
		, mObjDataBrackets(objDataBrackets)
		{
		}
		bool AddType(const ObjectDescriptor<Token>& type)
		{
			auto iter = mTypes.find(type);
			if(iter != mTypes.end())
			{
				return false;
			}
			mTypes.insert(type);
			return true;
		}
		bool GetType(const std::basic_string<Token>& typeName, ObjectDescriptor<Token>& type) const
		{
			auto iter = std::find_if(mTypes.begin(), mTypes.end(),
					[&typeName](const ObjectDescriptor<Token>& type)
					{
						return type.objName == typeName;
					});
			if(iter == mTypes.end())
			{
				return false;
			}
			type = *iter;
			return true;
		}
		bool IsSkipToken(Token t) const
		{
			auto iter = std::find(mIgnoreTokens.begin(), mIgnoreTokens.end(), t);
			return iter != mIgnoreTokens.end();
		}
		bool IsReservedType(const std::basic_string<Token>& typeName) const
		{
			auto iter = std::find_if(mTypes.begin(), mTypes.end(),
								[&typeName](const ObjectDescriptor<Token>& type)
								{
									return type.objName == typeName;
								});
			return iter != mTypes.end();
		}
		bool IsDataStartSymbol(Token t) const
		{
			return t == mObjDataBrackets.first;
		}

		bool IsDataEndSymbol(Token t) const
		{
			return t == mObjDataBrackets.second;
		}
		bool IsTypeDecl(const std::basic_string<Token>& typeName) const
		{
			return typeName == mTypeName;
		}
	private:
		std::basic_string<Token> mTypeName;
		std::set<ObjectDescriptor<Token>> mTypes;
		std::vector<Token> mIgnoreTokens;
		std::pair<Token, Token> mObjDataBrackets;
	};

	ParserConfiguration<char> GetDefaultParserConfig()
	{
		ObjectDescriptor<char> desc;
		desc.objName = "Int";
		desc.typeName = "Type";
		std::vector<ObjectDescriptor<char>> types{desc};
		ParserConfiguration<char> config(std::string("Type"), types,{' ', '\n'}, {'{','}'});
		return config;
	}
}
