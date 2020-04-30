#pragma once

#define RET_ON_FALSE(condition, retValue) \
	if(!(condition))\
	{\
		return retValue;\
	}

#define RET_ON_TRUE(condition, retValue) \
	if(condition)\
	{\
		return retValue;\
	}
