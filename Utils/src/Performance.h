#pragma once
#include <type_traits>
#include <chrono>
#include <utility>

template<typename T_Func, typename ... T_Args, typename TimeUnit>
typename std::result_of<T_Func(T_Args...)>::type
Performance(
		TimeUnit& elapsedTime, T_Func&& func, T_Args&& ... args)
{
    const auto startTime = std::chrono::system_clock::now();
    auto res = func(std::forward<T_Args>(args)...);
    elapsedTime =
    		std::chrono::duration_cast<TimeUnit>
    						(std::chrono::system_clock::now() - startTime);
    return res;
}
