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
		std::vector<Kernel>& GetKernels();
		const std::string& GetImageSourceName();
		const std::string& GetImageSourceType();
	private:
		SettingsHandler();
		bool ReadImageSourceInfo();
		bool ReadKernelUnits(const Parser::ObjectData& obj, Kernel& kernel);
		bool ReadKernelSize(const Parser::ObjectData& obj, uint32_t& kernelSize);
		bool ReadKernels();
	private:
		Objects mSettingsObjects;
		std::string mImageSourceType;
		std::string mImageSourceName;
		std::vector<Kernel> mKernels;
	};
}
