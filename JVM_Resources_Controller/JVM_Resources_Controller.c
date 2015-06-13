#include "Module_Loader/base.h"
#include"control_fun.h"
#include <Windows.h>

JNIEXPORT jint JNICALL intMethod
(JNIEnv *a, jobject b, jint c){
	printf("test ssss%d", c);
	return c;
}

static Module_Owner * module_owner = NULL;

/*Ä£¿éÈë¿Ú*/
int Create_JVM_Management(){
	SetEnvironmentVariable("_ALT_JAVA_HOME_DIR", "C:\\cygwin64\\home\\Jane\\jdk\\openjdk9\\build\\windows-x86_64-normal-server-release\\images\\jdk");
	JavaVMOption options[1];
	options[0].optionString = "-Djava.class.path=./scala-library.jar;./scala-reflect.jar;.";
	JavaVM * vm = Create_VM(options, "JVM_Management");
	JNI_Class_Stack *js = alloc_JNI_Class_Stack(vm, "JVM_Management_Library");
	js->add_Methods(js, (JNINativeMethod){ "intMethod", "(I)I", intMethod });
	printf("%s", js->Methods[0]->name);
	return EXIT_SUCCESS;
}


/*start this_Module*/
void * Get_This_Model_Owner_Key(){ return module_owner; }


Receipt * Init_JVM_Resources_Controller(){
	Module_Owner * module_owner=Register_Module_Info("Cullen Lee", "JVM_Resources_Controller", 0.1f);
	module_owner = module_owner->Module_Handle;
	return Create_Receipt(Init_JVM_Resources_Controller, SUCCESS, module_owner, "Init JVM Resources Controller success!");
}


/*end this_Module*/