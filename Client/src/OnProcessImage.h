#pragma once
#include "IOnCommand.h"

class OnProcessImage : public IOnCommand
{
public:
  ~OnProcessImage() = default;
  EReadStatus Handle(DescriptorHolder& sock);
};
