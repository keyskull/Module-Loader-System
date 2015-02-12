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
	repl_data->state = 1;
	return EXIT_SUCCESS;
}

int Add_command(CMD_list_stack *cmd_list_stack, char *cmd_name, Function func){
	if (cmd_list_stack->cmd_list != NULL){
		const CMD_struct cmd = { cmd_name, func };
		memmove(&(cmd_list_stack->cmd_list[cmd_list_stack->length]), &cmd, sizeof(CMD_struct));
		cmd_list_stack->length++;
	}
	else return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

Function Find_command(CMD_list_stack cmd_list, const unsigned char * const command){

	for (int i = 0; i<(int)cmd_list.length; i++){
		if (!strcoll((char *)cmd_list.cmd_list[i].cmd_name, command)){
			return cmd_list.cmd_list[i].func;
		}
	}
	return (Function)Not_Found_function;
}

int Run_command(Repl_data_struct *repl_data,const unsigned char *const command,const Args_struct *const args){
	if (command){
		Function cmd = Find_command(repl_data->cmd_list_stack, command);
		if (cmd == (Function)Not_Found_function){
			printf("%s command not found \n",command);
			return EXIT_FAILURE;
		}
		else {
			char *c = cmd(args, repl_data);
			if(c)printf("%s\n", c);
			if ((void *)c > (void *)"const_mark")free(c);
		}
	}
	return EXIT_SUCCESS;
}

int Free_repl(Repl_data_struct *repl_data){
	//free(repl_data->cmd_list_stack.cmd_list);
	return EXIT_SUCCESS;
}

int repl(Repl_data_struct *repl_data){

	/***Init***/
	if (Init_repl(repl_data) == EXIT_FAILURE)return INIT_REPL_ERROR;
	/***end Init***/
	while (repl_data->state){
		printf("%s >", repl_data->terminal->user_data->user_name);
		Command_struct *cmd = Control();
		Run_command(repl_data,cmd->command,cmd->args);
		free(cmd->command);
		free(cmd->args);
		free(cmd);
	}

	/** over **/

	Free_repl(repl_data);
	/** over done**/
	return EXIT_SUCCESS;
}


int Apply_repl(const Terminal_data *terminal){
	
	CMD_struct * cmd_struct=malloc(INIT_CACHE_SIZE/2*sizeof(CMD_struct));//need update
	CMD_list_stack cmd_list_stack = { cmd_struct, 0 };
	Init_base_command(&cmd_list_stack);
	Repl_data_struct repl_data = { 0, terminal, cmd_list_stack };
	int i = repl(&repl_data);
	free(cmd_struct);
	//for (int i = 0; i < cmd_list_stack.length;i++)free(cmd_list_stack.cmd_list[i]);
	if (i == INIT_REPL_ERROR || i == EXIT_FAILURE)return EXIT_FAILURE;
	else return EXIT_SUCCESS;
}

