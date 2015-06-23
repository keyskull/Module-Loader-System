
#ifndef __REPL_H__
#define __REPL_H__
#include "Terminal_module/terminal.h"
#define REPL_VERSION "1.0"
#define NOT_FOUND_FUNCTION ((char *)0)
#define INIT_REPL_ERROR 2

typedef struct _Args_struct Args_struct;
typedef struct _Repl_data_struct Repl_data_struct;
typedef char *(*Function)(const Args_struct *const args, Repl_data_struct * const repl_data);


/** cmd_list_struct**/
typedef struct _Args_struct{
	short int length;
	char *const* args;
}Args_struct;
typedef struct _CMD_struct{
	 char * cmd_name;
	 Function func;
} CMD_struct;
typedef struct _CMD_list_stack{
	CMD_struct ** cmd_list;
	unsigned short int length;
} CMD_list_stack;
/**end cmd_list_struct **/

/** repl struct**/
typedef struct _Command_struct{
	char * const command;
	Args_struct * const args;
}Command_struct;
typedef struct _Repl_data_struct{
	_Bool state;
	Terminal_data * terminal;
	CMD_list_stack *cmd_list_stack;
}Repl_data_struct;
/**end repl struct**/
#if defined(__cplusplus)
extern "C"
{
#endif                          /* __cplusplus */

char * Not_Found_function(char *args, Repl_data_struct * const repl_data);//return NOT_FOUND_FUNCTION
const char *const Get_repl_version(void);
int Init_base_command(CMD_list_stack *cmd_list);
Command_struct *Control(void);
int Add_command(CMD_list_stack *cmd_list,char *cmd_name,Function func);
CMD_list_stack * Get_cmd_list_stack(Terminal_data * terminal);
Receipt * Init_REPL(void);
Receipt * Apply_shell(Terminal_data *terminal);
#if defined(__cplusplus)
}                               /* End of extern "C" */
#endif                          /* __cplusplus */

#endif
