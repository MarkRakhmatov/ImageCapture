#include "SettingsHandler.h"
#include <string>
#include <sstream>

void ReadConvKernel(std::stringstream& ss, ConvKernel& ker)
{
	uint32_t kerSize{};
	ss >> kerSize;
	ker.resize(kerSize);
	for(uint32_t i = 0; i < kerSize; ++i)
	{
	   std::vector<KernelType> row(kerSize, 0.0f);
	   for(uint32_t j = 0; j < kerSize; ++j)
	   {
		   KernelType tmp{};
		   ss >> tmp;
		   row[j] = tmp;
	   }
	   ker[i] = std::move(row);
	}
}
void ReadConvHandlers(std::stringstream& ss, Settings& settings)
{
	uint32_t handlersCount;
	ss >> handlersCount;
	settings.resize(handlersCount);
	for(size_t i = 0 ; i < handlersCount; ++i)
	{
		//ReadConvKernel(ss, settings[i]);
	}
}

bool GetSettings(Socket& sock)
{
  uint32_t size{};
  auto res = sock.ReadData(&size);
  do
  {
     if(!res.second)
     {
         break;
     }
     std::string settingsStr(size, '\0');
     for(uint32_t i = 0; i < size; ++i)
     {
         char tmp{};
         res = sock.ReadData(&tmp);
         if(!res.second)
         {
             break;
         }
         settingsStr[i] = tmp;
     }
     std::stringstream ss(settingsStr);
     std::string handlerType;
     ss >> handlerType;
     const std::string convHandlerType("conv");
     if(handlerType != convHandlerType)
     {
    	 std::cout << "unsupported handler type: " << handlerType <<std::endl;
    	 break;
     }
 	 auto& settings = SettingsHandler::Get().GetSettings();
 	 ReadConvHandlers(ss, settings);
     return true;

  } while(false);
  return false;
}

Settings& SettingsHandler::GetSettings()
{
	return mSettings;
}
