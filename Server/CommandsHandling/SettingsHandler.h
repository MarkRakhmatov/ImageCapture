#pragma once
#include "Socket.h"
#include "ImageProcessing/ImageProcessor.h"
#include "ObjectDescriptor.h"

#include <memory>
#include <vector>

namespace ServerSide
{
	using Settings = std::vector<Parser::ObjectDescriptor<char>>;

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

	bool GetSettings(Communication::Socket& sock);
}
