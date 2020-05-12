#pragma once
#include "Socket.h"
#include "CodeGeneration.h"
#include <string>

namespace Communication
{
	bool SendString(Socket& socket, const std::string& str)
	{
		bool res = false;
		uint64_t size = str.size();
		res = socket.SendData(&size);
		RET_ON_FALSE(res, false);

		for(char ch : str)
		{
			res = socket.SendData(&ch);
			RET_ON_FALSE(res, false);
		}
		return true;
	}

	bool ReadString(Socket& socket, std::string& str)
	{
		bool res = false;

		uint64_t size = 0;
		res = socket.ReadData(&size);
		RET_ON_FALSE(res, false);

		str.resize(static_cast<size_t>(size), '\0');
		char ch;
		for(uint64_t i = 0; i < size; ++i)
		{
			res = socket.ReadData(&ch);
			RET_ON_FALSE(res, false);
			str[i] = ch;
		}
		return true;
	}

	template<typename TContainer>
	bool SendContainer(Socket& socket, const TContainer& container)
	{
		static_assert(std::is_pod<typename TContainer::value_type>::value == true,
				"Argument must be POD type!");
		bool res = false;
		uint64_t size = container.size();
		res = socket.SendData(&size);
		RET_ON_FALSE(res, false);

		for(auto element : container)
		{
			res = socket.SendData(&element);
			RET_ON_FALSE(res, false);
		}
		return true;
	}

	template<typename TContainer>
	bool ReadContainer(Socket& socket, TContainer& container)
	{
		bool res = false;

		uint64_t size = 0;
		res = socket.ReadData(&size);
		RET_ON_FALSE(res, false);
		container.resize(static_cast<size_t>(size));
		char ch;
		for(uint64_t i = 0; i < size; ++i)
		{
			res = socket.ReadData(&ch);
			RET_ON_FALSE(res, false);
			container[i] = ch;
		}
		return true;
	}
}
