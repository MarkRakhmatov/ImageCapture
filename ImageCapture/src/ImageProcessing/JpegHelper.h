#pragma once
#include "ImageBuffer.h"
#include "ResourceHolder.h"

#include <turbojpeg.h>
#include <iostream>

using JpegHolder = ResourceHolder<tjhandle, decltype(tjDestroy), tjDestroy>;

class JpegHelper
{
public:
  template<size_t pixSize = 1>
  static ImageBuffer<unsigned char, pixSize>
  Decompress(unsigned char* jpegBuff, int jpegSize,
  int pixelFormat = TJPF_GRAY)
  {
    JpegHolder jHandle{tjInitDecompress()};

    int width = 0;
    int height = 0;
    int jpegSubsamp = 0;
    int jpegColorspace = 0;
    int flags = TJFLAG_STOPONWARNING;
    int res = -1;
    res = tjDecompressHeader3(jHandle.Get(), jpegBuff, jpegSize, &width, &height, &jpegSubsamp, &jpegColorspace);
    if( res == -1)
    {
        return ImageBuffer<unsigned char, pixSize>{};
        std::cout << "Failed to decompress JPEG: "<< tjGetErrorStr2(jHandle.Get());
    }
    int pitch = pixSize * width;
    ImageBuffer<unsigned char, pixSize> buffer(width, height);
    res = tjDecompress2(jHandle.Get(), jpegBuff, jpegSize, buffer.Get(), width, pitch, height, pixelFormat, flags);
    return buffer;
  }
  template<size_t pixSize = 1>
  static std::vector<unsigned char>
  Compress(const ImageBuffer<unsigned char, pixSize>& buffer,
	   int pixelFormat = TJPF_GRAY,
	   int jpegQual = 75)
  {
    JpegHolder jHandle{tjInitCompress()};
    int jpegSubsamp = TJSAMP_GRAY;
    int flags = 1;
    int pitch = tjPixelSize[pixelFormat] * buffer.GetWidth();
    std::vector<unsigned char> compressedImage(buffer.GetWidth()*buffer.GetHeight(), '\0');
    unsigned long jpegCompSize = compressedImage.size();
    unsigned char* jpegBuf = compressedImage.data();
    int tj_stat = tjCompress2(jHandle.Get(), buffer.Get(), buffer.GetWidth(), pitch, buffer.GetHeight(),
  			    pixelFormat, &jpegBuf, &jpegCompSize, jpegSubsamp, jpegQual, flags);
    if (tj_stat != 0)
    {
	std::cout << "Failed to compress JPEG: "<< tjGetErrorStr2(jHandle.Get()) << std::endl;
        return {};
    }
    compressedImage.resize(jpegCompSize);
    return compressedImage;
  }

  static int WriteBufferToFile(const std::vector<unsigned char>& buf, const std::string& fileName);
private:
};

