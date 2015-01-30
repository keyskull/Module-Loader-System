#include "base.h"
#include "authentication.h"
#include <string.h>




typedef struct _Event{
	void * const func;
	const size_t _Size;
	const Terminal_data * const terminal;
	void *memory_handle;
}Event;


int Memory_manager(Event *event){
	static void **Memory_list=NULL;
	if (event->func == Init_Memory_manger){
		if (Memory_list != NULL)return EXIT_FAILURE;
		else {
			Memory_list = malloc(event->_Size);
			return EXIT_SUCCESS;
		}
	}
	else if (event->func == Apply_Memory){
		void *handle = malloc(event->_Size);
		if (handle == NULL)exit(1);
		else event->memory_handle = handle;
	}
	else if (event->func == Free_Memory){

	}
}
int Init_Memory_manger(const Terminal_data *terminal, size_t _Size){
	
}
void * Apply_Memory(const Terminal_data * terminal, size_t _Size){
	Event event = { Apply_Memory, _Size, terminal,NULL};
	Memory_manager(&event);
	return event.memory_handle;
}

void * Free_Memory(void * memory_handle){
	Event event = {memory_handle};
	return Memory_manager(&event);
}