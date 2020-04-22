#pragma once
#include <vector>

namespace Parser
{
	template<typename Token=char>
	using Tokens = std::vector<Token>;


	template<typename Token=char>
	using Brackets = std::pair<Token, Token>;

	enum class EStatus
	{
		SUCCESS,
		FAIL,
		FILE_END
	};
}
