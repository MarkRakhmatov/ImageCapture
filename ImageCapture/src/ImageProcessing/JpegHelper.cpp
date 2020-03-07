#include "JpegHelper.h"
#include "ResourceHolder.h"
#include <memory>
#include <fstream>
#include <algorithm>

#include "ImageBuffer.h"

using JpegHolder = ResourceHolder<tjhandle, decltype(tjDestroy), tjDestroy>;

ImageBuffer<unsigned char>
JpegHelper::Decompress (unsigned char* jpegBuff, int jpegSize)
{
  JpegHolder jHandle{tjInitDecompress()};

  int width = 0;
  int height = 0;
  int pixelFormat = TJPF_RGB;
  int jpegSubsamp = 0;
  int jpegColorspace = 0;
  int flags = TJFLAG_STOPONWARNING;
  int res = -1;
  res = tjDecompressHeader3(jHandle.Get(), jpegBuff, jpegSize, &width, &height, &jpegSubsamp, &jpegColorspace);
  if( res == -1)
  {
      return ImageBuffer<unsigned char>{};
  }
  int pitch = tjPixelSize[pixelFormat] * width;
  ImageBuffer<unsigned char> buffer(width, height);
  res = tjDecompress2(jHandle.Get(), jpegBuff, jpegSize, buffer.Get(), width, pitch, height, pixelFormat, flags);
  return buffer;
}

std::vector<unsigned char>
JpegHelper::Compress(const ImageBuffer<unsigned char>& buffer)
{
  JpegHolder jHandle{tjInitCompress()};

  int pixelFormat = TJPF_RGB;
  // 93 due to turbo-jpeg bug with compressing images
  // if quality > 93 image is corrupted for some reason
  int jpegQual = 93;
  int jpegSubsamp = 0;
  int flags = 1;
  int pitch = tjPixelSize[pixelFormat] * buffer.GetWidth();
  std::vector<unsigned char> compressedImage(pitch*buffer.GetHeight()/15, '\0');
  unsigned long jpegCompSize = compressedImage.size();
  unsigned char* jpegBuf = compressedImage.data();
  int tj_stat = tjCompress2(jHandle.Get(), buffer.Get(), buffer.GetWidth(), pitch, buffer.GetHeight(),
			    pixelFormat, &jpegBuf, &jpegCompSize, jpegSubsamp, jpegQual, flags);
  if (tj_stat != 0)
  {
      return {};
  }
  compressedImage.resize(jpegCompSize);
  return compressedImage;
}

int
JpegHelper::WriteBufferToFile (const std::vector<unsigned char>& buf,
			       const std::string& fileName)
{
  remove(fileName.c_str());
  if(!buf.size())
  {
      return -1;
  }
  std::ofstream outFile(fileName.c_str(), std::ios::binary | std::ios::app);
  outFile.write(reinterpret_cast<const char*>(buf.data()), buf.size());
  return 0;
}
