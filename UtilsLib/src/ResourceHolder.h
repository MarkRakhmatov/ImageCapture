#pragma once
#include <utility>

template<typename TResoure,
typename DeleterFuncDecl,
DeleterFuncDecl& DeleterFunc>
class ResourceHolder
{
public:
  ResourceHolder() = default;
  ResourceHolder(TResoure&& res)
  : mResource(std::forward<TResoure>(res))
  , mOwn(true)
  {}
  ResourceHolder(ResourceHolder&& res)
  {
    std::swap(mResource, res.mResource);
    std::swap(mOwn, res.mOwn);
  }
  ResourceHolder& operator=(ResourceHolder&& res)
  {
    std::swap(mResource, res.mResource);
    std::swap(mOwn, res.mOwn);
    return *this;
  }
  ResourceHolder(const ResourceHolder& res) = delete;
  ResourceHolder& operator=(const ResourceHolder& res) = delete;
  TResoure& Get()
  {
    return mResource;
  }

  void Reset()
  {
    if(mOwn)
    {
	DeleterFunc(mResource);
	mResource = {};
	mOwn = false;
    }
  }

  void Reset(TResoure&& res)
  {
    Reset();
    mResource = res;
    mOwn = true;;
  }
  ~ResourceHolder()
  {
    Reset();
  }
private:
  TResoure mResource;
  bool mOwn = false;
};

