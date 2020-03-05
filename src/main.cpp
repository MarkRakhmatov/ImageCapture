#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "VideoDevice/VideoDevice.h"
#include <sstream>
#include "ImageProcessing/JpegHelper.h"
#include "ImageProcessing/ImageProcessor.h"

ImageBuffer<unsigned char> GetImageBufferFromDevice(std::string deviceName)
{
  VideoDevice device(deviceName);
  MappedBuffer& buf = device.GetRawBuffer();
  auto imgBuff = JpegHelper::Decompress(reinterpret_cast<unsigned char*>(buf.Get()), buf.Size());
  if(imgBuff.Get()<0)
  {
      std::cout << "INVALID FILE FORMAT!!! FAILED TO DECOMPRESS JPEG "<<std::endl;
  }
  return imgBuff;
}

int main()
{
  std::string deviceName("/dev/video0");
  std::string fileName("webcam_output_");
  for(int i = 0; i < 20; ++i)
  {
      auto decomprBuffer = GetImageBufferFromDevice(deviceName);
      ImageProcessor::ToGrayScale(decomprBuffer);
      auto compressedBuffer = JpegHelper::Compress(decomprBuffer);
      std::ostringstream ss;
      ss << fileName << i << ".jpeg";
      JpegHelper::WriteBufferToFile(compressedBuffer, ss.str());
  }
}
