#include <iostream>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include "MappedBuffer.h"

void
MappedBuffer::Set(void* addr, size_t len, int prot, int flags, int fd, __off_t offset)
{
  void* buffPtr = ::mmap(addr, len, prot, flags, fd, offset);
  mBuffer = static_cast<char*>(buffPtr);
  memset(mBuffer, 0, len);
  mLength = len;
}

MappedBuffer::MappedBuffer (MappedBuffer&& otherBuffer)
{
  std::swap(otherBuffer.mBuffer, mBuffer);
  std::swap(otherBuffer.mLength, mLength);
  std::swap(otherBuffer.mSize, mSize);
}

MappedBuffer&
MappedBuffer::operator = (MappedBuffer&& otherBuffer)
{
  std::swap(otherBuffer.mBuffer, mBuffer);
  std::swap(otherBuffer.mLength, mLength);
  std::swap(otherBuffer.mSize, mSize);
  return *this;
}

bool
MappedBuffer::Reset()
{
  if(!mBuffer)
  {
      return false;
  }

  int ret = ::munmap(mBuffer, mLength);

  if(ret != 0)
  {
      std::cout << "Failed to unmaps buffer!" << std::endl;
      mBuffer = nullptr;

      return false;
  }
  mSize = 0;
  mLength = 0;
  mBuffer = nullptr;
  return true;
}

MappedBuffer::~MappedBuffer()
{
  Reset();
}


