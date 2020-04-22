#include "Parser.h"
#include "OnSetup.h"
#include "Command.h"
#include "FileSource.h"
#include "CodeGeneration.h"
#include "ParserConfiguration.h"
#include "ParserUtils.h"
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

/*bool SkipLine(const std::string& line)
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
}*/

/*std::string ReadSettings(const std::string& filename)
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
}*/

bool SendFilteringSettings(Socket& sock)
{
  std::string settingsFileName("settings.txt");
  Parser::FileSource<char> fileSource(settingsFileName);
  Parser::ObjectParser<Parser::FileSource<char>, char> parser;
  auto config = Parser::GetDefaultParserConfig();
  std::vector<Parser::ObjectDescriptor<char>> objects;
  for(;;)
  {
	  Parser::ObjectDescriptor<char> objDesc;
	  Parser::EStatus status = parser.Parse(fileSource, config, objDesc);
	  RET_ON_SUCCESS(status == Parser::EStatus::FAIL, false);
	  if(status == Parser::EStatus::FILE_END)
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
  return true;
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
  RET_ON_FAIL(SendFilteringSettings(sock), EConnectionStatus::FAIL);
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

