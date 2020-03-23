#include "OnExit.h"
#include "Command.h"

EConnectionStatus
OnExit::Handle(Socket& sock)
{
  return EConnectionStatus::SHUTDOWN;
}
