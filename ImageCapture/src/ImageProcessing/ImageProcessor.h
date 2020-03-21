#pragma once
#include "ImageBuffer.h"


template <size_t KernelSize = 3,
	  typename T = int32_t>
using ConvKernel = std::array<std::array<T, KernelSize>, KernelSize>;

template <size_t Count,
	size_t KernelSize = 3,
	typename T = int32_t>
using ConvKernels = std::array<ConvKernel<KernelSize, T>, Count>;


template <typename T,
	  size_t PixelSize,
	  typename KernelType,
	  size_t KernelSize>
struct ConvHandler
{
  ConvHandler(ConvKernel<KernelSize, KernelType> kernels)
  : mKernel(std::move(kernels))
  {}

  void operator()(ImageBuffer<T, PixelSize>& buffer,
      ImageBuffer<T, PixelSize>& resultBuffer, size_t& i, size_t& j)
  {
    int32_t resultPix{};
    for(size_t l = 0; l < KernelSize; ++l)
    {
	for(size_t m = 0; m < KernelSize; ++m)
	{
	    auto origPix = buffer.GetElement(i+l, j+m);
	    auto kerK = mKernel[l][m];
	    resultPix += origPix[0]*kerK;
	}
    }

    auto pix = resultBuffer.GetElement(
	i + (KernelSize - 1)/2,
	j + (KernelSize - 1)/2);

    pix[0] = resultPix;
  }
private:
  ConvKernel<KernelSize, KernelType> mKernel;
};

class ImageProcessor
{
public:
  template <typename T, size_t PixelSize = 1, typename ImageProcFunc>
  static ImageBuffer<T, PixelSize> Convolution(ImageBuffer<T, PixelSize>& buffer, ImageProcFunc imgProcessor)
  {
    auto width = buffer.GetWidth();
    auto height = buffer.GetHeight();
    ImageBuffer<T, PixelSize> resultBuffer(width, height);
    for(size_t i = 0; i < height; ++i)
    {
	for(size_t j = 0; j < width; ++j)
	{
	    imgProcessor(buffer, resultBuffer, i, j);
	}
    }
    return resultBuffer;
  }
};
