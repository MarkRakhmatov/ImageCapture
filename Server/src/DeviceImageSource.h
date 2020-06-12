#include "IImageSource.h"
#include "VideoDevice/VideoDevice.h"
#include <string>

namespace ServerSide
{

	class DeviceImageSource: public IImageSource<unsigned char>
	{
	public:
		DeviceImageSource(const std::string& deviceName, int width, int height);
		std::vector<unsigned char> GetImage() override;
	private:
		VideoDevice mDevice;
	};

}
