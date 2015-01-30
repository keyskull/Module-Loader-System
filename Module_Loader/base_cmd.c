#include "base.h"
#include<string.h>
#include "repl.h"
#include "apply.h"



char * base_su(const Args_struct *const args, Repl_data_struct * const repl_data){
	if (args->length>0){
		const User_Info _use = { 0, (unsigned char *)args->args[0] };
		User_Info *const use = malloc(sizeof(User_Info));
		memmove(use, &_use, sizeof(User_Info));
		Terminal_data *ter = Apply_terminal(use);
		Apply_repl(ter);
	}
	else return "you should be enter login name.";
	return NULL;
}


char *base_module(const Args_struct *const args, Repl_data_struct * const repl_data){

	return "load success";
}

char * base_time(const Args_struct *const args, Repl_data_struct * const repl_data){
	return Get_date("%Y/%m/%d %H:%M:%S");
}

char *base_help(const Args_struct *const args, Repl_data_struct * const repl_data){
	int i = 0;
	char *c = calloc(INIT_CACHE_SIZE, sizeof(char));
	strcpy(c, "this is Mylib help command\n");
	while(repl_data->public_cmd_list[i]!=NULL){
		strcat(c,(char *)repl_data->public_cmd_list[i]);
		if (repl_data->public_cmd_list[i + 1] != NULL)strcat(c, "\n");
		i++;
	}

	return c;
}

char * base_exit(const Args_struct *const args, Repl_data_struct * const repl_data)
{
	repl_data->state = 0;
	Kill_terminal(repl_data->terminal);
	return "exit success";
}

int Add_command_help_document(){
	return EXIT_SUCCESS;
}
int Init_base_command(CMD_list_stack *cmd_list_stack)
{
	Add_command(cmd_list_stack, "su", base_su);
	Add_command(cmd_list_stack, "model", base_module);
	Add_command(cmd_list_stack, "time", base_time);
	Add_command(cmd_list_stack, "help", base_help);
	Add_command(cmd_list_stack, "exit", base_exit);
	return EXIT_SUCCESS;
}

