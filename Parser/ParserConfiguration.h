#pragma once
#include "ParserTypes.h"
#include "ObjectDescriptor.h"
#include "TypeInfo.h"
#include "ReservedTypesParser.h"

#include <vector>
#include <string>
#include <set>
#include <algorithm>

namespace Parser
{
	template<typename Source, typename Char>
	class ParserConfiguration
	{
	public:
		ParserConfiguration()=default;
		ParserConfiguration(const std::basic_string<Char>& typeName,
				const std::vector<TypeInfo<Source, Char>>& types,
				const std::vector<Char>& ignoreChars,
				const std::vector<Char>& specCharacters,
				const std::pair<Char, Char>& objDataBrackets,
				const std::pair<Char, Char>& stringBrackets,
				const std::pair<Char, Char>& charBrackets,
				const std::pair<Char, Char>& arrayBrackets,
				const std::pair<Char, Char>& digits,
				const std::pair<Char, Char>& uppercaseLetters,
				const std::pair<Char, Char>& lowercaseLetters,
				Char separator,
				Char escapeSequenceStart)
		: mTypeName(typeName)
		, mTypes(types.begin(), types.end())
		, mIgnoreChars(ignoreChars)
		, mSpecCharacters(specCharacters)
		, mObjDataBrackets(objDataBrackets)
		, mStringBrackets(stringBrackets)
		, mCharBrackets(charBrackets)
		, mArrayBrackets(arrayBrackets)
		, mDigits(digits)
		, mUppercaseLetters(uppercaseLetters)
		, mLowercaseLetters(lowercaseLetters)
		, mSeparator(separator)
		, mEscapeSequenceStart(escapeSequenceStart)
		{
		}

		void AddType(const ObjectDescriptor<Char>& type)
		{
			TypeInfo<Source, Char> typeInfo;
			typeInfo.name = type.name;
			++mCustomTypesCount;
			typeInfo.type = static_cast<EType>(static_cast<uint32_t>(EType::LAST_RESERVED) + mCustomTypesCount);
			typeInfo.data = std::move(type.objectData);
			mTypes.insert(typeInfo);
		}
		EType GetTypeID(const std::basic_string<Char>& typeName) const
		{
			for(auto& typeObj : mTypes)
			{
				if(typeObj.name == typeName)
				{
					return typeObj.type;
				}
			}
			return EType::INVALID;
		}

		ObjectDataReaderFunc<Source, Char> GetTypeReader(uint32_t typeID) const
		{
			for(auto& typeObj : mTypes)
			{
				if(typeObj.type == typeID)
				{
					return typeObj.reader;
				}
			}
			return nullptr;
		}

		bool IsSkipChar(Char ch) const
		{
			auto iter = std::find(mIgnoreChars.begin(), mIgnoreChars.end(), ch);
			return iter != mIgnoreChars.end();
		}
		bool IsEscapeSequenceStart(Char ch) const
		{
			return mEscapeSequenceStart == ch;
		}
		bool IsSpecCharacter(Char ch) const
		{
			auto iter = std::find(mSpecCharacters.begin(), mSpecCharacters.end(), ch);
			return iter != mSpecCharacters.end();
		}

		bool IsBlockStart(Char ch) const
		{
			return ch == mObjDataBrackets.first;
		}

		bool IsBlockEnd(Char ch) const
		{
			return ch == mObjDataBrackets.second;
		}

		bool IsStringStart(Char ch) const
		{
			return ch == mStringBrackets.first;
		}

		bool IsStringEnd(Char ch) const
		{
			return ch == mStringBrackets.second;
		}
		bool IsCharStart(Char ch) const
		{
			return ch == mCharBrackets.first;
		}

		bool IsCharEnd(Char ch) const
		{
			return ch == mCharBrackets.second;
		}
		bool IsArrayStart(Char ch) const
		{
			return ch == mArrayBrackets.first;
		}

		bool IsArrayEnd(Char ch) const
		{
			return ch == mArrayBrackets.second;
		}

		bool IsDigit(Char ch) const
		{
			return mDigits.first <= ch && ch <=mDigits.second;
		}

		bool IsLetter(Char ch) const
		{
			return (mUppercaseLetters.first <= ch && ch <= mUppercaseLetters.second)
					|| (mLowercaseLetters.first <= ch && ch<=mLowercaseLetters.second);
		}
		bool IsSeparator(Char ch) const
		{
			return ch == mSeparator;
		}

		bool IsTypeDecl(const std::basic_string<Char>& typeName) const
		{
			return typeName == mTypeName;
		}
		bool IsTypeDecl(EType typeID) const
		{
			return typeID == EType::TYPE;
		}
	private:
		std::basic_string<Char> mTypeName;
		std::set<TypeInfo<Source, Char>> mTypes;
		uint32_t mCustomTypesCount{};
		std::vector<Char> mIgnoreChars;
		std::vector<Char> mSpecCharacters;
		std::pair<Char, Char> mObjDataBrackets;
		std::pair<Char, Char> mStringBrackets;
		std::pair<Char, Char> mCharBrackets;
		std::pair<Char, Char> mArrayBrackets;
		std::pair<Char, Char> mDigits;
		std::pair<Char, Char> mUppercaseLetters;
		std::pair<Char, Char> mLowercaseLetters;
		Char mSeparator;
		Char mEscapeSequenceStart;
	};

	template<typename Source>
	ParserConfiguration<Source, char> GetDefaultParserConfig()
	{
		TypeInfo<Source, char> int32Info;
		int32Info.name = "int32";
		int32Info.type = EType::INT;
		int32Info.reader = &ReadNumericObject<int32_t, Source, char>;

		TypeInfo<Source, char> charInfo;
			charInfo.name = "char";
			charInfo.type = EType::CHAR;
			charInfo.reader =&ReadCharObject<Source, char>;

		TypeInfo<Source, char> stringInfo;
			stringInfo.name = "string";
			stringInfo.type = EType::STRING;
			stringInfo.reader = &ReadStringObject<Source, char>;

		std::vector<TypeInfo<Source, char>> typesInfo{
						int32Info,
						charInfo,
						stringInfo};
		ParserConfiguration<Source, char> config(
				"Type",
				{typesInfo},
				{' ', '\n', '\t'},
				{'\\', '\n', '\t','\'','\"'},
				{'{','}'},
				{'\"','\"'},
				{'\'', '\''},
				{'[', ']'},
				{'0', '9'},
				{'A', 'Z'},
				{'a', 'z'},
				',',
				'\\');
		return config;
	}
}
