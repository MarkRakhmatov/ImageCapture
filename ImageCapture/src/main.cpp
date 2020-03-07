#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "VideoDevice/VideoDevice.h"
#include <sstream>
#include "ImageProcessing/JpegHelper.h"
#include "ImageProcessing/ImageProcessor.h"

int main()
{
  std::string deviceName("/dev/video0");
  std::string fileName("Images/webcam_output_");

  VideoDevice device(deviceName);
  device.HandleParameters();
  for(int i = 0; i < 100; ++i)
  {
      auto decomprBuffer = GetImageBufferFromDevice(device);
      ImageProcessor::ToGrayScale(decomprBuffer);
      auto compressedBuffer = JpegHelper::Compress(decomprBuffer);
      std::ostringstream ss;
      ss << fileName << i << ".jpeg";
      JpegHelper::WriteBufferToFile(compressedBuffer, ss.str());
  }
}
