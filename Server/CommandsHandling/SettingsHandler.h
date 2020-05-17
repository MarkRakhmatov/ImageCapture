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
		bool ReadKernelUnits(const Parser::ObjectData& obj, Kernel& kernel);
		bool ReadKernelSize(const Parser::ObjectData& obj, uint32_t& kernelSize);
		bool ReadKernels();
		std::vector<Kernel>& GetKernels();
		Objects& GetSettings();
	private:
		SettingsHandler() = default;
	private:
		Objects mSettingsObjects;
		std::string imageSource;
		std::string imageSourceName;
		std::vector<Kernel> mKernels;
	};

	bool GetSettings(Communication::Socket& sock);
}
