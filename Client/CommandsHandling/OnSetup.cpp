#include "Parser.h"
#include "OnSetup.h"
#include "Command.h"
#include "FileSource.h"
#include "CodeGeneration.h"
#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include <fstream>
#include <string>

using namespace Parser;

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

EStatus ReadSettings(FileSource<char>& fileSource,
		ParserConfiguration<FileSource<char>, char>& config,
		std::vector<ObjectDescriptor<char>>& objects)
{
	ObjectParser<FileSource<char>, char> parser;
	for(;;)
	{
		ObjectDescriptor<char> objDesc;
		EStatus status = parser.Parse(fileSource, config, objDesc);
		RET_ON_TRUE(status == EStatus::FAIL, status);
		if(status == EStatus::FILE_END)
		{
			break;
		}
		if(config.IsTypeDecl(objDesc.type))
		{
			config.AddType(objDesc);
			continue;
		}
		objects.push_back(objDesc);
	}
	return EStatus::SUCCESS;
}

EConnectionStatus SendFilteringSettings(Socket& sock)
{
	std::string settingsFileName("settings.txt");
	FileSource<char> fileSource(settingsFileName);
	ParserConfiguration<FileSource<char>, char> config = GetDefaultParserConfig<FileSource<char>>();
	std::vector<ObjectDescriptor<char>> objects;
	EStatus status = ReadSettings(fileSource, config, objects);
	switch(status)
	{
	case EStatus::SUCCESS:
		break;
	case EStatus::FILE_END:
	case EStatus::FAIL:
	default:
		return EConnectionStatus::FAIL;
	}

	//WriteObjects(sock, objects);

	return EConnectionStatus::SUCCESS;
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

  return SendFilteringSettings(sock);
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

