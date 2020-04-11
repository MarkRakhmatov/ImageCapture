#pragma once
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
		bool PeekToken(Token& token)
		{
			token = mFile.peek();
			if(!mFile || mFile.eof())
			{
				return false;
			}
			token = static_cast<Token>(token);
			return true;
		}
		bool GetToken(Token& token)
		{
			token = mFile.get();
			if(!mFile || mFile.eof())
			{
				return false;
			}
			token = static_cast<Token>(token);
			return true;
		}
	private:
		std::basic_ifstream<Token> mFile;
	};

	using FileSourceA = FileSource<char>;
}
