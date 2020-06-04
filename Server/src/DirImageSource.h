#pragma once

#include <IImageSource.h>
#include <dirent.h>
#include "ResourceHolder.h"

namespace ServerSide
{
	class DirImageSource: public IImageSource<unsigned char>
	{
	public:
		DirImageSource(const std::string& dirName);
		ImageBuffer<unsigned char> GetImage() override;
	private:
		bool GetJpegFromFile(std::vector<unsigned char>& jpegBuff);
	private:
		std::string mDirectoryName;
		ResourceHolder<DIR*, decltype(closedir), closedir> mDir;
	};

}
