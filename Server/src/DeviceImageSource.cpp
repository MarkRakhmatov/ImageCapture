#include "DeviceImageSource.h"

namespace ServerSide
{

DeviceImageSource::DeviceImageSource(const std::string &deviceName, int width, int height)
	: mDevice(deviceName, width, height)
{
}

	std::vector<unsigned char> DeviceImageSource::GetImage()
	{
		return GetImageBufferFromDevice(mDevice);
	}
}

