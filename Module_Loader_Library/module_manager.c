#include"Module_Loader\base.h"
#include"Module_Loader\module.h"
#include<string.h>
#include<time.h>



typedef struct _Event{
	const void * const func;
	const module_struct * const module;
}Event;

/*需要修改成同时多个管理器*/

static Receipt *const Module_manager(Event *event){

	static module_stack *module_list = NULL;
	static const void *Event_cache = NULL;
	clock_t _clock = clock() + 30000;
	while (Event_cache != NULL)	if (clock()>_clock)return EXIT_TIMEOUT;
	Event_cache = event->func;

	if (event->func == Add_module){

		
	}
	else if (event->func == Remove_module){
	
	}
	else{
		Event_cache = NULL;
		return EXIT_FAILURE;
	}
	Event_cache = NULL;
	return EXIT_SUCCESS;
}

static Receipt *const _Add_module(const module_struct *const module, Receipt *const(*const func)(Event *event))
{
	Event event = { Add_module, module };
	func(&event);
	return NULL;
}

static Receipt *const _Remove_module(const module_struct *const module, Receipt *const(*const func)(Event *event))
{
	Event event = { Remove_module, NULL };
	return func(&event);
}


Receipt *const Add_module(const module_struct *const module){ return _Add_module(module, Module_manager); }
Receipt *const Remove_module(const module_struct *const module){ return _Remove_module(module, Module_manager); }