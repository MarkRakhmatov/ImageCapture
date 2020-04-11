#pragma once
#include "TypeHandler.h"

#include <vector>
#include <set>
#include <algorithm>

namespace Parser
{
	template<typename Token=char>
	class ParserConfiguration
	{
	public:
		using TypeID = int;

		ParserConfiguration()=default;
		ParserConfiguration(const std::vector<Token>& ignoreTokens, const std::basic_string<Token>& typeName)
		: mIgnoreTokens(ignoreTokens)
		, mTypeName(typeName)
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
		bool GetType(const std::basic_string<Token>& typeName, ObjectDescriptor<Token>& type)
		{
			auto iter = std::find_if(mTypes.begin(), mTypes.end(),
					[&typeName](ObjectDescriptor<Token>& type)
					{
						return type.objName = typeName;
					});
			if(iter == mTypes.end())
			{
				return false;
			}
			type = *iter;
			return true;
		}
		bool IsSkipToken(Token t)
		{
			auto iter = std::find(mIgnoreTokens.begin(), mIgnoreTokens.end(), t);
			return iter != mIgnoreTokens.end();
		}
		bool IsTypeDecl(const std::basic_string<Token>& typeName)
		{
			return typeName == mTypeName;
		}
	private:
		std::basic_string<Token> mTypeName;
		std::set<ObjectDescriptor<Token>> mTypes;
		std::vector<Token> mIgnoreTokens;
	};

	ParserConfiguration<char> GetDefaultParserConfig()
	{
		return ParserConfiguration<char>({' ', '\n'}, "Type");
	}
}
