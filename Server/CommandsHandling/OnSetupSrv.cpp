#include "OnSetupSrv.h"
#include "SettingsHandler.h"
#include "CodeGeneration.h"
#include "CommunicationUtils.h"

namespace ServerSide
{
	using namespace Communication;
	using namespace Parser;

	bool ReadObjectData(Socket& sock, ObjectData& objData)
	{
		auto& data = objData.GetRawData();
		bool res = ReadContainer(sock, data);
		RET_ON_FALSE(res, false);
		auto& subData = objData.GetSubData();
		uint64_t size{0};
		res = sock.ReadData(&size);
		RET_ON_FALSE(res, false);
		for(uint64_t i = 0 ; i < size; ++i)
		{
			res = ReadObjectData(sock, subData[i]);
			RET_ON_FALSE(res, false);
		}
		return true;
	}
	bool ReadObject(Socket &sock, Parser::ObjectDescriptor<char>& object)
	{
		bool res = ReadString(sock, object.name);
		RET_ON_FALSE(res, false);
		res = sock.ReadData(&object.type);
		RET_ON_FALSE(res, false);
		res = sock.ReadData(&object.arrayDepth);
		RET_ON_FALSE(res, false);

		return ReadObjectData(sock, object.objectData);
	}

	bool ReadObjects(Socket &sock, std::vector<Parser::ObjectDescriptor<char>>& objects)
	{
		uint64_t objCount{0};
		bool res = sock.ReadData(&objCount);
		RET_ON_FALSE(res, false);
		for(uint64_t i = 0; i < objCount; ++i)
		{
			res = ReadObject(sock, objects[0]);
			RET_ON_FALSE(res, false);
		}
		return true;
	}

	EConnectionStatus OnSetupSrv::Handle(Socket &sock)
	{
		std::vector<Parser::ObjectDescriptor<char>> objects;
		bool res = ReadObjects(sock, objects);
		RET_ON_FALSE(res, EConnectionStatus::FAIL);
		auto& settings = SettingsHandler::Get().GetSettings();
		settings = std::move(objects);
		return EConnectionStatus::SUCCESS;
	}
}
