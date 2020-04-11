#pragma once
#include "TypeDataParser.h"

#include <string>
#include <vector>
#include <unordered_map>


namespace Parser
{
	template<typename Token, typename Source>
	class TypeHandler
	{
	public:
		using TypeParser = TypeDataParser<Source, Token>;
		using Types = std::unordered_map<std::basic_string<Token>, TypeParser>;
		TypeHandler()=default;
		TypeHandler(const Types& types)
		:mTypes(types)
		{}
		bool ParseObject(Source& src, ObjectDescriptor<Token>& obj)
		{
			TypeParser* parser = FindTypeParser(obj.objName);
			if(!parser)
			{
				return false;
			}
			return parser->ReadObjectData(src, obj);
		}
		void AddTypes(const Types& types)
		{
			mTypes.insert(types.bebin(), types.end());
		}
	private:
		TypeDataParser<Source, Token>* FindTypeParser(const std::basic_string<Token>& objName)
		{
			auto it = std::find(mTypes.begin(), mTypes.end(), objName);
			if(it == mTypes.end())
			{
				return nullptr;
			}
			return *it.second;
		}
	private:
		Types mTypes;
	};
}
