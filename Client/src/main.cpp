#include "Client.h"
#include <iostream>

int main()
{
  for(;;)
  {
      ClientSide::Client client;

      auto status = client.Communicate();
      if(status == Communication::EConnectionStatus::FAIL)
      {
          //std::cout << "Communication fail!" << std::endl;
          continue;
      }
      if(status == Communication::EConnectionStatus::SHUTDOWN)
      {
          return 0;
      }
  }
}

