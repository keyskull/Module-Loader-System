#include "Module_Loader/base.h"
#include<string.h>

typedef const struct _Compatible_List{
	char* MID;
}Compatible_List;

typedef const struct _Module_Info{
	char* MY_MID;
	time_t Create_time;
	char * const Author_Name;
	char * const Module_Name;
	float Version;
}Module_Info;

typedef struct _Module_Stack{
	Module_Info * module_info;
	struct _Module_Stack * next;
}Module_Stack;
static Module_Stack *_Module_stack = NULL;

char *Check_MID(char* Author_name, char * Module_Name, float Version){
////////////////////////////////////////////////////////////////
	return "test";
}


void *Create_Module_Handle(char* Author_name, char * Module_Name, float Version){
	const time_t now = time(NULL);
	const Module_Info mh = { Check_MID(Author_name, Module_Name, Version), now, Author_name, Module_Name, Version };
	Module_Info *module_handle = malloc(sizeof(Module_Info));
	memmove(module_handle, &mh, sizeof(Module_Info));
	return module_handle;
}

/*system-func*/
Module_Info *Check_Handle(void * Module_Handle){
	if (_Module_stack == NULL)return NULL;
	else{
		Module_Stack * module_stack_head = _Module_stack, *module_stack = module_stack_head;
		do module_stack = module_stack->next;
		while (module_stack->module_info != Module_Handle || module_stack->module_info !=module_stack_head);
		if (module_stack == Module_Handle)return Module_Handle;
		else return NULL;
	}
}


/*end system-func*/

/* 用户的指针函数*/
char * Get_Module_Name(Module_Owner * Module_Owner){
	Module_Info *module = Check_Handle(Module_Owner);
	if (module!=NULL)return module->Module_Name;
	else NULL;
}
char * Get_Author_Name(Module_Owner * Module_Owner){
	Module_Info *module = Check_Handle(Module_Owner);
	if (module != NULL)return module->Author_Name;
	else NULL;
}
float Get_Author_Version(Module_Owner * Module_Owner){
	Module_Info *module = Check_Handle(Module_Owner);
	if (module != NULL)return module->Version;
	else NULL;
}
/*end user-point-func*/
Module_Owner *Register_Module_Info(char* Author_name, char * Module_Name, float Version){
	const Module_Owner _module_owner = { Create_Module_Handle(Author_name, Module_Name, Version), Get_Module_Name, Get_Author_Name, Get_Author_Version };
	Module_Owner *module_owner = malloc(sizeof(Module_Owner));
	memmove(module_owner, &_module_owner, sizeof(Module_Owner));
	return module_owner;
}

Module_Owner *Register_Module_Info_PLUS_Compatible_List(char* Author_name, char * Module_Name, float Version){
	const Module_Owner _module_owner = { Create_Module_Handle(Author_name, Module_Name, Version), Get_Module_Name, Get_Author_Name, Get_Author_Version };
	Module_Owner *module_owner = malloc(sizeof(Module_Owner));
	memmove(module_owner, &_module_owner, sizeof(Module_Owner));
	return module_owner;
}