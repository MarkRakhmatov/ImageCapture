#include "TypeInfo.h"
#include "CodeGeneration.h"
#include "ParserTypes.h"
#include "ParserUtils.h"
#include "ObjectHandler.h"
namespace Parser
{
	template<typename Source, typename Char>
	EStatus ReadArrayBottom(Source& src, const ParserConfiguration<Source, Char>& config, ObjectDataReaderFunc<Source, Char> dataReader, ObjectData& objdata)
	{
		auto status = ReadBlockStart(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);

		auto& subData = objdata.GetSubData();

		for(;;)
		{
			ObjectData data;
			status = dataReader(src, config, data);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
			subData.push_back(data);
			status = SkipChars(src, config);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			Char ch;
			status = src.PeekChar(ch);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			if(!config.IsSeparator(ch))
			{
				break;
			}

			status = src.GetChar(ch);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
		}

		return ReadBlockEnd(src, config);
	}

	template<typename Source, typename Char>
	EStatus ReadArrayImpl(Source& src, const ParserConfiguration<Source, Char>& config, ObjectDataReaderFunc<Source, Char> dataReader, ObjectData& obj, uint8_t arrayDepth)
	{
		auto status = EStatus::FAIL;
		if(arrayDepth == 1)
		{
			return ReadArrayBottom(src, config, dataReader, obj);
		}

		status = ReadBlockStart(src, config);
		RET_ON_FALSE(status == EStatus::SUCCESS, status);
		--arrayDepth;
		auto& subData = obj.GetSubData();
		for(;;)
		{
			subData.resize(subData.size() + 1);
			auto& currentObj = subData[subData.size()-1];

			status = ReadArrayImpl(src, config, dataReader, currentObj, arrayDepth);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);


			status = SkipChars(src, config);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			Char ch;
			status = src.PeekChar(ch);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			if(!config.IsSeparator(ch))
			{
				break;
			}

			status = src.GetChar(ch);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);
		}

		return ReadBlockEnd(src, config);
	}


	template<typename Source, typename Char>
	EStatus ReadArray(Source& src, const ParserConfiguration<Source, Char>& config, ObjectDataReaderFunc<Source, Char> dataReader, ObjectDescriptor<Char>& obj)
	{
		if(obj.arrayDepth == 0)
		{
			auto status = ReadBlockStart(src, config);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			status = dataReader(src, config, obj.objectData);
			RET_ON_FALSE(status == EStatus::SUCCESS, status);

			return ReadBlockEnd(src, config);
		}

		return ReadArrayImpl(src, config, dataReader, obj.objectData, obj.arrayDepth);
	}
}
