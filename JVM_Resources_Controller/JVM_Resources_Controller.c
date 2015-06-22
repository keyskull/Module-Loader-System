#include "Module_Loader/base.h"
#include "Terminal_module/terminal.h"
#include "Module_Loader/apply.h"
#include"JVM_Resources_Controller/control_fun.h"
#include "REPL_module/repl.h"
#include"handle.h"

Terminal_data *ter = NULL;
/*Module_Loader Jni_Class*/

JNIEXPORT jboolean JNICALL Java_cn_Module_1Loader_Have_1Module
(JNIEnv *env, jobject obj, jstring s){
	char* rtn = NULL;
	jclass clsstring = (*env)->FindClass(env,"java/lang/String");
	jstring strencode = (*env)->NewStringUTF(env,"utf-8");
	jmethodID mid = (*env)->GetMethodID(env,clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray)(*env)->CallObjectMethod(env,s, mid, strencode);
	jsize alen = (*env)->GetArrayLength(env,barr);
	jbyte* ba = (*env)->GetByteArrayElements(env,barr, JNI_FALSE);
	if (alen > 0){
		rtn = (char*)malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	(*env)->ReleaseByteArrayElements(env,barr, ba, 0);
	My_module_owner->Have_Module(rtn);
}
/*end Module_Loader Jni_Class*/
/*Shell_Module Jni_Class*/

JNIEXPORT jboolean JNICALL Java_cn_Shell_1Module_Run_1REPL
(JNIEnv * env, jobject obj){
	Apply_shell(ter);
}

JNIEXPORT jboolean JNICALL Java_cn_Shell_1Module_Init_1REPL
(JNIEnv * env, jobject obj){
	User_Info use = { 0, "key_q" };
	ter = Apply_terminal(&use);
}


JNIEXPORT jboolean JNICALL Java_cn_Shell_1Module_Add_1Command
(JNIEnv * env, jobject obj, jstring cmd, jstring classname, jstring func_name){
	Function func();
	Add_command(ter->running_program, cmd, func);

}
/*end Shell_Module Jni_Class*/
/*模块入口*/
Receipt * Create_JVM_Management(void){
	JavaVMOption options[1];
	options[0].optionString = "-Djava.class.path=./scala-library.jar;./Module_loader_System.jar";
	VM_stack * vm = Create_VM(options,1, "JVM_Management");
	JNI_Class_Stack *jc = alloc_JNI_Class_Stack("Module_Loader");
	jc->add_Methods(jc, (JNINativeMethod){ "Have_Module", "(Ljava/lang/String;)Z", Java_cn_Module_1Loader_Have_1Module });
	vm->Add_Class_Stack(vm, jc);
	jc = alloc_JNI_Class_Stack("Shell_Module");
	jc->add_Methods(jc, (JNINativeMethod){ "Run_REPL", "()Z", Java_cn_Shell_1Module_Run_1REPL });
	jc->add_Methods(jc, (JNINativeMethod){ "Init_REPL", "()Z", Java_cn_Shell_1Module_Init_1REPL });
	jc->add_Methods(jc, (JNINativeMethod){ "Add_Command", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", Java_cn_Shell_1Module_Add_1Command });
	vm->Add_Class_Stack(vm, jc);
	vm->Run_Jni_Onload(vm->JVM_Handle);//装载JNI函数
	return vm->Run_main_method(vm->JVM_Handle);//运行JVM 管理模块
}


/*start this_Module*/


Receipt * Init_JVM_Resources_Controller(void){
	Module_Owner * module_owner = Register_Module_Info("Cullen Lee", "JVM_Resources_Controller", 0.1f);
	My_module_owner = module_owner->Module_Handle;
	return Create_Receipt(Init_JVM_Resources_Controller, SUCCESS, My_module_owner, "Init JVM Resources Controller success!");
}


/*end this_Module*/