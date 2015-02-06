#include "base.h"
#include<string.h>
#include "terminal.h"

typedef struct _terminal_stack{
	Terminal_data * const terminal;
	struct _terminal_stack *next;
}Terminal_stack;

typedef struct _Event{
	const void * const func;
	const User_Info * const user_info;
	const Terminal_data * terminal;
}Event;

int Terminal_manager(Event *event){
	static void *Event_cache = NULL;
	static Terminal_stack *terminal_list = NULL;
	clock_t _clock = clock() + 30000;
	while (Event_cache != NULL)	if (clock()>_clock)return EXIT_TIMEOUT;
	Event_cache = event->func;

	if(event->func == Apply_terminal){
		if(event->user_info != NULL){
			unsigned int tid = 0;
			Terminal_stack *t = terminal_list, *new_struct = malloc(sizeof(Terminal_stack));
			if(terminal_list != NULL){
				do ++tid;
				while((t = t->next) != terminal_list);
			}
			else{
				tid = 0;
				new_struct->next = new_struct;
				terminal_list = new_struct;
			}
			const time_t now = time(NULL);
			const Terminal_data _terminal = { tid, now, event->user_info };
			Terminal_data *terminal = malloc(sizeof(Terminal_data));
			memcpy(terminal, &_terminal, sizeof(Terminal_data));
			Terminal_stack new_struct_s = { (Terminal_data * const)terminal, terminal_list->next };
			memcpy(new_struct, &new_struct_s, sizeof(Terminal_stack));
			terminal_list->next = new_struct;
			event->terminal = new_struct->terminal;
		}
	}
	else if (event->func == Kill_terminal){
		Terminal_stack *ter_cache = terminal_list;
		while (ter_cache->next->terminal != event->terminal){
			if (ter_cache->next != terminal_list)ter_cache = ter_cache->next; 
			else break;
		}
		if (ter_cache->next->terminal == event->terminal){
			Terminal_stack * t = ter_cache->next;
			ter_cache->next = t->next;
			free(t);
		}
	}
		else return EXIT_FAILURE;
	Event_cache=NULL;
	return EXIT_SUCCESS;
}

Terminal_data * Apply_terminal(const User_Info * user){
	Event event = { Apply_terminal, user, NULL };
	Terminal_manager(&event);
	return event.terminal;
}

int Kill_terminal(const Terminal_data * terminal){
	Event event = { Kill_terminal, NULL, terminal };
	return Terminal_manager(&event);
}