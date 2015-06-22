#include "Module_Loader/base.h"

static Module_Owner * My_module_owner = NULL;
VM_stack* Create_VM(JavaVMOption options[], int options_len, char * Main_ClassName);