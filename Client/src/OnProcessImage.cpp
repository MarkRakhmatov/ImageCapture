#include "OnProcessImage.h"
#include <iostream>

EReadStatus OnProcessImage::Handle(DescriptorHolder& sock)
{
  int x = 0;
  int y = 0;
  // add wrapper to wait for response
  int bytesRead = read(sock.Get(), &x, sizeof(x));
  if(bytesRead < 0)
  {
      return EReadStatus::FAIL;
  }
  bytesRead = read(sock.Get(), &y, sizeof(y));
  if(bytesRead < 0)
  {
      return EReadStatus::FAIL;
  }
  std::cout << "x = " << x << std::endl
      << "y = " << y << std::endl;
  return EReadStatus::SUCCESS;
}
