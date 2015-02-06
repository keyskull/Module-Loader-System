
#include "base.h"
#include "repl.h"
#include "terminal.h"
#include <string.h>




int Apply_repl(Terminal_data *terminal){
	Repl_data_struct *repl_data = malloc(sizeof(Repl_data_struct));
	repl_data->terminal = terminal;
	repl(repl_data);
	free(repl_data);
	return EXIT_SUCCESS;
}

