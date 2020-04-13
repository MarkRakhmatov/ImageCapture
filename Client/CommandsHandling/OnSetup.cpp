#include "Parser.h"
#include "OnSetup.h"
#include "Command.h"
#include "FileSource.h"
#include "ParserConfiguration.h"
#include <fstream>
#include <string>

EConnectionStatus
OnSetup::Handle(Socket& sock)
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
  Parser::FileSource<char> fileSource(settingsFileName);
  Parser::Parser<Parser::FileSource<char>, char> parser;
  auto config = Parser::GetDefaultParserConfig();
  std::vector<Parser::ObjectDescriptor<char>> objects;
  for(;;)
  {
	  Parser::ObjectDescriptor<char> objDesc;
	  if(!parser.Parse(fileSource, config, objDesc))
	  {
		  break;
	  }
	  if(config.IsTypeDecl(objDesc.typeName))
	  {
		  config.AddType(objDesc);
		  continue;
	  }
	  objects.push_back(objDesc);
  }

  std::string settingsStr = ReadSettings(settingsFileName);
  uint32_t size{static_cast<uint32_t>(settingsStr.size())};
  if(!size)
  {
      std::cout << "Failed to get settings!"<< std::endl;
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
OnSetup::SendRequest(Socket& sock)
{
  EProcessImage command = EProcessImage::SETUP_CAMERA;

  auto res = sock.SendData(&command);
  if(!res.second)
  {
      std::cout << "Failed to send command!" << std::endl;
      return EConnectionStatus::FAIL;
  }
  SendFilteringSettings(sock);
  return EConnectionStatus::SUCCESS;
}

EConnectionStatus
OnSetup::GetResponse(Socket& sock)
{
  EConnectionStatus status = EConnectionStatus::FAIL;
  sock.ReadData(&status);
  if(status == EConnectionStatus::FAIL)
  {
      std::cout << "Failed to setup camera!" << std::endl;
      return status;
  }

  std::cout << "Successfully setup camera!" << std::endl;
  return status;
}

