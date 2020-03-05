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
    for(auto i = 0; i <height; ++i)
    {
	for(auto j = 0; j < width; ++j)
	{
	    auto pixel = buffer.GetElement(i, j);
	    auto avarage = (pixel.data[0] + pixel.data[1] + pixel.data[2])/3;
	    pixel.data[0] = avarage;
	    pixel.data[1] = avarage;
	    pixel.data[2] = avarage;
	}
    }
  }
};
