#include "ObjectData.h"
#include <string>

namespace Parser
{
	template<typename T>
	T AsInt(const ObjectData& objData)
	{
		return *objData.GetData<T>();
	}

	char AsChar(const ObjectData& objData)
	{
		return *objData.GetData<char>();
	}

	std::string AsString(const ObjectData& objData)
	{
		return objData.GetData<char>();
	}

	char* AsCString(ObjectData& objData)
	{
		return objData.GetData<char>();
	}
}
