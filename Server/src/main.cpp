#include "Server.h"
#include <iostream>

int main()
{
  for(;;)
  {
	  ServerSide::Server server;
	  Communication::EConnectionStatus status = server.Listen();
        switch(status)
        {
        case Communication::EConnectionStatus::FAIL:
          continue;
        case Communication::EConnectionStatus::SHUTDOWN:
          return 0;
        default:
          continue;
        };
  }
}
