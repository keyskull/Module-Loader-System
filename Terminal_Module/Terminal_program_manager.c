#include"Module_Loader/base.h"
#include"Terminal_module/terminal.h"
#include<omp.h>
#include<string.h>
typedef Terminal_Receipt*const(*Program)(Terminal_data *terminal);
typedef const struct _Runing_program{
	Receipt * const Receipt;
	Terminal_data * const terminal;
	int PID;
	int status;
}Runing_program;
typedef struct _Program_stack{
	Runing_program * const Program;
	struct _Program_stack *next;
}Program_stack;
static Program_stack * Program_list=NULL;

Runing_program* Mark_program(Terminal_data * const terminal, Program func){
	Runing_program * const program = malloc(sizeof(Runing_program));
	if (Program_list == NULL){
		Runing_program program_1 = { Create_Receipt(func, SUCCESS, "run succes"), terminal, 0, rand() };
		memmove(program, &program_1, sizeof(Runing_program));
	}
	else{
		Program_stack *i=Program_list;
		while (i->next != NULL)i = i->next;
		Runing_program program_1 = { Create_Receipt(func, SUCCESS, "run succes"), terminal, i->Program->PID + 1, 1 };
		memmove(program, &program_1, sizeof(Runing_program));
	}
	return program;
}

Receipt* Kill_program(Terminal_data * const terminal, const int pid, Receipt* father){
	if (Program_list != NULL){
		Program_stack *i = Program_list,*ii=i; 
		Receipt * receipt=NULL;
		while (i->next != NULL){
			if (i->Program->PID == pid){
				ii->next = i->next;
				free(i);
				receipt = Create_Receipt_extends(Kill_program, SUCCESS, father, "kill program succes.");
				break;
			}
			else{
				ii = i;
				i = i->next;
			}
		}
		if (receipt == NULL) return Create_Receipt(Kill_program, ERROR, "kill program failed.");
		else return receipt;
	}
	else return Create_Receipt(Kill_program, WARRING, "Program list is empty");
}
Receipt* Run_program(Terminal_data * const terminal, Program func){
	Runing_program * program=Mark_program(terminal, func);
	Receipt  * receipt,*kill;
	receipt = func(terminal);
	kill = Kill_program(terminal, program->PID, receipt);
	return kill;


}