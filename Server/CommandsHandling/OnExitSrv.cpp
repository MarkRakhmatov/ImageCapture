/*
 * OnExitSrv.cpp
 *
 *  Created on: Mar 14, 2020
 *      Author: mark
 */

#include "OnExitSrv.h"

EConnectionStatus
OnExitSrv::Handle(Socket& sock)
{
  EConnectionStatus status;
  auto res = sock.Send(&status, sizeof(status));
  if(!res.second)
  {
      return EConnectionStatus::FAIL;
  }
  return EConnectionStatus::DISCONNECT;
}
