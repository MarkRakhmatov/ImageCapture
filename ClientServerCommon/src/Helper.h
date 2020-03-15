#pragma once
#include "Socket.h"
#include "Response.h"

static EConnectionStatus ReadStatus(Socket& sock)
{
  EConnectionStatus status = EConnectionStatus::FAIL;
  auto res = sock.Read(&status, sizeof(status));

  return status;

  if(!res.second)
  {
      return EConnectionStatus::FAIL;
  }
}
