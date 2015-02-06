
#ifndef __AUTHENTICATION_H__
#define __AUTHENTICATION_H__
#include <time.h>  
#include"user.h"

typedef struct _License{
	const time_t create_time;
	const User_Info *const owner;
}License;

typedef struct _Validated_License{
	const time_t validation_time;
	const License *const license;
}Validated_License;

void * Apply_Memory(const Terminal_data *terminal, size_t _Size);
int Free_Memory(const Terminal_data * terminal, void * memory_handle);
#endif