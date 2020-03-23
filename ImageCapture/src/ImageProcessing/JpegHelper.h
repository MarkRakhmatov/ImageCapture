#pragma once
#include "ImageBuffer.h"
#include "ResourceHolder.h"

#include <turbojpeg.h>
#include <iostream>

using JpegHolder = ResourceHolder<tjhandle, decltype(tjDestroy), tjDestroy>;

void ConvertImgFormat(const ImageBuffer<unsigned char>& buffer, TJSAMP& samp, TJPF& pixFormat);


class JpegHelper
{
public:
  static ImageBuffer<unsigned char>
  Decompress(unsigned char* jpegBuff, int jpegSize,
	     TJPF pixelFormat = TJPF_GRAY)
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
        return {};
        std::cout << "Failed to decompress JPEG: "<< tjGetErrorStr2(jHandle.Get());
    }
    auto pixSize = tjPixelSize[pixelFormat];
    int pitch = pixSize * width;
    ImageBuffer<unsigned char> buffer(width, height, EPixelType::GRAY_SCALE);
    res = tjDecompress2(jHandle.Get(), jpegBuff, jpegSize, buffer.Get(), width, pitch, height, pixelFormat, flags);
    return buffer;
  }

  static std::vector<unsigned char>
  Compress(const ImageBuffer<unsigned char>& buffer,
	   int jpegQual = 75)
  {
    JpegHolder jHandle{tjInitCompress()};
    TJSAMP jpegSubsamp{};
    TJPF pixelFormat{};
    ConvertImgFormat(buffer, jpegSubsamp, pixelFormat);
    int flags = TJFLAG_STOPONWARNING;
    int pitch = buffer.GetPixelSize() * buffer.GetWidth();
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

