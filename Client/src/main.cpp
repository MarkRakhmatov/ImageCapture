#include "Client.h"
#include <iostream>

int main()
{
  for(;;)
  {
    try
    {
      Client client;
      for(;;)
      {
          auto status = client.Communicate();
          if(status == EConnectionStatus::FAIL)
          {
              std::cout << "Communication fail!" << std::endl;
              break;
          }
      }
    }
    catch(const std::exception& ex)
    {
        std::cout << "Exception! Message:" << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Unknown exception caught\n";
    }
  }
}

