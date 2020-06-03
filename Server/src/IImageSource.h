#pragma once
#include "ImageBuffer.h"

namespace ServerSide
{
	template<typename T>
	class IImageSource
	{
	public:
		virtual ~IImageSource() = default;
		virtual ImageBuffer<T> GetImage() = 0;
	};
}
