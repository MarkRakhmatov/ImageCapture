#pragma once
#include "ImageBuffer.h"
#include "Kernel.h"
#include <stdint.h>
#include <vector>
#include <limits>
#include <iostream>
#include <cmath>


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

template <typename T>
struct ConvOperatorHandler
{
  ConvOperatorHandler(Kernel& kernel1, Kernel& kernel2)
  : mKernel1(kernel1)
  , mKernel2(kernel2)
  , mKernelUnitsSize(mKernel1.Get().size())
  , mKernelSize(mKernel1.Size())
  , mKernelHalfSize((mKernelSize - 1)/2)
  {
  }

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
    int32_t resultPix1{};
    int32_t resultPix2{};
    const std::vector<KernelUnit>& kernelUnits1= mKernel1.Get();
    const std::vector<KernelUnit>& kernelUnits2= mKernel2.Get();
    for(uint32_t counter = 0; counter < mKernelUnitsSize; ++counter)
    {
	resultPix1 +=
		*buffer.GetElement(kernelUnits1[counter].i + kernelMappingOffsetI,
				   kernelUnits1[counter].j + kernelMappingOffsetJ) * kernelUnits1[counter].coeff;
	resultPix2 +=
		*buffer.GetElement(kernelUnits2[counter].i + kernelMappingOffsetI,
				   kernelUnits2[counter].j + kernelMappingOffsetJ) * kernelUnits2[counter].coeff;
    }

    resultBuffer.GetElement(i,j)[0] = Normalize<T>(sqrt(resultPix1*resultPix1 + resultPix2*resultPix2));
  }
private:
  Kernel mKernel1;
  Kernel mKernel2;
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

template <typename T>
static ImageBuffer<T> RgbToGrayscale(ImageBuffer<T>& buffer)
{
  uint32_t height = buffer.GetHeight();
  uint32_t width = buffer.GetWidth();
  ImageBuffer<T> resultBuffer(width, height, EPixelType::GRAY_SCALE);
  for(uint32_t i = 0; i < height; ++ i)
  {
      for(uint32_t j = 0; j < width; ++ j)
      {
	  T* rgb = buffer.GetElement(i, j);
	  float grayScale = rgb[0]*0.299f+rgb[1]*0.587f+rgb[2]*0.11f;
	  *resultBuffer.GetElement(i, j) = grayScale < 255 ? grayScale : 255;
      }
  }
  return resultBuffer;
}
