#include "IImageSource.h"
#include "VideoDevice/VideoDevice.h"

namespace ServerSide
{

	class DeviceImageSource: public IImageSource<unsigned char>
	{
	public:
		DeviceImageSource(const std::string& deviceName, int width, int height);
		ImageBuffer<unsigned char> GetImage() override;
	private:
		VideoDevice mDevice;
	};

}
