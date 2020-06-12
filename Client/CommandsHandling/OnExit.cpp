#include "OnExit.h"
#include "Command.h"

namespace ClientSide
{
	using namespace Communication;
	EConnectionStatus
	OnExit::Handle(Socket& sock)
	{
	  return EConnectionStatus::SHUTDOWN;
	}
}
