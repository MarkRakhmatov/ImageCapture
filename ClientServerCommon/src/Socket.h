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

class Socket
{
public:
  Socket(int&& descriptor)
  : mDescriptor(std::move(descriptor))
  {
    if(descriptor < 0)
    {
        std::string message = "Failed to accept connection";
    }
  }
  Socket(const std::string& serverIp)
  : mDescriptor(socket(AF_INET, SOCK_STREAM, 0))
  {
    mServAddr.sin_family = AF_INET;
    mServAddr.sin_port = htons(3425);

    if(inet_pton(AF_INET, serverIp.data(), &mServAddr.sin_addr)<=0)
    {
        std::cout<<"Failed to convert server ip to binary format!"<<std::endl;
    }

    if(connect(mDescriptor.Get(), reinterpret_cast<sockaddr*>(&mServAddr), sizeof(mServAddr)) < 0)
    {
        std::cout<<"Failed to open connection!"<<std::endl;
    }
  }

  int Get()
  {
    return mDescriptor.Get();
  }

  template <typename T>
  std::pair<int, bool> Read(T* data, size_t size, int flags = 0)
  {
    size_t bytesCount = recv(mDescriptor.Get(), data, size, flags);
    if(bytesCount != size)
    {
        return{};
    }
    return {bytesCount, true};
  }

  template <typename T>
  std::pair<int, bool> Send(T* data, size_t size, int flags = 0)
  {
    size_t bytesCount = send(mDescriptor.Get(), data, size, flags);
    if(bytesCount != size)
    {
        return{};
    }
    return {bytesCount, true};
  }

  template<typename TArg, typename ... TArgs>
  std::pair<int, bool> SendData(TArg arg1, TArgs ... args)
  {
    static_assert(std::is_pointer<TArg>::value == true, "Argument must be pointer type!");
    static_assert(std::is_pod<TArg>::value == true,
            "Argument must be pointer to POD type!");
    auto res = Send(arg1, sizeof(*arg1));
    if(!res.second)
    {
      return res;
    }
    return SendData(args...);
  }

  template<typename TArg, typename ... TArgs>
  std::pair<int, bool> ReadData(TArg arg1, TArgs ... args)
  {
    static_assert(std::is_pointer<TArg>::value == true, "Argument must be pointer type!");

    auto res = Read(arg1, sizeof(*arg1));
    if(!res.second)
    {
      return res;
    }
    return ReadData(args...);
  }

  template<typename TArg>
  std::pair<int, bool> SendData(TArg arg)
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
  std::pair<int, bool> ReadData(TArg arg)
  {
    static_assert(std::is_pointer<TArg>::value == true, "Argument must be pointer type!");

    using ArgType = typename std::remove_pointer<TArg>::type;

    static_assert(std::is_pod<ArgType>::value == true,
        "Argument must be pointer to POD type!");
    static_assert(std::is_pointer<ArgType>::value  == false,
        "Argument must be pointer to POD type, but not pointer to pointer!!!");
    auto res = Read(arg, sizeof(*arg));
    return res;
  }
private:
  DescriptorHolder mDescriptor;
  sockaddr_in mServAddr{};
};
