#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include <cstring>

namespace Parser
{
	using Byte = uint8_t;
	template<typename Token = char>
	struct ObjectDescriptor
	{
		std::basic_string<Token> typeName;
		std::basic_string<Token> objName;
		std::vector<Byte> objectData;
		std::vector<ObjectDescriptor> subObjects;
	};

	template<typename Token = char>
	bool operator < (const ObjectDescriptor<Token>& obj, const ObjectDescriptor<Token>& obj1)
	{
		return obj.objName<obj1.objName;
	}
	template<typename T, typename Source, typename Token>
	bool ReadObjectData(Source& src, ObjectDescriptor<Token>& objDesc)
	{
		T data;
		if(!src.Read(&data))
		{
			return false;
		}
		size_t origSize = objDesc.objectData.size();
		objDesc.objectData.resize( origSize + sizeof(T));

		return std::memcpy(objDesc.objectData.data() + origSize, &data, sizeof(T)) != nullptr;
	}
}
