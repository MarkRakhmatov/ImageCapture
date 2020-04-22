#pragma once

#define RET_ON_FAIL(condition, retValue) \
	if(!(condition))\
	{\
		return retValue;\
	}

#define RET_ON_SUCCESS(condition, retValue) \
	if(condition)\
	{\
		return retValue;\
	}
