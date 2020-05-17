#include "SettingsHandler.h"
#include "Accessors.h"
#include <sstream>
#include <string>

namespace ServerSide
{
	using namespace Communication;
	using namespace Parser;

	bool GetSettings(Socket& sock)
	{

	  return false;
	}

	bool SettingsHandler::ReadKernelUnits(const Parser::ObjectData& obj, Kernel& kernel)
	{
		std::stringstream converter(AsString(obj));
		RET_ON_FALSE(converter, false);
		KernelUnit unit{};
		uint32_t size = kernel.Size();
		for(uint32_t i = 0; i < size; ++i)
		{
			for(uint32_t j = 0; j < size; ++j)
			{
				converter >> unit.coeff;
				RET_ON_FALSE(converter, false);
				if(unit.coeff == 0)
				{
					continue;
				}
				unit.i = i;
				unit.j = j;
				kernel.Get().push_back(unit);
			}
		}
		return true;
	}

	bool SettingsHandler::ReadKernelSize(const Parser::ObjectData& obj, uint32_t& kernelSize )
	{
		std::stringstream converter;
		std::string kernelSizeStr{Parser::AsString(obj)};
		converter << kernelSizeStr;
		RET_ON_FALSE(converter, false);
		converter >> kernelSize;
		RET_ON_FALSE(converter, false);
		return true;
	}

	bool SettingsHandler::ReadKernels()
	{
		std::string kernelsName{"kernels"};
		auto* object = GetObjectByName(kernelsName);
		RET_ON_FALSE(object, false);
		RET_ON_FALSE(object->type == EType::STRING, false);
		RET_ON_FALSE(object->arrayDepth == 1, false);
		auto& subData = object->objectData.GetSubData();
		uint32_t size = static_cast<uint32_t>(subData.size());
		RET_ON_FALSE(size || size%2 != 0, false);
		mKernels.resize(size/2);
		uint32_t objectsCounter = 0;
		uint32_t kernelSize = 0;
		std::vector<KernelUnit> kernelData;
		bool res{false};
		for(uint32_t i = 0; i < subData.size(); ++i, ++objectsCounter)
		{
			res = ReadKernelSize(subData[i], kernelSize);
			RET_ON_FALSE(res, false);
			mKernels[objectsCounter].SetSize(kernelSize);
			res = ReadKernelUnits(subData[++i], mKernels[objectsCounter]);
			RET_ON_FALSE(res, false);
		}
		return true;
	}

	std::vector<Kernel>& SettingsHandler::GetKernels()
	{
		return mKernels;
	}

	Parser::ObjectDescriptor<char>* SettingsHandler::GetObjectByName(
			const std::string &name)
	{
		for(auto& object : mSettingsObjects)
		{
			if(object.name == name)
			{
				return &object;
			}
		}
		return nullptr;
	}


	Objects& SettingsHandler::GetSettings()
	{
		return mSettingsObjects;
	}
}
