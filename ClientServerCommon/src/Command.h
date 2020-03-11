#pragma once
#include <stdint.h>
enum class ECommand : uint32_t
{
  //SETUP_CAMERA,
  ECHO,
  PROCESS_IMAGE,
  SHUTDOWN,
  SIZE
};
