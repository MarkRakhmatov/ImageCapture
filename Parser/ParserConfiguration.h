#pragma once
#include "ObjectHandler.h"

#include <vector>
#include <string>
#include <set>
#include <algorithm>

namespace Parser
{
	template<typename Token>
	class ParserConfiguration
	{
	public:
		ParserConfiguration()=default;
		ParserConfiguration(const std::basic_string<Token>& typeName,
				const std::vector<ObjectDescriptor<Token>>& types,
				const std::vector<Token>& ignoreTokens,
				const std::pair<Token, Token>& objDataBrackets,
				const std::pair<Token, Token>& stringBrackets,
				const std::pair<Token, Token>& charBrackets,
				const std::pair<Token, Token>& arrayBrackets,
				const std::pair<Token, Token>& digits,
				const std::pair<Token, Token>& uppercaseLetters,
				const std::pair<Token, Token>& lowercaseLetters,
				Token separator)
		: mTypeName(typeName)
		, mTypes(types.begin(), types.end())
		, mIgnoreTokens(ignoreTokens)
		, mObjDataBrackets(objDataBrackets)
		, mStringBrackets(stringBrackets)
		, mCharBrackets(charBrackets)
		, mArrayBrackets(arrayBrackets)
		, mDigits(digits)
		, mUppercaseLetters(uppercaseLetters)
		, mLowercaseLetters(lowercaseLetters)
		, mSeparator(separator)
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
		bool IsSkipToken(Token token) const
		{
			auto iter = std::find(mIgnoreTokens.begin(), mIgnoreTokens.end(), token);
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
		bool IsBlockStart(Token token) const
		{
			return token == mObjDataBrackets.first;
		}

		bool IsBlockEnd(Token token) const
		{
			return token == mObjDataBrackets.second;
		}
		bool IsStringStart(Token token) const
		{
			return token == mStringBrackets.first;
		}

		bool IsStringEnd(Token token) const
		{
			return token == mStringBrackets.second;
		}
		bool IsCharStart(Token token) const
		{
			return token == mCharBrackets.first;
		}

		bool IsCharEnd(Token token) const
		{
			return token == mCharBrackets.second;
		}
		bool IsArrayStart(Token token) const
		{
			return token == mArrayBrackets.first;
		}

		bool IsArrayEnd(Token token) const
		{
			return token == mArrayBrackets.second;
		}

		bool IsDigit(Token token) const
		{
			return mDigits.first <= token && token <=mDigits.second;
		}

		bool IsLetter(Token token) const
		{
			return (mUppercaseLetters.first <= token && token <= mUppercaseLetters.second)
					|| (mLowercaseLetters.first <= token && token<=mLowercaseLetters.second);
		}
		bool IsSeparator(Token token) const
		{
			return token == mSeparator;
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
		std::pair<Token, Token> mStringBrackets;
		std::pair<Token, Token> mCharBrackets;
		std::pair<Token, Token> mArrayBrackets;
		std::pair<Token, Token> mDigits;
		std::pair<Token, Token> mUppercaseLetters;
		std::pair<Token, Token> mLowercaseLetters;
		Token mSeparator;
	};

	ParserConfiguration<char> GetDefaultParserConfig()
	{
		ParserConfiguration<char> config(
				"Type",
				{
					{"Type", "int32"},
					{"Type", "char"},
					{"Type", "string"}
				},
				{' ', '\n', '\t'},
				{'{','}'},
				{'\"','\"'},
				{'\'', '\''},
				{'[', ']'},
				{'0', '9'},
				{'A', 'Z'},
				{'a', 'z'},
				',');
		return config;
	}
}
