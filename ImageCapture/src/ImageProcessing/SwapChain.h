#pragma once
#include <utility>

template<typename T>
class SwapChain
{
public:
  SwapChain(T* active, T* back)
  :mActiveObj(active)
  , mBackObj(back)
  {}
  T* GetActive()
  {
    return mActiveObj;
  }
  void Swap()
  {
    std::swap(mBackObj, mActiveObj);
  }
private:
  T* mActiveObj{nullptr};
  T* mBackObj{nullptr};
};
