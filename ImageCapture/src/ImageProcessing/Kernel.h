#pragma once
#include <stdint.h>
#include <vector>

struct KernelUnit
{
  uint32_t i;
  uint32_t j;
  float coeff;
};

class Kernel
{
public:
  Kernel(std::vector<KernelUnit>& kernelUnits, uint32_t size)
  : mKernelUnits(kernelUnits)
  , mSize(size)
  {}
  Kernel() = default;
  Kernel(const Kernel&) = default;
  Kernel& operator =(const Kernel&) = default;
  Kernel(Kernel&& ker)
  : mKernelUnits(std::move(ker.mKernelUnits))
  , mSize(ker.mSize)
  {
    ker.mSize = 0;
  }
  Kernel& operator =(Kernel&& ker)
  {
    mKernelUnits = std::move(ker.mKernelUnits);
    mSize = ker.mSize;
    ker.mSize = 0;
    return *this;
  }
  std::vector<KernelUnit>& Get()
  {
      return mKernelUnits;
  }
  const std::vector<KernelUnit>& Get()const
  {
      return mKernelUnits;
  }
  uint32_t Size() const
  {
    return mSize;
  }
  void SetSize(uint32_t size)
  {
    mSize = size;
  }
private:
  std::vector<KernelUnit> mKernelUnits;
  uint32_t mSize{};
};
