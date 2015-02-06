#include "base.h"
#include<string.h>
#include <time.h>
#include "repl.h"



char * Not_Found_function(char *args, Repl_data_struct * const repl_data){ return NOT_FOUND_FUNCTION; }
const char *const Get_repl_version(void){return REPL_VERSION;}

int Init_repl(Repl_data_struct *repl_data){
	if (!repl_data)return EXIT_FAILURE;
	else if(!repl_data->terminal)return EXIT_FAILURE;
	else if (!repl_data->terminal->user_data)return EXIT_FAILURE;
	repl_data->cmd_list_stack = malloc(sizeof(CMD_list_stack));
	repl_data->cmd_list_stack->cmd_list = calloc(INIT_CACHE_SIZE / 2, sizeof(CMD_stack *));
	repl_data->cmd_list_stack->length = 0;
	Init_base_command(repl_data->cmd_list_stack);

	repl_data->state = 1;
	repl_data->public_cmd_list = calloc(repl_data->cmd_list_stack->length, sizeof(char *));
	for (int i = 0; i <(int)repl_data->cmd_list_stack->length; i++){
		repl_data->public_cmd_list[i] = calloc(strlen((const char *)repl_data->cmd_list_stack->cmd_list[i]->cmd_name) + 1, sizeof(unsigned char));
		strcpy((char *)repl_data->public_cmd_list[i], (const char *)repl_data->cmd_list_stack->cmd_list[i]->cmd_name);
		repl_data->public_cmd_list[i + 1] = NULL;
	}

	return EXIT_SUCCESS;
}

int Add_command(CMD_list_stack *cmd_list_stack, char *cmd_name, Function func){
	if (cmd_list_stack->cmd_list != NULL){
		cmd_list_stack->cmd_list[cmd_list_stack->length] = malloc(sizeof(CMD_stack));
		cmd_list_stack->cmd_list[cmd_list_stack->length]->cmd_name = (unsigned char *)cmd_name;
		cmd_list_stack->cmd_list[cmd_list_stack->length]->func = func;
		cmd_list_stack->length++;
	}
	else return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

Function Find_command(CMD_list_stack *cmd_list, const unsigned char * const command){

	for (int i = 0; i<(int)cmd_list->length; i++){
		if (!strcoll((char *)cmd_list->cmd_list[i]->cmd_name, command)){
			return cmd_list->cmd_list[i]->func;
		}
	}

	return (Function)Not_Found_function;
}

int Run_command(Repl_data_struct *repl_data,const unsigned char *const command,const Args_struct *const args){
	if (command){
		Function cmd = Find_command(repl_data->cmd_list_stack, command);
		if (cmd == (Function)Not_Found_function)return EXIT_FAILURE;
		else {
			char *c = cmd(args, repl_data);
			if(c)printf("%s\n", c);
			if ((void *)c > (void *)"const_mark")free(c);
		}
	}
	return EXIT_SUCCESS;
}

int Free_repl(Repl_data_struct *repl_data){
	free(repl_data->cmd_list_stack->cmd_list);
	free(repl_data->cmd_list_stack);
	free(repl_data);
	return EXIT_SUCCESS;
}

int repl(Repl_data_struct *repl_data){

	/***Init***/
	if (Init_repl(repl_data) == EXIT_FAILURE)return INIT_REPL_ERROR;
	//if (repl_data->terminal->user_data->last_login_date != NULL)printf("last login date: %s\n", repl_data->terminal->user_data->last_login_date);
	/***end Init***/
	while (repl_data->state){
		printf("%s >", repl_data->terminal->user_data->user_name);
		Command_struct *cmd = Control();
		if (Run_command(repl_data,
			cmd->command,
			cmd->args) != EXIT_SUCCESS)
			printf("%s command not found \n", cmd->command);
		free(cmd->command);
		free(cmd->args);
		free(cmd);
	}

	/** over **/

	Free_repl(repl_data);
	/** over done**/
	return EXIT_SUCCESS;
}


