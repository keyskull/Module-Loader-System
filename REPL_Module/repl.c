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

Repl_data_struct * Get_repl_data_struct(Terminal_data * terminal){
	if (repl_stack != NULL){
		Repl_Stack *stack_head = repl_stack, *_repl_stack = stack_head;
		do{
			if (_repl_stack->repl_data->terminal != terminal)
				_repl_stack = _repl_stack->next;
			else return _repl_stack->repl_data;
		} while (_repl_stack!=stack_head);
	}
	return NULL;
}

CMD_list_stack * Get_cmd_list_stack(Terminal_data * terminal){
	if (repl_stack != NULL){
		Repl_Stack *stack_head = repl_stack, *_repl_stack = stack_head;
		do{
			if (_repl_stack->repl_data->terminal != terminal)
				_repl_stack = _repl_stack->next;
			else return _repl_stack->repl_data->cmd_list_stack;
		} while (_repl_stack != stack_head);
	}
	return NULL;
}


/*start this_Module*/

Receipt * Init_REPL(void){
	Module_Owner * module_owner = Register_Module_Info("Cullen Lee", "Init_REPL", 0.1f);
	module_owner = module_owner->Module_Handle;
	printf("Init Shell Module Success\n");
	return Create_Receipt(Init_REPL, SUCCESS, REPL_module_owner, "Init REPL success!");
}

/*end this_Module*/
char * Not_Found_function(char *args, Repl_data_struct * const repl_data){ return NOT_FOUND_FUNCTION; }
const char *const Get_repl_version(void){ return REPL_VERSION; }


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

Function Find_command(CMD_list_stack *cmd_list,char * command){

	for (int i = 0; i<cmd_list->length; i++)
		if (!strcoll(cmd_list->cmd_list[i]->cmd_name, command))
			return cmd_list->cmd_list[i]->func;
	return (Function)Not_Found_function;
}

int Run_command(Repl_data_struct *repl_data,char * command, const Args_struct *const args){
	if (command[0]!='\0'){
		char * _command = malloc(sizeof(char)*(strlen(command) + 1));
		memmove(_command, command, sizeof(char)*(strlen(command) + 1));
		if (_command[strlen(_command) - 1] == '\r')_command[strlen(_command) - 1] = '\0';
		Function cmd = Find_command(repl_data->cmd_list_stack, _command);
		if (cmd == (Function)Not_Found_function){
			printf("%s command not found \n", _command);
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
		free(_command);
	}
	return EXIT_SUCCESS;
}


int repl(Repl_data_struct *repl_data){
	/***Init***/
	//Init_REPL();
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


Receipt * Apply_shell(Terminal_data *terminal){
	CMD_list_stack *cmd_list_stack = malloc(sizeof(CMD_list_stack));
	cmd_list_stack->cmd_list = NULL;
	cmd_list_stack->length = 0;
	Init_base_command(cmd_list_stack);
	Repl_data_struct * repl_data = malloc(sizeof(Repl_data_struct));
	repl_data->state = 0;
	repl_data->terminal = terminal;
	repl_data->cmd_list_stack = cmd_list_stack;
	Save_repl(repl_data);
	return Create_Receipt(Apply_shell, SUCCESS, REPL_module_owner, "Apply succes");
}

Receipt * Run_shell(Terminal_data *terminal){
	Repl_data_struct * repl_data = Get_repl_data_struct(terminal);
	repl_data->state = 1;
	int i = repl(repl_data);
	if (i == INIT_REPL_ERROR || i == EXIT_FAILURE)return Create_Receipt(Apply_shell, ERROR, REPL_module_owner, "run failed");
	else return Create_Receipt(Run_shell, SUCCESS, REPL_module_owner, "run succes");

}