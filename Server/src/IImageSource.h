#pragma once
#include <vector>

namespace ServerSide
{
	template<typename T>
	class IImageSource
	{
	public:
		virtual ~IImageSource() = default;
		virtual std::vector<T> GetImage() = 0;
	};
}
