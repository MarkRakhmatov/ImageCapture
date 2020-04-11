#pragma once
#include <vector>
#include <string>


namespace Parser
{
	template<typename Token = char>
	struct ObjectDescriptor
	{
		std::basic_string<Token> typeName;
		std::basic_string<Token> objName;
		std::vector<Token> objectData;
		std::vector<ObjectDescriptor> subObjects;
	};
}
