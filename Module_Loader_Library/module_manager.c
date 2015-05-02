#include"Module_Loader/base.h"
#include<string.h>
#include<time.h>


Module_Info module_system_Info;



typedef const struct _Module_handle{
	time_t create_time;
	Receipt (*init)(Module_Owner *owner);
}Module_handle;

typedef struct _module_stack{
	Module_Info *const module_info;
	struct _module_stack * const next;
}module_stack;

typedef struct _Event{
	const void * const func;
	Module_Info *const module_info;
}Event;


static Receipt *const Module_manager(Event *event){

	static module_stack *module_list = NULL;
	static const void *Event_cache = NULL;
	clock_t _clock = clock() + 30000;
	while (Event_cache != NULL)	if (clock()>_clock)return Create_Receipt(event->func,ERROR,&module_system_Info,"time out.");
	Event_cache = event->func;

	if (event->func == Register_Module){

		
	}
	else{
		Event_cache = NULL;
		return Create_Receipt(event->func, ERROR, &module_system_Info, "time out.");
	}
	Event_cache = NULL;
	return Create_Receipt(event->func, SUCCESS, &module_system_Info, "sucess");
}

static Receipt *const _Register_Module(Module_Info *const module_info, Receipt *const(*const func)(Event *event))
{
	Event event = { Register_Module, module_info };
	func(&event);
	return NULL;
}

Receipt *const Register_Module(Module_Info *const module_info){ return _Register_Module(module_info, Module_manager); }

Module_Info Create_Moudle_Info(char *const Author, char *const Module_Name, float Version, Module_stack * const Compatible, Module_stack * const Necessary_Module){
	
	Module_handle *const handle;
	Module_Info module = { handle, Author, Module_Name, Version, Compatible, Necessary_Module };
	return module;
}