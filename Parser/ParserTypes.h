#pragma once
#include <stdint.h>
#include <vector>

namespace Parser
{
	template<typename Token>
	using Tokens = std::vector<Token>;


	template<typename Token>
	using Brackets = std::pair<Token, Token>;

	enum class EStatus
	{
		SUCCESS,
		FAIL,
		FILE_END
	};

	enum EType : uint32_t
	{
		INT,
		CHAR,
		STRING
	};
}
