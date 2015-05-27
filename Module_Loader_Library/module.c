#include "Module_Loader/base.h"
#include<string.h>

typedef const struct _Module_Info{
	const time_t Create_time;
	const char * const Author_Name;
	const char * const Module_Name;
	const float Version;
}Module_Info;

Module_Info *Create_Module_Handle(char* Author_name, char * Module_Name, float Version){
	const time_t now = time(NULL);
	Module_Info mh = { now, Author_name, Module_Name, Version };
	Module_Info *module_handle = malloc(sizeof(Module_Info));
	memmove(module_handle, &mh, sizeof(Module_Info));
	return module_handle;
}

char * Get_Module_Name(void * Module_Handle){
	Module_Info *module=Module_Handle;
	return memcpy(NULL, module->Module_Name, sizeof(char)*strlen(module->Module_Name));
}
char * Get_Author_Name(void * Module_Handle){
	Module_Info *module = Module_Handle;
	return module->Author_Name;
}
float Get_Author_Version(void * Module_Handle){
	Module_Info *module = Module_Handle;
	return module->Version;
}

Module_Owner *Register_Module_Info(char* Author_name, char * Module_Name, float Version){

	const Module_Owner module_owner = { Create_Module_Handle(Author_name, Module_Name, Version), Get_Module_Name, Get_Author_Name, Get_Author_Version };
	

}