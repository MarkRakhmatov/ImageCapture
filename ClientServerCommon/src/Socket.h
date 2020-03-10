#pragma once
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <utility>
#include <string>
#include "DescriptorHolder.h"
#include <arpa/inet.h>
#include <stdexcept>
#include <iostream>

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
private:
  DescriptorHolder mDescriptor;
  sockaddr_in mServAddr{};
};
