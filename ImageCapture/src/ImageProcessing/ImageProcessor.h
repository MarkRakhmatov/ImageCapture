#pragma once
#include "ImageBuffer.h"

class ImageProcessor
{
public:
  template <typename T, size_t PixelSize>
  static void ToGrayScale(ImageBuffer<T, PixelSize>& buffer)
  {
    auto width = buffer.GetWidth();
    auto height = buffer.GetHeight();
    for(size_t i = 0; i <height; ++i)
    {
	for(size_t j = 0; j < width; ++j)
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
    }
  }
};
