#include "base.h"
#include "authentication.h"
#include <string.h>




typedef struct _Event{ 
	void * const func;
	const size_t _Size;
	const Terminal_data * const terminal;
	void *memory_handle;
}Event;

int Init_Memory_manger(const Terminal_data *terminal, size_t _Size);

int Memory_manager(Event *event){
	static void **Memory_list=NULL;
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
	else return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
int Init_Memory_manger(const Terminal_data *terminal, size_t _Size){
	Event event = { Init_Memory_manger,_Size,terminal,NULL };
	return Memory_manager(&event);
}
void * Apply_Memory(const Terminal_data * terminal, size_t _Size){
	Event event = { Apply_Memory, _Size, terminal,NULL};
	Memory_manager(&event);
	return event.memory_handle;
}

int Free_Memory(const Terminal_data * terminal, void * memory_handle){
	Event event = { Free_Memory, 0, terminal, memory_handle };
	return Memory_manager(&event);
}