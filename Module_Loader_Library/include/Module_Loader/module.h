#include "base.h"

typedef struct _module_stack module_stack;
typedef const struct _module_struct module_struct;

typedef const struct _module_struct{
	char *const module_name;
	const module_stack * const Execution_Conditions;
}module_struct;

typedef struct _module_stack{
	module_struct module;
	struct _module_stack * const next;
}module_stack;

Receipt *const Add_module(module_struct *const module);
Receipt *const Remove_module(module_struct *const module);