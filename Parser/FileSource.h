#pragma once
#include "CodeGeneration.h"
#include "ParserUtils.h"

#include <fstream>
#include <string>

namespace Parser
{
	template<typename Char>
	class FileSource
	{
	public:
		FileSource(const std::basic_string<Char>& filename)
		: mFile(filename)
		{}
		EStatus PeekChar(Char& ch)
		{
			ch = static_cast<Char>(mFile.peek());

			RET_ON_TRUE(mFile.eof(), EStatus::DATA_END);
			RET_ON_FALSE(mFile, EStatus::FAIL);
			return EStatus::SUCCESS;
		}
		EStatus GetChar(Char& ch)
		{
			ch = static_cast<Char>(mFile.get());
			RET_ON_TRUE(mFile.eof(), EStatus::DATA_END);
			RET_ON_FALSE(mFile, EStatus::FAIL);
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
		std::basic_ifstream<Char> mFile;
	};
}
