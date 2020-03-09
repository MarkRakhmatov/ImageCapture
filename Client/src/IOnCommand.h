#pragma once
#include <memory>
#include <vector>
#include <string>
#include "DescriptorHolder.h"

enum class EReadStatus
{
  SUCCESS,
  FAIL,

};

class IOnCommand
{
public:
  virtual ~IOnCommand(){};
  virtual EReadStatus Handle(DescriptorHolder& sock) = 0;
};
