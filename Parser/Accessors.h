#include "ObjectData.h"
#include <string>

namespace Parser
{
	template<typename T>
	T AsInt(ObjectData& objData)
	{
		return *objData.GetData<T>();
	}

	char AsChar(ObjectData& objData)
	{
		return *objData.GetData<char>();
	}

	std::string AsString(ObjectData& objData)
	{
		return objData.GetData<char>();
	}

	char* AsCString(ObjectData& objData)
	{
		return objData.GetData<char>();
	}
}
