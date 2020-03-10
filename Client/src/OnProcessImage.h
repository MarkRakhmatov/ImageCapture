#pragma once
#include "IOnCommand.h"

class OnProcessImage : public IOnCommand
{
public:
  ~OnProcessImage() = default;
  void Handle(Socket& sock);
};
