#pragma once
#include "ImageBuffer.h"
#include <limits>


using KernelType = float;
using ConvKernel = std::vector<std::vector<KernelType>>;

template <typename T, typename T1>
T Normalize(T1 num)
{
  std::numeric_limits<T> lim;
  if(num > lim.max())
  {
      return lim.max();
  }
  if(num < lim.min())
  {
      return lim.min();
  }
  return num;
}

template<typename T>
T InBounds(T val, T lowwerBound, T upperBound)
{
  if(val<lowwerBound)
  {
      return lowwerBound;
  }
  if(val>upperBound)
  {
      return upperBound;
  }
  return val;
}

template <typename T>
struct ConvHandler
{
  ConvHandler(ConvKernel kernels)
  : mKernel(std::move(kernels))
  {}

  void operator()(ImageBuffer<T>& buffer,
      ImageBuffer<T>& resultBuffer, size_t& i, size_t& j)
  {
    int32_t resultPix{};
    auto kernelSize = mKernel.size();
    if(j > buffer.GetWidth() - kernelSize
	|| i > buffer.GetHeight() - kernelSize)
    {
	return;
    }
    int32_t iStart = InBounds<int32_t>(i - (kernelSize - 1)/2, 0, buffer.GetHeight());
    int32_t jStart =  InBounds<int32_t>(j - (kernelSize - 1)/2, 0, buffer.GetWidth());
    size_t heightBound = InBounds<size_t>(kernelSize, 0, buffer.GetHeight() - iStart);
    size_t widthBound = InBounds<size_t>(kernelSize, 0, buffer.GetWidth() - jStart);

    for(size_t l = 0; l < heightBound; ++l)
    {
	for(size_t m = 0; m < widthBound; ++m)
	{
	    auto origPix = buffer.GetElement(iStart+l, jStart+m);
	    auto kerK = mKernel[l][m];
	    resultPix += origPix[0]*kerK;
	}
    }

    auto pix = resultBuffer.GetElement(i,j);

    pix[0] = Normalize<T>(resultPix);
  }
private:
  ConvKernel mKernel;
};

class ImageProcessor
{
public:
  template <typename T, typename ImageProcFunc>
  static void Convolution(ImageBuffer<T>& buffer, ImageBuffer<T>& resultBuffer, ImageProcFunc imgProcessor)
  {
    auto width = buffer.GetWidth();
    auto height = buffer.GetHeight();
    for(size_t i = 0; i < height; ++i)
    {
	for(size_t j = 0; j < width; ++j)
	{
	    imgProcessor(buffer, resultBuffer, i, j);
	}
    }
  }
};
