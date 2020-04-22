#pragma once
#include "CodeGeneration.h"
#include "ParserTypes.h"

#include <stdint.h>
#include <vector>
#include <string>
#include <cstring>

namespace Parser
{
	using Byte = uint8_t;
	template<typename Token>
	struct ObjectDescriptor
	{
		std::basic_string<Token> typeName;
		std::basic_string<Token> objName;
		std::vector<Byte> objectData;
		std::vector<ObjectDescriptor> subObjects;
	};

	template<typename Token>
	bool operator < (const ObjectDescriptor<Token>& obj, const ObjectDescriptor<Token>& obj1)
	{
		return obj.objName<obj1.objName;
	}

	template<typename T, typename Source, typename Token>
	EStatus ReadObjectData(Source& src, ObjectDescriptor<Token>& objDesc)
	{
		T data;
		auto status = src.Read(&data);
		RET_ON_FAIL(status == EStatus::SUCCESS, status);
		size_t origSize = objDesc.objectData.size();
		objDesc.objectData.resize( origSize + sizeof(T));

		if(!std::memcpy(objDesc.objectData.data() + origSize, &data, sizeof(T)))
		{
			return EStatus::FAIL;
		}
		return EStatus::SUCCESS;
	}
}
