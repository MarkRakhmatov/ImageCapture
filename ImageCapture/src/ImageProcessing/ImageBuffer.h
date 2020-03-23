#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <utility>
#include <cassert>

enum class EPixelType
{
  GRAY_SCALE,
  RGB,

  SIZE
};
struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};
template<typename T>
class ImageBuffer
{
public:
  ImageBuffer() = default;
  ImageBuffer(ImageBuffer&) = delete;
  ImageBuffer& operator=(ImageBuffer&) = delete;

  ImageBuffer(int width, int height, EPixelType pixelType)
  : mWidth(width)
  , mHeight(height)
  , mPixelType(pixelType)
  , mBuffer(mHeight*mWidth*GetPixelSize(), T{})
  {
  }

  ImageBuffer(ImageBuffer&& otherBuffer)
  : mWidth(otherBuffer.mWidth)
  , mHeight(otherBuffer.mHeight)
  , mPixelType(otherBuffer.mPixelType)
  , mBuffer(std::move(otherBuffer.mBuffer))
  {
    otherBuffer.mWidth = 0;
    otherBuffer.mHeight = 0;
  }

  ImageBuffer& operator=(ImageBuffer&& otherBuffer)
  {
    std::swap(mWidth, otherBuffer.mWidth);
    std::swap(mHeight, otherBuffer.mHeight);
    std::swap(mPixelType, otherBuffer.mPixelType);
    mBuffer = std::move(otherBuffer.mBuffer);
    return *this;
  }

  ~ImageBuffer() = default;

  size_t GetWidth() const
  {
    return mWidth;
  }
  size_t GetHeight() const
  {
    return mHeight;
  }

  size_t GetPixelSize() const
  {
    return pixelTypeToSize.at(mPixelType);
  }
  EPixelType GetPixelType() const
  {
    return mPixelType;
  }


  T* GetElement(size_t i, size_t j)
  {
    auto index = (i*mWidth + j)*GetPixelSize();
    return &mBuffer[index];
  }

  const T* GetElement(size_t i, size_t j) const
  {
    auto index = (i*mWidth + j)*GetPixelSize();
    return &mBuffer[index];
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
  std::unordered_map<EPixelType, size_t, EnumClassHash>
	    pixelTypeToSize{
	      {EPixelType::GRAY_SCALE, 1},
	      {EPixelType::RGB, 3}};
  EPixelType mPixelType{EPixelType::GRAY_SCALE};
  std::vector<T> mBuffer;
};
