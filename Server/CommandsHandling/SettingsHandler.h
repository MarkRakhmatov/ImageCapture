#pragma once
#include "Socket.h"
#include "ImageProcessing/ImageProcessor.h"
#include "ObjectDescriptor.h"

#include <memory>
#include <vector>

namespace ServerSide
{
	using Objects = std::vector<Parser::ObjectDescriptor<char>>;

	class SettingsHandler
	{
	public:
		static SettingsHandler& Get()
		{
			static SettingsHandler handler{};
			return handler;
		}
		Parser::ObjectDescriptor<char>* GetObjectByName(const std::string& name);
		Objects& GetSettings();
		bool ReadSettings();
		std::vector<Kernel>& GetCustomKernels();
		std::vector<Kernel>& GetSobelKernels();
		std::vector<Kernel>& GetPrewittKernels();
		const std::string& GetImageSourceName();
		const std::string& GetImageSourceType();
		const std::string& GetPreprocessingAlgoName();
		int32_t GetPreanalysisWindowSize();
		int32_t GetPointDetectionWindfowSize();
		int32_t GetDetectionThreshold();
		int32_t GetBrightnessLowerBound();
	private:
		SettingsHandler();
		bool ReadKernelsByName(const std::string& objName, std::vector<Kernel>& kernels);
		bool ReadKernelUnits(const Parser::ObjectData& obj, Kernel& kernel);
		bool ReadKernelSize(const Parser::ObjectData& obj, uint32_t& kernelSize);
		bool ReadKernels();
	private:
		Objects mSettingsObjects;
		std::string mImageSourceType;
		std::string mImageSourceName;
		std::string mPreprocessingAlgoName;
		int32_t mPreanalysisWindowSize;
		int32_t mPointDetectionWindfowSize;
		int32_t mDetectionThreshold;
		int32_t mBrightnessLowerBound;
		std::vector<Kernel> mCustomKernels;
		std::vector<Kernel> mSobelKernels;
		std::vector<Kernel> mPrewittKernels;
	};
}
