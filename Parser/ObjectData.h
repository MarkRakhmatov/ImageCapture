#pragma once
#include "ParserTypes.h"
#include <cstring>

namespace Parser
{
	class ObjectData
	{
	public:
		template<typename T>
		EStatus AppendData(T newData)
		{
			size_t origSize = data.size();
			data.resize( origSize + sizeof(T));

			if(!std::memcpy(data.data() + origSize, &newData, sizeof(T)))
			{
				return EStatus::FAIL;
			}
			return EStatus::SUCCESS;
		}

		template<typename T>
		T* GetData(size_t offset = 0)
		{
			if(offset*sizeof(T) > data.size()-1)
			{
				return nullptr;
			}
			return reinterpret_cast<T*>(data.data() + offset*sizeof(T));
		}
		std::vector<ObjectData>& GetSubData()
		{
			return subData;
		}

		ObjectData* At(uint32_t index)
		{
			return &subData[index];
		}
	private:
		std::vector<Byte> data;
		std::vector<ObjectData> subData;
	};
}
