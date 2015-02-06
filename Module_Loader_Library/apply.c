
#include "base.h"
#include "repl.h"
#include "terminal.h"
#include <string.h>




int Apply_repl(Terminal_data *terminal){
	Repl_data_struct *repl_data = malloc(sizeof(Repl_data_struct));
	repl_data->terminal = terminal;
	int i = repl(repl_data);
	if(i == INIT_REPL_ERROR ||i == EXIT_FAILURE)return EXIT_FAILURE;
	else return EXIT_SUCCESS;
}

