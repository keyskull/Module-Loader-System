/*
 * Default_run.c
 *
 *  Created on: 2014年11月19日
 *      Author: key_q
 */

#include "base.h"
#include "terminal.h"
#include "apply.h"
#include <windows.h> 


_declspec(dllexport) int Default_run(void){

	User_Info _use = { 0, "key_q" };
	User_Info *const use = malloc(sizeof(User_Info));
	memmove(use, &_use, sizeof(User_Info));
	Terminal_data *ter = Apply_terminal(use);
	Apply_repl(ter);

#pragma omp parallel
	{
#pragma omp master
		{
	
			//printf("test2");
			//run_terminal_controller();
		}
#pragma omp single
		{
		/*
		User_Info use = { 0, (unsigned char *)"key_q" };
		const Terminal_data *terminal = Apply_terminal(use);
		printf("%s", terminal->login_date);
		Apply_repl(terminal);
		*/
		}
		
	}
	

	return EXIT_SUCCESS;
}