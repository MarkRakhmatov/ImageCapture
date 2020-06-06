#include "SettingsHandler.h"
#include "Accessors.h"
#include <sstream>
#include <string>

namespace ServerSide
{
	using namespace Communication;
	using namespace Parser;

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

	bool SettingsHandler::ReadSettings()
	{
		RET_ON_FALSE(ReadKernels(), false);
		{
		const std::string imgSourceTypeStr{"imageSrcType"};
		Parser::ObjectDescriptor<char>* imgSourceType = GetObjectByName(imgSourceTypeStr);
		RET_ON_FALSE(imgSourceType, false);
		mImageSourceType = Parser::AsString(imgSourceType->objectData);
		}

		{
		const std::string imgSourceNameStr{"imageSrcName"};
		Parser::ObjectDescriptor<char>* imgSourceName = GetObjectByName(imgSourceNameStr);
		RET_ON_FALSE(imgSourceName, false);
		mImageSourceName = Parser::AsString(imgSourceName->objectData);
		}

		{
		const std::string preanalysisWindowSizeStr{"preanalysisWindowSize"};
		Parser::ObjectDescriptor<char>* preanalysisWindowSizeObj = GetObjectByName(preanalysisWindowSizeStr);
		RET_ON_FALSE(preanalysisWindowSizeObj, false);
		mPreanalysisWindowSize = Parser::AsInt<int32_t>(preanalysisWindowSizeObj->objectData);
		}

		{
		const std::string pointDetectionWindfowSizeStr{"pointDetectionWindfowSize"};
		Parser::ObjectDescriptor<char>* pointDetectionWindfowSize = GetObjectByName(pointDetectionWindfowSizeStr);
		RET_ON_FALSE(pointDetectionWindfowSize, false);
		mPointDetectionWindfowSize = Parser::AsInt<int32_t>(pointDetectionWindfowSize->objectData);
		}

		{
		const std::string detectionThresholdStr{"detectionThreshold"};
		Parser::ObjectDescriptor<char>* detectionThreshold = GetObjectByName(detectionThresholdStr);
		RET_ON_FALSE(detectionThreshold, false);
		mDetectionThreshold = Parser::AsInt<int32_t>(detectionThreshold->objectData);
		}

		{
		const std::string brightnessLowerBoundStr{"brightnessLowerBound"};
		Parser::ObjectDescriptor<char>* brightnessLowerBound = GetObjectByName(brightnessLowerBoundStr);
		RET_ON_FALSE(brightnessLowerBound, false);
		mBrightnessLowerBound = Parser::AsInt<int32_t>(brightnessLowerBound->objectData);
		}

		{
		const std::string preprocessingAlgoNameStr{"preprocessingAlgo"};
		Parser::ObjectDescriptor<char>* preprocessingAlgoName = GetObjectByName(preprocessingAlgoNameStr);
		RET_ON_FALSE(preprocessingAlgoName, false);
		mPreprocessingAlgoName = Parser::AsString(preprocessingAlgoName->objectData);
		}

		return true;
	}

	int32_t SettingsHandler::GetPreanalysisWindowSize()
	{
		return mPreanalysisWindowSize;
	}

	int32_t SettingsHandler::GetPointDetectionWindfowSize()
	{
		return mPointDetectionWindfowSize;
	}
	int32_t SettingsHandler::GetDetectionThreshold()
	{
		return mDetectionThreshold;
	}

	int32_t SettingsHandler::GetBrightnessLowerBound()
	{
		return mBrightnessLowerBound;
	}

	std::vector<Kernel>& SettingsHandler::GetSobelKernels()
	{
		return mSobelKernels;
	}

	std::vector<Kernel>& SettingsHandler::GetPrewittKernels()
	{
		return mPrewittKernels;
	}

	const std::string& SettingsHandler::GetPreprocessingAlgoName()
	{
		return mPreprocessingAlgoName;
	}

	bool SettingsHandler::ReadKernelsByName(const std::string& objName, std::vector<Kernel>& kernels)
	{
		auto* object = GetObjectByName(objName);
		RET_ON_FALSE(object, false);
		RET_ON_FALSE(object->type == EType::STRING, false);
		RET_ON_FALSE(object->arrayDepth == 1, false);

		auto& subData = object->objectData.GetSubData();
		uint32_t size = static_cast<uint32_t>(subData.size());
		RET_ON_FALSE(size || size%2 != 0, false);
		kernels.resize(size/2);

		uint32_t objectsCounter = 0;
		uint32_t kernelSize = 0;
		std::vector<KernelUnit> kernelData;

		bool res{false};
		for(uint32_t i = 0; i < subData.size(); ++i, ++objectsCounter)
		{
			res = ReadKernelSize(subData[i], kernelSize);
			RET_ON_FALSE(res, false);
			kernels[objectsCounter].SetSize(kernelSize);
			res = ReadKernelUnits(subData[++i], kernels[objectsCounter]);
			RET_ON_FALSE(res, false);
		}
		return true;
	}

	bool SettingsHandler::ReadKernels()
	{
		bool res = false;
		{
		const std::string customKernelsName{"customKernels"};
		res = ReadKernelsByName(customKernelsName, mCustomKernels);
		RET_ON_FALSE(res, false);
		}
		{
		const std::string sobelKernelsName{"sobelKernels"};
		res = ReadKernelsByName(sobelKernelsName, mSobelKernels);
		RET_ON_FALSE(res, false);
		}
		{
		const std::string prewittKernelsName{"prewittKernels"};
		res = ReadKernelsByName(prewittKernelsName, mPrewittKernels);
		RET_ON_FALSE(res, false);
		}
		return true;
	}

	std::vector<Kernel>& SettingsHandler::GetCustomKernels()
	{
		return mCustomKernels;
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

	const std::string& SettingsHandler::GetImageSourceName()
	{
		return mImageSourceName;
	}

	const std::string& SettingsHandler::GetImageSourceType()
	{
		return mImageSourceType;
	}

	Objects& SettingsHandler::GetSettings()
	{
		return mSettingsObjects;
	}

	SettingsHandler::SettingsHandler()
	: mImageSourceType("Device")
	, mImageSourceName("/dev/video0")
	, mPreanalysisWindowSize(30)
	, mPointDetectionWindfowSize(15)
	, mDetectionThreshold(120)
	, mBrightnessLowerBound(50)
	{}

}
