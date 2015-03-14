#include "Module_Loader\base.h"
#include "Module_Loader\terminal.h"
#include "Module_Loader\apply.h"
#include <windows.h> 


_declspec(dllexport) int Default_run(void){

	const User_Info use = { 0, "key_q" };
	Terminal_data *ter = Apply_terminal(&use);
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