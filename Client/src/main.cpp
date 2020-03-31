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
          //std::cout << "Communication fail!" << std::endl;
          continue;
      }
      if(status == EConnectionStatus::SHUTDOWN)
      {
          return 0;
      }
  }
}

