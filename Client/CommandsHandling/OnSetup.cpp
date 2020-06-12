#include "Parser.h"
#include "OnSetup.h"
#include "Command.h"
#include "FileSource.h"
#include "CodeGeneration.h"
#include "ObjectDescriptor.h"
#include "ParserConfiguration.h"
#include "ParserUtils.h"
#include "CommunicationUtils.h"

#include <fstream>
#include <string>


namespace ClientSide
{
	using namespace Communication;
	using namespace Parser;

	EConnectionStatus
	OnSetup::Handle(Socket& sock)
	{
	  EConnectionStatus status = SendRequest(sock);
	  RET_ON_TRUE(status == EConnectionStatus::FAIL, status);
	  return GetResponse(sock);
	}

	bool SendObjectData(Socket& sock, const ObjectData& objData)
	{
		const auto& data = objData.GetRawData();
		bool res = SendContainer(sock, data);
		RET_ON_FALSE(res, false);
		const auto& subData = objData.GetSubData();
		uint64_t size = subData.size();
		res = sock.SendData(&size);
		RET_ON_FALSE(res, false);
		for(uint64_t i = 0 ; i < size; ++i)
		{
			res = SendObjectData(sock, subData[i]);
			RET_ON_FALSE(res, false);
		}
		return true;
	}

	bool WriteObject(Socket& sock, const ObjectDescriptor<char>& object)
	{
		bool res = SendString(sock, object.name);
		RET_ON_FALSE(res, false);
		res = sock.SendData(&object.type);
		RET_ON_FALSE(res, false);
		res = sock.SendData(&object.arrayDepth);
		RET_ON_FALSE(res, false);

		return SendObjectData(sock, object.objectData);
	}

	bool WriteObjects(Socket& sock, const std::vector<ObjectDescriptor<char>>& objects)
	{
		bool res = false;
		uint64_t size = objects.size();
		res = sock.SendData(&size);
		RET_ON_FALSE(res, false);

		for(auto& object : objects)
		{
			res = WriteObject(sock, object);
			RET_ON_FALSE(res, false);
		}
		return true;
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
			if(status == EStatus::DATA_END)
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
		auto config = GetDefaultParserConfig<FileSource<char>>();
		std::vector<ObjectDescriptor<char>> objects;
		EStatus status = ReadSettings(fileSource, config, objects);
		switch(status)
		{
		case EStatus::SUCCESS:
			break;
		case EStatus::DATA_END:
		case EStatus::FAIL:
		default:
			return EConnectionStatus::FAIL;
		}

		bool res = WriteObjects(sock, objects);

		return res ? EConnectionStatus::SUCCESS : EConnectionStatus::FAIL;
	}

	EConnectionStatus
	OnSetup::SendRequest(Socket& sock)
	{
	  EProcessImage command = EProcessImage::SETUP_CAMERA;

	  bool res = sock.SendData(&command);
	  if(!res)
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
	  bool res = sock.ReadData(&status);
	  if(!res)
	  {
		  std::cout << "Failed to read status!" << std::endl;
		  return EConnectionStatus::FAIL;
	  }
	  if(status == EConnectionStatus::FAIL)
	  {
		  std::cout << "Failed to update settings!" << std::endl;
		  return status;
	  }

	  std::cout << "Successfully updated settings!" << std::endl;
	  return status;
	}
}

