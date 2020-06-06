#pragma once

#include <string>
#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "MappedBuffer.h"
#include "DescriptorHolder.h"
#include "CallWrapper.h"
#include <vector>
#include "Utils.h"

class VideoDevice
{
public:
  explicit VideoDevice(const std::string& deviceName, int width = 1920, int height = 1080);
  VideoDevice(VideoDevice&& dev);
  VideoDevice& operator=(VideoDevice&& dev);
  VideoDevice(const VideoDevice& res) = delete;
  VideoDevice& operator=(const VideoDevice& res) = delete;
  void Reset(const std::string& deviceName, int width = 1920, int height = 1080);

  MappedBuffer& GetBuffer();
  void HandleParameters();
private:
  template<typename T>
  std::pair<int, bool> AsyncIoctl(int request, T* pRequestData)
  {
    return WaitForAsyncCall<decltype(CheckIoctl), CheckIoctl>
      (ioctl, mTimeout, mDescriptor.Get(), request, pRequestData);
  }
  void OpenDevice();
  void SetImageFormat();
  void InitBuffer();
  void Reset();
private:
  DescriptorHolder mDescriptor{};
  MappedBuffer mBuffer{};
  std::string mDeviceName;
  int mHeight{};
  int mWidth{};

  const std::chrono::milliseconds mTimeout{500};
};

std::vector<unsigned char> GetImageBufferFromDevice(VideoDevice& device);
