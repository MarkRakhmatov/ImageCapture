#include "OnProcessImage.h"
#include <iostream>
#include <unistd.h>

void OnProcessImage::Handle(Socket& sock)
{
  int32_t x = 0;
  int32_t y = 0;

  auto result = sock.Read(&x, sizeof(x));
  if(!result.second)
  {
      throw std::runtime_error("Failed to get server response!");
  }
  result = sock.Read(&y, sizeof(y));
  if(!result.second)
  {
      throw std::runtime_error("Failed to get server response!");
  }
  std::cout << "x = " << x << std::endl
        << "y = " << y << std::endl;
}
