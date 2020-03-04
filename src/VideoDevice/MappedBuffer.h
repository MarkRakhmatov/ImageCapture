#pragma once
#include <string>

class MappedBuffer
{
public:
  MappedBuffer() = default;
  ~MappedBuffer();
  MappedBuffer(MappedBuffer&&);
  MappedBuffer& operator=(MappedBuffer&&);
  MappedBuffer(const MappedBuffer&) = delete;
  MappedBuffer& operator=(const MappedBuffer&) = delete;

  void Set(void* addr, size_t len, int prot,
      int flags, int fd, __off_t offset);
  char* Get()
  {
    return mBuffer;
  }
  const char* Get() const
  {
    return mBuffer;
  }
  void Update(int size)
  {
    mSize = size;
  }
  int Size() const
  {
    return mSize;
  }
  bool Reset();
private:
  char* mBuffer = 0;
  int mSize = 0;
  int mLength = 0;
};

