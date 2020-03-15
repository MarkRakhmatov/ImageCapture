#pragma once
#include <stdint.h>

enum class EConnectionStatus : uint32_t
{
  FAIL,
  SUCCESS,
  SHUTDOWN,
  DISCONNECT,

};
