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
bool SkipLine(const std::string& line)
{
	if(line.empty())
	{
		return true;
	}
	if(line.find(";", 0) == 0)
	{
		return true;
	}
	return false;
}
std::string ReadSettings(const std::string& filename)
{
	std::ifstream settings(filename);
	std::string settingsStr;
	for(std::string temp; std::getline(settings, temp);)
	{
		if(SkipLine(temp))
		{
			continue;
		}
	    settingsStr += " " + temp;
	}
	settings.close();
	return settingsStr;
}

void SendFilteringSettings(Socket& sock)
{
  std::string settingsFileName("settings.txt");
  std::string settingsStr = ReadSettings(settingsFileName);
  uint32_t size{static_cast<uint32_t>(settingsStr.size())};
  if(!size)
  {
      std::cout << "Failed to get settings! Use default settings."<< std::endl;
      return;
  }
  sock.SendData(&size);
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
  // move filtering settings code to OnSetup command handler
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
  std::cout << "x = " << x << std::endl;
  std::cout << "y = " << y << std::endl;
  return status;
}
