#include "Utils.h"

ECheckOperation
CheckIoctl (int val)
{
  if(val == 0)
  {
      return ECheckOperation::SUCCESS;
  }
  if(errno == EAGAIN)
  {
      return ECheckOperation::IN_PROGRESS;
  }
  return ECheckOperation::FAIL;
}

ECheckOperation
CheckOpenFile (int descriptor)
{
  if(descriptor >= 0)
  {
      return ECheckOperation::SUCCESS;
  }
  if(errno == EAGAIN)
  {
      return ECheckOperation::IN_PROGRESS;
  }
  return ECheckOperation::FAIL;
}
