#include"Module_Loader/base.h"
#include"Terminal_module/terminal.h"
#include"handle.h"


/*start this_Module*/



Receipt * Init_Terminal(){
	Module_Owner * module_owner = Register_Module_Info("Cullen Lee", "Init_Terminal", 0.1f);
	Terminal_module_owner = module_owner->Module_Handle;
	printf("Init Terminal Module Success.\n");
	return Create_Receipt(Init_Terminal, SUCCESS, Terminal_module_owner, "Init Terminal success!");
}

/*end this_Module*/




Terminal_Receipt * Create_Terminal_Receipt(){

}

