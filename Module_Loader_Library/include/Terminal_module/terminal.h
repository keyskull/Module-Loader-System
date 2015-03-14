/*
 * terminal.h
 *
 *  Created on: 2014年11月19日
 *      Author: key_q
 */



#ifndef __TERMINAL_H__
#define __TERMINAL_H__
#include <time.h>  
#include "User_module\user.h"

typedef struct _running_func_stack{
	const void *const  func;
	struct _func_stack *next;
}running_func_stack;

typedef const struct _Terminal_data {
	const unsigned short int tid;
	const time_t login_date;
	running_func_stack *const running_func;
	User_Info * const user_data;
}Terminal_data;
#if defined(__cplusplus)
extern "C"
{
#endif                          /* __cplusplus */
Terminal_data  *Apply_terminal(User_Info * user);
int Kill_terminal(Terminal_data * terminal);
#if defined(__cplusplus)
}                               /* End of extern "C" */
#endif                          /* __cplusplus */
#endif