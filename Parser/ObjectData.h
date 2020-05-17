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
			size_t origSize = mData.size();
			mData.resize( origSize + sizeof(T));

			if(!std::memcpy(mData.data() + origSize, &newData, sizeof(T)))
			{
				return EStatus::FAIL;
			}
			return EStatus::SUCCESS;
		}

		template<typename T>
		T* GetData(size_t offset = 0)
		{
			if(offset*sizeof(T) > mData.size()-1)
			{
				return nullptr;
			}
			return reinterpret_cast<T*>(mData.data() + offset*sizeof(T));
		}

		template<typename T>
		const T* GetData(size_t offset = 0) const
		{
			if(offset*sizeof(T) > mData.size()-1)
			{
				return nullptr;
			}
			return reinterpret_cast<const T*>(mData.data() + offset*sizeof(T));
		}
		const std::vector<Byte>& GetRawData() const
		{
			return mData;
		}

		std::vector<Byte>& GetRawData()
		{
			return mData;
		}

		std::vector<ObjectData>& GetSubData()
		{
			return mSubData;
		}

		const std::vector<ObjectData>& GetSubData() const
		{
			return mSubData;
		}

		ObjectData* At(uint32_t index)
		{
			return &mSubData[index];
		}
	private:
		std::vector<Byte> mData;
		std::vector<ObjectData> mSubData;
	};
}
