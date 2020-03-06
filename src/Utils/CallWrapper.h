#pragma once
#include <chrono>
#include <thread>
#include <type_traits>

enum class ECheckOperation
{
  IN_PROGRESS,
  SUCCESS,
  FAIL,

};

class CallWrapper
{
public:

template<
  typename TCheckerFunc,
  TCheckerFunc& CheckerFunc,
  typename TFunc,
  typename ... TArgs>

  std::pair<typename std::result_of<TFunc(TArgs...)>::type, bool>
  WaitForAsyncCall(TFunc&& func, std::chrono::milliseconds timeout, TArgs ... args)
  {
    using ReturnType = typename std::result_of<TFunc(TArgs...)>::type;
    auto startTime = std::chrono::system_clock::now();
    do
    {
      ReturnType res = func(std::forward<TArgs>(args)...);
      auto opStatus = CheckerFunc(res);
      if(opStatus == ECheckOperation::SUCCESS)
      {
	  return {res, true};
      }
      if(opStatus == ECheckOperation::FAIL)
      {
	  break;
      }
    } while(std::chrono::system_clock::now() - startTime < timeout);
    return {ReturnType{}, false};
  }
};

ECheckOperation CheckIoctl(int val);

ECheckOperation CheckOpenFile(int descriptor);

