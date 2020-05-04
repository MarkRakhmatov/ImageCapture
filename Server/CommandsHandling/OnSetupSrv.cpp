#include "OnSetupSrv.h"
#include "SettingsHandler.h"


EConnectionStatus ReadObject(Socket &sock, Parser::ObjectDescriptor<char>& obj)
{
	/*obj.typeName;
	obj.objName;
	obj.objectData;
	obj.subObjects;*/
	return EConnectionStatus::FAIL;
}

EConnectionStatus OnSetupSrv::Handle(Socket &sock)
{
	auto& settings = SettingsHandler::Get().GetSettings();
	uint32_t objCount{0};
	auto res = sock.ReadData(&objCount);
	if(!res.second)
	{
		return EConnectionStatus::FAIL;
	}
	for(uint32_t i = 0; i < objCount; ++i)
	{
		uint32_t type{0};
		uint64_t dataSize{0};
		res = sock.ReadData(&type, &dataSize);
	}
	return EConnectionStatus::SUCCESS;
}
