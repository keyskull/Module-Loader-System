/*
 * control.c
 *
 *  Created on: 2014年11月14日
 *      Author: key_q
 */


#include "Module_Loader\base.h"
#include <string.h>
#include "Module_Loader\repl.h"



Args_struct * const Analysis_args(char *args){
	Args_struct * const args_struct = malloc(sizeof(Args_struct));
	if (args == NULL){
		args_struct->length = 0;
		args_struct->args = NULL;
		return args_struct;
	}
	int n = 0;
	while (args[n] != '\0')n++;
	if (args[n - 1] == '\n')args[n - 1] = '\0';
	char ** _args = Tokenizer(args,' ');
	int i = 0;
	if (_args != NULL)while (_args[i] != NULL)i++;
	const Args_struct _args_struct = { i ,_args};
	memmove(args_struct, &_args_struct, sizeof(Args_struct));
	printf("%d\n", args_struct->length);//test
	for (int n = 0; n < i; n++)printf("%s\n", args_struct->args[n]);//test
	return args_struct;
}
Command_struct *Analysis_command(char *command){

	int len = strlen(command);
	char * real_cmd = calloc(len, sizeof(char));
	char * args = NULL;
	for (int i = 0; i<len; i++)
		switch (command[i]){
			case ' ':
				i++;
				args = &command[i];
				i = (int)strlen(command);
				break;
			case '\0':break;
			case '\n':real_cmd[i] = '\0'; break;
			default:
				real_cmd[i] = command[i]; break;
		}
	Args_struct *const _args = Analysis_args(args);
	Command_struct _cmd_struct = { real_cmd, _args }, *cmd_struct = malloc(sizeof(Command_struct));
	memmove(cmd_struct, &_cmd_struct, sizeof(Command_struct));
	
	return cmd_struct;
}

Command_struct *Control(void){
	char * command = malloc(CMD_CACHE*sizeof(char));
	memset(command, '\0', CMD_CACHE*sizeof(char));
	fgets(command, CMD_CACHE*sizeof(char), stdin);
	int i = 0;
	while(strstr( command,"\n")==NULL){
		++i;
		char _command[CMD_CACHE];
		fgets(_command, CMD_CACHE*sizeof(char), stdin);
		char *__command = malloc((i+1)*CMD_CACHE*sizeof(char));
		strcpy(__command,command);
		strcat(__command, _command);
		free(command);
		command = __command;
	}
	Command_struct * cs = Analysis_command(command);
	free(command);
	return cs;
}
