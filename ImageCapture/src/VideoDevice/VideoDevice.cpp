#include "VideoDevice.h"
#include <thread>
#include <chrono>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>
#include <linux/v4l2-common.h>
#include <linux/v4l2-controls.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <pthread.h>

VideoDevice::VideoDevice(const std::string& deviceName)
: mDeviceName(deviceName)
{
  OpenDevice(mDeviceName);
  SetImageFormat();
}

void VideoDevice::SetImageFormat(int width, int height)
{
  v4l2_format imageFormat{0};
  imageFormat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  imageFormat.fmt.pix.width = width;
  imageFormat.fmt.pix.height = height;

  imageFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG;
  imageFormat.fmt.pix.field = V4L2_FIELD_NONE;

  auto res = AsyncIoctl(VIDIOC_S_FMT, &imageFormat);
  if(!res.second)
  {
      perror("Device could not set format, VIDIOC_S_FMT");
  }
}

MappedBuffer&
VideoDevice::GetBuffer()
{
  mBuffer.Reset();
  InitBuffer();

  return mBuffer;
}

void
VideoDevice::OpenDevice(const std::string& deviceName)
{
  auto res = WaitForAsyncCall<decltype(CheckOpenFile), CheckOpenFile>
			  (open, mTimeout, deviceName.c_str(), O_RDWR | O_NONBLOCK);
  if(!res.second)
  {
      perror("Failed to open device, OPEN");
  }
  mDescriptor = DescriptorHolder{std::move(res.first)};

  v4l2_capability capability;
  res = AsyncIoctl(VIDIOC_QUERYCAP, &capability);
  if(!res.second)
  {
      perror("Failed to get device capabilities, VIDIOC_QUERYCAP");
  }
  if((capability.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0)
  {
      std::cout << "Device not capable to capture video!" <<std::endl;
  }
}

void
VideoDevice::HandleParameters()
{
  v4l2_streamparm params{};
  params.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  auto res= AsyncIoctl(VIDIOC_G_PARM , &params);
  if(!res.second)
  {
      perror("Could not request stream parameters, VIDIOC_G_PARM");
      return;
  }
  if((params.parm.capture.capturemode & V4L2_MODE_HIGHQUALITY) != 0 )
  {
      return;
  }
  params.parm.capture.capturemode |= V4L2_MODE_HIGHQUALITY;
  res= AsyncIoctl(VIDIOC_S_PARM , &params);
  if(!res.second)
  {
      perror("Could not set stream parameters, VIDIOC_G_PARM");
  }
}

VideoDevice&
VideoDevice::operator = (VideoDevice&& dev)
{
  mDescriptor = std::move(dev.mDescriptor);
  mBuffer = std::move(dev.mBuffer);
  mDeviceName = std::move(dev.mDeviceName);
  return *this;
}

VideoDevice::VideoDevice (VideoDevice&& dev)
: mDescriptor(std::move(dev.mDescriptor))
, mBuffer(std::move(dev.mBuffer))
, mDeviceName(std::move(dev.mDeviceName))
{}

void
VideoDevice::InitBuffer()
{
  // 4. Request Buffers from the device
  v4l2_requestbuffers requestBuffer{0};
  requestBuffer.count = 1; // one request buffer
  requestBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; // request a buffer which we an use for capturing frames
  requestBuffer.memory = V4L2_MEMORY_MMAP;
  auto res = AsyncIoctl(VIDIOC_REQBUFS, &requestBuffer);
  if(!res.second)
  {
      perror("Could not request buffer from device, VIDIOC_REQBUFS");
      return;
  }

  // 5. Query the buffer to get raw data ie. ask for the you requested buffer
  // and allocate memory for it
  v4l2_buffer queryBuffer = {0};
  queryBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  queryBuffer.memory = V4L2_MEMORY_MMAP;
  queryBuffer.index = 0;

  res = AsyncIoctl(VIDIOC_QUERYBUF, &queryBuffer);
  if(!res.second)
  {
      perror("Device did not return the buffer information, VIDIOC_QUERYBUF");
      std::cout << "Errno: " << errno <<std::endl;
      return;
  }

  mBuffer.Set(NULL, queryBuffer.length, PROT_READ | PROT_WRITE, MAP_SHARED,
	      mDescriptor.Get(), queryBuffer.m.offset);

  // 6. Get a frame
  // Create a new buffer type so the device knows which buffer we are talking about

  v4l2_buffer bufferinfo{0};
  memset(&bufferinfo, 0, sizeof(bufferinfo));
  bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  bufferinfo.memory = V4L2_MEMORY_MMAP;
  bufferinfo.index = 0;
  // Activate streaming
  int type = bufferinfo.type;
  res = AsyncIoctl(VIDIOC_STREAMON, &type);
  if(!res.second)
  {
      perror("Could not start streaming, VIDIOC_STREAMON");
      Reset();
      return;
  }

  // Queue the buffer
  res = AsyncIoctl(VIDIOC_QBUF, &bufferinfo);
   if(!res.second)
  {
      perror("Could not queue buffer, VIDIOC_QBUF");
      Reset();
      return;
  }

  // Dequeue the buffer
  res = AsyncIoctl(VIDIOC_DQBUF, &bufferinfo);
  if(!res.second)
  {
      perror("Could not dequeue the buffer, VIDIOC_DQBUF");
      Reset();
      return;
  }
  // Frames get written after dequeuing the buffer
  mBuffer.Update(bufferinfo.bytesused);
  std::cout << "Buffer has: " << bufferinfo.bytesused / 1024
          << " KBytes of data" << std::endl;

  res = AsyncIoctl(VIDIOC_STREAMOFF, &type);
  if(!res.second)
  {
      perror("Could not end streaming, VIDIOC_STREAMOFF");
      std::cout << "Descriptor: " << mDescriptor.Get() <<std::endl;
      Reset();
      return;
  }
}

void
VideoDevice::Reset ()
{
  mBuffer.Reset();
  mDescriptor.Reset();
  OpenDevice(mDeviceName);
}

ImageBuffer<unsigned char>
GetImageBufferFromDevice (VideoDevice& device)
{
  MappedBuffer& buf = device.GetBuffer();
  auto imgBuff = JpegHelper::Decompress(reinterpret_cast<unsigned char*>(buf.Get()), buf.Size());
  if(imgBuff.Get()<0)
  {
      std::cout << "INVALID FILE FORMAT!!! FAILED TO DECOMPRESS JPEG "<<std::endl;
  }
  return imgBuff;
}
