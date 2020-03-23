#include "OnCapture.h"
#include "Command.h"
#include <fstream>

EConnectionStatus
OnCapture::Handle(Socket& sock)
{
  EConnectionStatus status = SendRequest(sock);
  if(status == EConnectionStatus::FAIL)
  {
      return status;
  }
  return GetResponse(sock);
}

void SendFilteringSettings(Socket& sock)
{
  std::string settingsFileName("settings.txt");
  std::ifstream settings(settingsFileName);
  std::string settingsStr;
  std::string temp;
  while(settings >> temp)
  {
      settingsStr += " " + temp;
  }
  settings.close();
  uint32_t size{static_cast<uint32_t>(settingsStr.size())};
  sock.SendData(&size);
  if(!size)
  {
      std::cout << "Failed to get settings! Use default settings."<< std::endl;
      return;
  }
  std::pair<int,bool> res{};
  for(size_t i = 0; i < size; ++i)
  {
      res = sock.SendData(&settingsStr[i]);
      if(!res.second)
      {
          std::cout << "Failed to send settings!"<< std::endl;
          return;
      }
  }

}

EConnectionStatus
OnCapture::SendRequest(Socket& sock)
{
  EProcessImage command = EProcessImage::PROCESS_IMAGE;

  auto res = sock.SendData(&command);
  if(!res.second)
  {
      return EConnectionStatus::FAIL;
  }
  SendFilteringSettings(sock);
  return EConnectionStatus::SUCCESS;
}

EConnectionStatus
OnCapture::GetResponse(Socket& sock)
{
  EConnectionStatus status = EConnectionStatus::FAIL;
  sock.ReadData(&status);

  if(status == EConnectionStatus::FAIL)
  {
      return status;
  }

  int32_t x = 0;
  int32_t y = 0;

  auto res = sock.ReadData(&x, &y);
  if(!res.second)
  {
      return EConnectionStatus::FAIL;
  }
  std::cout << "x = " << x << std::endl
        << "y = " << y << std::endl;
  return status;
}
