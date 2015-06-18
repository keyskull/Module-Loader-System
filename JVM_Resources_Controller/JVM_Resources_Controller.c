#include "Module_Loader/base.h"
#include"JVM_Resources_Controller/control_fun.h"
#include"handle.h"
JNIEXPORT jint JNICALL intMethod
(JNIEnv *a, jobject b, jint c){
	printf("test ssss%d", c);
	return c;
}


/*模块入口*/
int Create_JVM_Management(){
	JavaVMOption options[1];
	options[0].optionString = "-Djava.class.path=./scala-library.jar;./scala-reflect.jar;.";
	VM_stack * vm = Create_VM(options, "JVM_Management");
	JNI_Class_Stack *js = alloc_JNI_Class_Stack("JVM_Management_Library");
	js->add_Methods(js, (JNINativeMethod){ "intMethod", "(I)I", intMethod });
	vm->Run_Jni_Onload;//装载JNI函数
	vm->Run_main_method;//运行JVM 管理模块
	printf("%s", js->Methods_List[0]->name);
	return EXIT_SUCCESS;
}


/*start this_Module*/


Receipt * Init_JVM_Resources_Controller(){
	Module_Owner * module_owner = Register_Module_Info("Cullen Lee", "JVM_Resources_Controller", 0.1f);
	My_module_owner = module_owner->Module_Handle;
	return Create_Receipt(Init_JVM_Resources_Controller, SUCCESS, My_module_owner, "Init JVM Resources Controller success!");
}


/*end this_Module*/