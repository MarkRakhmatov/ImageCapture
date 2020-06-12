#pragma once
#include "DescriptorHolder.h"

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <utility>
#include <string>
#include <arpa/inet.h>
#include <stdexcept>
#include <iostream>
#include <type_traits>

namespace Communication
{
	class Socket
	{
	public:
	  Socket(int descriptor)
	  : mDescriptor(std::move(descriptor))
	  {
		if(descriptor < 0)
		{
			mDescriptor.Reset();
		}
	  }
	  ~Socket()
	  {
		if(mDescriptor.Get() && 0!= ::shutdown(mDescriptor.Get(), SHUT_RDWR))
		{
			std::cout<<"Failed to shutdown socket!"<<std::endl;
		}
	  }
	  void Reset()
	  {
		  mDescriptor.Reset();
	  }

	  void Reset(int descriptor)
	  {
		  mDescriptor = std::move(descriptor);
	  }
	  int Get()
	  {
		return mDescriptor.Get();
	  }

	  template <typename T>
	  bool Read(T* data, size_t size, int flags = 0)
	  {
		size_t bytesCount = recv(mDescriptor.Get(), data, size, flags);
		if(bytesCount != size)
		{
			return false;
		}
		return true;
	  }

	  template <typename T>
	  bool Send(T* data, size_t size, int flags = 0)
	  {
		size_t bytesCount = send(mDescriptor.Get(), data, size, flags);
		if(bytesCount != size)
		{
			return false;
		}
		return true;
	  }

	  template<typename TArg, typename ... TArgs>
	  bool SendData(TArg arg1, TArgs ... args)
	  {
		static_assert(std::is_pointer<TArg>::value == true, "Argument must be pointer type!");
		static_assert(std::is_pod<TArg>::value == true,
				"Argument must be pointer to POD type!");
		bool res = Send(arg1, sizeof(*arg1));
		if(!res)
		{
		  return res;
		}
		return SendData(args...);
	  }

	  template<typename TArg, typename ... TArgs>
	  bool ReadData(TArg arg1, TArgs ... args)
	  {
		static_assert(std::is_pointer<TArg>::value == true, "Argument must be pointer type!");

		bool res = Read(arg1, sizeof(*arg1));
		if(!res)
		{
		  return res;
		}
		return ReadData(args...);
	  }

	  template<typename TArg>
	  bool SendData(TArg arg)
	  {
		static_assert(std::is_pointer<TArg>::value == true,
			"The argument must be a pointer type!");

		using ArgType = typename std::remove_pointer<TArg>::type;

		static_assert(std::is_pod<ArgType>::value == true,
			"The argument must be a pointer to a POD type!");
		static_assert(std::is_pointer<ArgType>::value  == false,
			"The argument must be a pointer to a POD type, but not a pointer to a pointer!!!");

		auto res = Send(arg, sizeof(*arg));

		return res;
	  }

	  template<typename TArg>
	  bool ReadData(TArg arg)
	  {
		static_assert(std::is_pointer<TArg>::value == true, "Argument must be pointer type!");

		using ArgType = typename std::remove_pointer<TArg>::type;

		static_assert(std::is_pod<ArgType>::value == true,
			"Argument must be pointer to POD type!");
		static_assert(std::is_pointer<ArgType>::value  == false,
			"Argument must be pointer to POD type, but not pointer to pointer!!!");
		return Read(arg, sizeof(*arg));;
	  }
	private:
	  DescriptorHolder mDescriptor;
	};
}
