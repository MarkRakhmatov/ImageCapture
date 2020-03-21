#include "JpegHelper.h"
#include <memory>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>
#include <ctime>

#include "ImageBuffer.h"

void AddDataToStream(std::ostringstream& ss)
{
  auto now = std::chrono::system_clock::now();

    ss << std::chrono::time_point_cast<std::chrono::milliseconds>(now)
  	  .time_since_epoch()
  	  .count();
}

int
JpegHelper::WriteBufferToFile (const std::vector<unsigned char>& buf,
			       const std::string& fileName)
{
  std::ostringstream ss;

  ss << fileName;
  AddDataToStream(ss);
  ss << ".jpeg";
  if(!buf.size())
  {
      return -1;
  }
  std::ofstream outFile(ss.str(), std::ios::binary | std::ios::app);
  outFile.write(reinterpret_cast<const char*>(buf.data()), buf.size());
  return 0;
}
