#pragma once
#include <memory>
#include <vector>
#include <array>
#include <utility>

template<typename T, size_t PixelSize = 3>
struct Pixel
{
  Pixel(T* dataPtr):data(dataPtr)
  {}
  T* data;
};

template<typename T, size_t PixelSize = 3>
class ImageBuffer
{
public:
  ImageBuffer() = default;
  ~ImageBuffer() = default;
  ImageBuffer(ImageBuffer&) = delete;
  ImageBuffer& operator=(ImageBuffer&) = delete;
  ImageBuffer(ImageBuffer&& otherBuffer)
  : mWidth(otherBuffer.mWidth)
  , mHeight(otherBuffer.mHeight)
  , mBuffer(std::move(otherBuffer.mBuffer))
  {
    otherBuffer.mWidth = 0;
    otherBuffer.mHeight = 0;
  }
  ImageBuffer& operator=(ImageBuffer&& otherBuffer)
  {
    std::swap(mWidth, otherBuffer.mWidth);
    std::swap(mHeight, otherBuffer.mHeight);
    mBuffer = std::move(otherBuffer.mBuffer);
    return *this;
  }
  ImageBuffer(int width, int height)
  : mWidth(width)
  , mHeight(height)
  , mBuffer(mHeight*mWidth*PixelSize, T{})
  {
  }
  size_t GetWidth() const
  {
    return mWidth;
  }
  size_t GetHeight() const
  {
    return mHeight;
  }
  constexpr size_t GetPixelSize() const
  {
    return PixelSize;
  }
  Pixel<T, PixelSize> GetElement(size_t i, size_t j)
  {
    auto index = (i*mWidth + j)*PixelSize;
    return Pixel<T, PixelSize>(&mBuffer[index]);
  }
  T* Get()
  {
    return mBuffer.data();
  }

  const T* Get() const
  {
    return mBuffer.data();
  }
private:
  size_t mWidth{0};
  size_t mHeight{0};
  std::vector<T> mBuffer;
};
