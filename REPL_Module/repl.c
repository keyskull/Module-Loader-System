#include "Module_Loader/base.h"
#include "REPL_module/repl.h"
#include "Terminal_module/terminal.h"
#include"handle.h"
#include <string.h>
#include <time.h>


typedef struct Repl_data_struct_Stack{
	Repl_data_struct * repl_data;
	struct Repl_data_struct_Stack *next;
}Repl_Stack;

Repl_Stack *repl_stack = NULL;

Receipt * Save_repl(Repl_data_struct * repl_data){
	Repl_Stack *_repl_stack = malloc(sizeof(Repl_Stack));
	_repl_stack->repl_data = repl_data;
	if (repl_stack == NULL){
		repl_stack = _repl_stack;
		repl_stack->next = repl_stack;
	}
	else{
		_repl_stack->next = repl_stack;
		repl_stack->next = _repl_stack;
	}
	return Create_Receipt(Save_repl, SUCCESS, REPL_module_owner, "Init Save Shell success!");
}



/*start this_Module*/

Receipt * Init_REPL(){
	Module_Owner * module_owner = Register_Module_Info("Cullen Lee", "Init_REPL", 0.1f);
	module_owner = module_owner->Module_Handle;
	return Create_Receipt(Init_REPL, SUCCESS, REPL_module_owner, "Init REPL success!");
}

/*end this_Module*/
char * Not_Found_function(char *args, Repl_data_struct * const repl_data){ return NOT_FOUND_FUNCTION; }
const char *const Get_repl_version(void){ return REPL_VERSION; }

static int Init_repl(Repl_data_struct *repl_data){
	if (!repl_data)return EXIT_FAILURE;
	else if (!repl_data->terminal)return EXIT_FAILURE;
	else if (!repl_data->terminal->user_data)return EXIT_FAILURE;
	repl_data->state = 1;
	return EXIT_SUCCESS;
}

int Add_command(CMD_list_stack *cmd_list_stack, char *cmd_name, Function func){
	char *_cmd_name = malloc(sizeof(char)*(strlen(cmd_name)+1));
	memmove(_cmd_name, cmd_name, (sizeof(char)*strlen(cmd_name)+1));
	const CMD_struct cmd = { _cmd_name, func };
	if (cmd_list_stack->cmd_list != NULL){
		CMD_struct ** cmd_struct = malloc(sizeof(CMD_struct *) *(cmd_list_stack->length+1));
		memmove(cmd_struct, cmd_list_stack->cmd_list, sizeof(CMD_struct *) *cmd_list_stack->length);
		cmd_struct[cmd_list_stack->length] = malloc(sizeof(CMD_struct));
		memmove(cmd_struct[cmd_list_stack->length], &cmd, sizeof(CMD_struct));
		cmd_list_stack->cmd_list = cmd_struct;
		cmd_list_stack->length++;
	}
	else {
		CMD_struct ** cmd_struct = malloc(sizeof(CMD_struct *));
		cmd_struct[cmd_list_stack->length]= malloc(sizeof(CMD_struct));
		memmove(cmd_struct[cmd_list_stack->length], &cmd, sizeof(CMD_struct));
		cmd_list_stack->cmd_list = cmd_struct;
		cmd_list_stack->length=1;
	}
	return EXIT_SUCCESS;
}

static Function Find_command(CMD_list_stack *cmd_list,char * command){

	for (int i = 0; i<cmd_list->length; i++)
		if (!strcoll(cmd_list->cmd_list[i]->cmd_name, command))
			return cmd_list->cmd_list[i]->func;
	return (Function)Not_Found_function;
}

static int Run_command(Repl_data_struct *repl_data,char * command, const Args_struct *const args){
	if (command){
		Function cmd = Find_command(repl_data->cmd_list_stack, command);
		if (cmd == (Function)Not_Found_function){
			printf("%s command not found \n", command);
			return EXIT_FAILURE;
		}
		else {

			char *c = cmd(args, repl_data);
			if (c)printf("%s\n", c);
			int i = strlen(c) + 1;
			char *cache = malloc(i);
			memmove(cache, c, i);
			free(cache);
		}
	}
	return EXIT_SUCCESS;
}


static int repl(Repl_data_struct *repl_data){
	/***Init***/
	if (Init_repl(repl_data) == EXIT_FAILURE)return INIT_REPL_ERROR;
	/***end Init***/
	while (repl_data->state){
		printf("%s >", repl_data->terminal->user_data->user_name);
		Command_struct *cmd = Control();
		Run_command(repl_data, cmd->command, cmd->args);
		free(cmd->command);
		free(cmd->args);
		free(cmd);
	}

	/** over **/


	/** over done**/
	return EXIT_SUCCESS;
}


Receipt *const Apply_shell(Terminal_data *terminal){
	CMD_list_stack *cmd_list_stack = malloc(sizeof(CMD_list_stack));
	cmd_list_stack->cmd_list = NULL;
	cmd_list_stack->length = 0;
	Init_base_command(cmd_list_stack);
	Repl_data_struct * repl_data = malloc(sizeof(Repl_data_struct));
	repl_data->state = 0;
	repl_data->terminal = terminal;
	repl_data->cmd_list_stack = cmd_list_stack;
	Save_repl(repl_data);
	int i = repl(repl_data);
	if (i == INIT_REPL_ERROR || i == EXIT_FAILURE)
		return Create_Receipt(Apply_shell, ERROR, REPL_module_owner, "run fall");
	else return Create_Receipt(Apply_shell, SUCCESS, REPL_module_owner, "run succes");
}
