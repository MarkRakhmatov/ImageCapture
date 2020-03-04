#pragma once
#include <turbojpeg.h>
#include "ImageBuffer.h"

class JpegHelper
{
public:
  static ImageBuffer<unsigned char> Decompress(unsigned char* jpegBuff, int jpegSize);
  static std::vector<unsigned char> Compress(const ImageBuffer<unsigned char>& buffer);

  static int WriteBufferToFile(const std::vector<unsigned char>& buf, const std::string& fileName);
private:
};

