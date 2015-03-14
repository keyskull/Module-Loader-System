#include "Module_Loader\base.h"
#include "User_module\authentication.h"
#include "Terminal_module\terminal.h"
#include "Module_Loader\memory.h"
#include <string.h>




typedef struct _Event{ 
	void * const func;
	const size_t _Size;
	Terminal_data * const terminal;
	void *memory_handle;
}Event;

int Init_Memory_manger(Terminal_data *terminal, size_t _Size);

static int Memory_manager(Event *event){
	static void **Memory_list=NULL;
	static void *Event_cache = NULL;
	clock_t _clock = clock() + 30000;
	while (Event_cache != NULL)	if (clock()>_clock)return EXIT_TIMEOUT;
	Event_cache = event->func;
	if (event->func == Init_Memory_manger){
		if (Memory_list != NULL)return EXIT_FAILURE;
		else if ((Memory_list = malloc(event->_Size)) == NULL)return EXIT_FAILURE;
	}
	else if (event->func == Apply_Memory){
		void *handle = malloc(event->_Size);
		if (handle == NULL)exit(1);
		else event->memory_handle = handle;
	}
	else if (event->func == Free_Memory){

	}
	else {
		Event_cache = NULL;
		return EXIT_FAILURE;
	}
	Event_cache = NULL;
	return EXIT_SUCCESS;
}
int Init_Memory_manger(Terminal_data *terminal, size_t _Size){
	Event event = { Init_Memory_manger,_Size,terminal,NULL };
	return Memory_manager(&event);
}
void * Apply_Memory(Terminal_data * terminal, size_t _Size){
	Event event = { Apply_Memory, _Size, terminal,NULL};
	Memory_manager(&event);
	return event.memory_handle;
}

int Free_Memory(Terminal_data * terminal, void * memory_handle){
	Event event = { Free_Memory, 0, terminal, memory_handle };
	return Memory_manager(&event);
}