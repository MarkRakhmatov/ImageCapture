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
    for(size_t l = 0; l < kernelSize; ++l)
    {
	for(size_t m = 0; m < kernelSize; ++m)
	{
	    auto origPix = buffer.GetElement(i+l, j+m);
	    auto kerK = mKernel[l][m];
	    resultPix += origPix[0]*kerK;
	}
    }

    auto pix = resultBuffer.GetElement(
	i + (kernelSize - 1)/2,
	j + (kernelSize - 1)/2);

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
