#pragma once
#include <stdint.h>
#include <vector>
#include <utility>

namespace Parser
{
	using Byte = uint8_t;
	template<typename Char>
	using Chars = std::vector<Char>;


	template<typename Char>
	using Brackets = std::pair<Char, Char>;

	enum class EStatus
	{
		SUCCESS,
		FAIL,
		INVALID_TYPE,
		DATA_END
	};

	enum EType : uint32_t
	{
		INVALID,
		TYPE,
		INT,
		CHAR,
		STRING,

		LAST_RESERVED
	};
}
