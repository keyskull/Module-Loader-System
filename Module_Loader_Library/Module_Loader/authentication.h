
#ifndef __AUTHENTICATION_H__
#define __AUTHENTICATION_H__
#include <time.h>  
#include"user.h"

typedef const struct _License{
	const time_t create_time;
	const User_Info *const owner;
}License;

typedef const struct _Validated_License{
	const time_t validation_time;
	License *const license;
}Validated_License;

#endif