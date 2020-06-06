#include "JpegHelper.h"
#include <memory>
#include <fstream>
#include <algorithm>

#include "ImageBuffer.h"


int
JpegHelper::WriteBufferToFile (const std::vector<unsigned char>& buf,
			       const std::string& fileName)
{
  if(!buf.size())
  {
      return -1;
  }
  std::ofstream outFile(fileName, std::ios::binary | std::ios::app);
  outFile.write(reinterpret_cast<const char*>(buf.data()), buf.size());
  return 0;
}

void
ConvertImgFormat (const ImageBuffer<unsigned char>& buffer, TJSAMP& samp,
		  TJPF& pixFormat)
{
  const std::unordered_map<EPixelType, std::pair<TJPF, TJSAMP>, EnumClassHash<uint16_t>>
	buffFormatToJpeg
	{
	  {EPixelType::GRAY_SCALE, {TJPF::TJPF_GRAY, TJSAMP::TJSAMP_GRAY}},
	  {EPixelType::RGB, {TJPF::TJPF_RGB, TJSAMP::TJSAMP_444}}
	};
  pixFormat = buffFormatToJpeg.at(buffer.GetPixelType()).first;
  samp = buffFormatToJpeg.at(buffer.GetPixelType()).second;
}
