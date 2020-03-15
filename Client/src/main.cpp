#include "Client.h"
#include <iostream>

int main()
{
  for(;;)
  {
      Client client;

      auto status = client.Communicate();
      if(status == EConnectionStatus::FAIL)
      {
          std::cout << "Communication fail!" << std::endl;
          break;
      }
      if(status == EConnectionStatus::DISCONNECT)
      {
          return 0;
      }
  }
}

