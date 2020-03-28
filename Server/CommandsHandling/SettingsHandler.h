#pragma once
#include "Socket.h"
#include "ImageProcessing/ImageProcessor.h"

#include <memory>
#include <vector>

using Settings = std::vector<ConvKernel>;

class SettingsHandler
{
public:
	static SettingsHandler& Get()
	{
		static SettingsHandler handler{};
		return handler;
	}
	Settings& GetSettings();
private:
	SettingsHandler() = default;
private:
	Settings mSettings;
};

bool GetSettings(Socket& sock);
