#pragma once
#include "CodeGeneration.h"
#include "ParserUtils.h"

#include <fstream>
#include <string>

namespace Parser
{
	template<typename Token=char>
	class FileSource
	{
	public:
		FileSource(const std::basic_string<Token>& filename)
		: mFile(filename)
		{}
		EStatus PeekToken(Token& token)
		{
			token = mFile.peek();

			RET_ON_SUCCESS(mFile.eof(), EStatus::FILE_END);
			RET_ON_FALSE(mFile, EStatus::FAIL);
			token = static_cast<Token>(token);
			return EStatus::SUCCESS;
		}
		EStatus GetToken(Token& token)
		{
			token = mFile.get();
			RET_ON_SUCCESS(mFile.eof(), EStatus::FILE_END);
			RET_ON_FALSE(mFile, EStatus::FAIL);
			token = static_cast<Token>(token);
			return EStatus::SUCCESS;
		}
		template<typename T>
		EStatus Read(T* data)
		{
			RET_ON_FALSE(data, EStatus::FAIL);
			mFile >> *data;
			RET_ON_FALSE(mFile, EStatus::FAIL);
			return EStatus::SUCCESS;
		}
	private:
		std::basic_ifstream<Token> mFile;
	};
}
