#include "DirImageSource.h"
#include "CodeGeneration.h"
#include "JpegHelper.h"
#include <fstream>

namespace ServerSide
{

	DirImageSource::DirImageSource(const std::string &dirName)
	 : mDirectoryName(dirName)
	 , mDir(opendir(dirName.c_str()))
	{
	}

	std::vector<unsigned char> DirImageSource::GetImage()
	{
		std::vector<unsigned char> jpegBuff;
		bool res = GetJpegFromFile(jpegBuff);
		if(!res)
		{
			return {};
		}
		return jpegBuff;
	}

	bool DirImageSource::GetJpegFromFile(std::vector<unsigned char>& jpegBuff)
	{
		struct dirent* entry = nullptr;
		for(;;)
		{
			entry = readdir(mDir.Get());
			RET_ON_FALSE(entry, false);
			if(entry->d_type == DT_REG)
			{
				break;
			}
		}

		std::string fileName = mDirectoryName + '/' + entry->d_name;
		std::ifstream imageStream(fileName, std::ifstream::binary);
		RET_ON_FALSE(imageStream, false);
		imageStream.seekg (0, imageStream.end);
		int length = imageStream.tellg();
		imageStream.seekg (0, imageStream.beg);

		jpegBuff.resize(static_cast<size_t>(length));
		imageStream.read(reinterpret_cast<char*>(jpegBuff.data()), length);
		RET_ON_FALSE(imageStream, false);

		return true;
	}

}
