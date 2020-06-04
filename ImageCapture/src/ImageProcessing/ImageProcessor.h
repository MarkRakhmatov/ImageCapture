#pragma once
#include "ImageBuffer.h"
#include "Kernel.h"
#include <stdint.h>
#include <vector>
#include <limits>


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

template<typename T>
bool IsInBounds(T val, T lowwerBound, T upperBound)
{
  return (val<=lowwerBound || val>=upperBound) ? false :true;
}

template <typename T>
struct ConvHandler
{
  ConvHandler(Kernel& kernel)
  : mKernel(kernel)
  , mKernelUnitsSize(mKernel.Get().size())
  , mKernelSize(mKernel.Size())
  , mKernelHalfSize((mKernelSize - 1)/2)
  {}

  void operator()(ImageBuffer<T>& buffer,
      ImageBuffer<T>& resultBuffer, uint32_t i, uint32_t j)
  {
    int32_t kernelMappingOffsetI = i - mKernelHalfSize;
    int32_t kernelMappingOffsetJ = j - mKernelHalfSize;
    if(!IsInBounds<int32_t>(kernelMappingOffsetI, 0, buffer.GetHeight() - mKernelSize)
	|| !IsInBounds<int32_t>(kernelMappingOffsetJ, 0, buffer.GetWidth() - mKernelSize))
    {
	return;
    }
    int32_t resultPix{};
    const std::vector<KernelUnit>& kernelUnits= mKernel.Get();
    for(uint32_t counter = 0; counter < mKernelUnitsSize; ++counter)
    {
	resultPix +=
		*buffer.GetElement(kernelUnits[counter].i + kernelMappingOffsetI,
				   kernelUnits[counter].j + kernelMappingOffsetJ) * kernelUnits[counter].coeff;
    }

    resultBuffer.GetElement(i,j)[0] = Normalize<T>(resultPix);
  }
private:
  Kernel mKernel;
  uint32_t mKernelUnitsSize{};
  uint32_t mKernelSize{};
  uint32_t mKernelHalfSize{};
};

class ImageProcessor
{
public:
  template <typename T, typename ImageProcFunc>
  static void Convolution(ImageBuffer<T>& buffer, ImageBuffer<T>& resultBuffer, ImageProcFunc& imgProcessor)
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
