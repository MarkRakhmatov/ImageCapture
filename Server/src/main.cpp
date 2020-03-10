#include "Server.h"
#include <iostream>

int main()
{
  for(;;)
  {
        Server server;
        EConnectionStatus status = server.Listen();
        switch(status)
        {
        case EConnectionStatus::FAIL:
          continue;
        case EConnectionStatus::SHUTDOWN:
          return 0;
        default:
          continue;
        };
  }
}
