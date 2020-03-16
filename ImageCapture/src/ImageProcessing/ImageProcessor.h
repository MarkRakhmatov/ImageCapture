#pragma once
#include "ImageBuffer.h"

template <typename T, size_t PixelSize = 3>
using BlockProcessorFunc = void (*) (ImageBuffer<T, PixelSize>& buffer, size_t& i, size_t& j);

class ImageProcessor
{
public:

  template <typename T, size_t PixelSize = 3>
  static void GrayScale(ImageBuffer<T, PixelSize>& buffer, size_t& i, size_t& j)
  {
    auto pixel = buffer.GetElement(i, j);
    auto& red = pixel[0];
    auto& green = pixel[1];
    auto& blue = pixel[2];
    auto avarage = (red + green + blue)/3;
    red = avarage;
    blue = avarage;
    green = avarage;
  }

  template <typename T, size_t PixelSize = 3>
  static void TestFilter(ImageBuffer<T, PixelSize>& buffer, size_t& i, size_t& j)
  {
    const size_t blockSize = 10;
    size_t accumulatorR = 0;
    size_t accumulatorG = 0;
    size_t accumulatorB = 0;
    for(size_t k = i; (k < (i + blockSize)) && k < buffer.GetHeight(); ++k)
    {
	for(size_t l = j; (l < (j + blockSize)) && l < buffer.GetWidth(); ++l)
	{
	    auto pixel = buffer.GetElement(k, l);
	    auto& red = pixel[0];
	    auto& green = pixel[1];
	    auto& blue = pixel[2];
	    accumulatorR += red;
	    accumulatorG += green;
	    accumulatorB += blue;
	}
    }

    for(size_t k = i; (k < (i + blockSize)) && k < buffer.GetHeight(); ++k)
    {
	for(size_t l = j; (l < (j + blockSize)) && l < buffer.GetWidth(); ++l)
	{
	    auto pixel = buffer.GetElement(k, l);
	    auto& red = pixel[0];
	    auto& green = pixel[1];
	    auto& blue = pixel[2];
	    red = accumulatorR / (blockSize*blockSize);
	    green = accumulatorG / (blockSize*blockSize);
	    blue = accumulatorB / (blockSize*blockSize);
	}
    }
    auto step = blockSize-blockSize/3;
    if(j > buffer.GetWidth() - blockSize - 1)
    {
	i += step;
    }
    j+= step;
  }

  template <typename T, size_t PixelSize = 3>
  static void ProcessByBlocks(ImageBuffer<T, PixelSize>& buffer, BlockProcessorFunc<T, PixelSize> func)
  {
    auto width = buffer.GetWidth();
    auto height = buffer.GetHeight();
    for(size_t i = 0; i < height; ++i)
    {
	for(size_t j = 0; j < width; ++j)
	{
	    func(buffer, i, j);
	}
    }
  }
};
