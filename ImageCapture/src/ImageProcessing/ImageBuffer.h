#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <utility>
#include <array>
#include <stdint.h>
#include <cassert>

enum class EPixelType : int16_t
{
  GRAY_SCALE,
  RGB,

  SIZE
};
template<typename TUnderlaying>
struct EnumClassHash
{
    template <typename T>
    TUnderlaying operator()(T t) const
    {
        return static_cast<TUnderlaying>(t);
    }
};

static uint32_t GetPixelSizeByType(EPixelType type)
{
  static const std::array<std::pair<EPixelType, uint32_t>, 2>
	    pixelTypeToSize{
	    std::pair<EPixelType, uint32_t>{EPixelType::GRAY_SCALE, 1},
	    std::pair<EPixelType, uint32_t>{EPixelType::RGB, 3}};
  if(type >= EPixelType::SIZE)
  {
      return 0;
  }
  return pixelTypeToSize[static_cast<int16_t>(type)].second;
}

template<typename T>
class ImageBuffer
{
public:
  ImageBuffer() = default;
  ImageBuffer(ImageBuffer&) = delete;
  ImageBuffer& operator=(ImageBuffer&) = delete;
  using ElementType = T;
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

  uint32_t GetWidth() const
  {
    return mWidth;
  }
  uint32_t GetHeight() const
  {
    return mHeight;
  }

  uint32_t GetPixelSize() const
  {
    return GetPixelSizeByType(mPixelType);
  }
  EPixelType GetPixelType() const
  {
    return mPixelType;
  }


  T* GetElement(uint32_t i, uint32_t j)
  {
    return &mBuffer[(i*mWidth + j)*GetPixelSize()];
  }

  const T* GetElement(uint32_t i, uint32_t j) const
  {
    return &mBuffer[(i*mWidth + j)*GetPixelSize()];
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
  uint32_t mWidth{0};
  uint32_t mHeight{0};
  EPixelType mPixelType{EPixelType::GRAY_SCALE};
  std::vector<T> mBuffer;
};
