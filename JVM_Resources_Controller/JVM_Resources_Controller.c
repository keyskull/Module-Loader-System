#include "Module_Loader/base.h"
#include "Terminal_module/terminal.h"
#include"JVM_Resources_Controller/control_fun.h"
#include "REPL_module/repl.h"
#include"handle.h"
#include<windows.h>
#include <string.h>
#include<stdio.h>
#include<direct.h>
static Terminal_data *ter = NULL;
static void*JVM_Handle = NULL;
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
	return (jboolean)1;
}
/*end Module_Loader Jni_Class*/
/*Shell_Module Jni_Class*/

JNIEXPORT jboolean JNICALL Java_cn_Shell_1Module_Run_1REPL
(JNIEnv * env, jobject obj){
	Run_shell(ter);
	return (jboolean)1;
}

JNIEXPORT jboolean JNICALL Java_cn_Shell_1Module_Init_1REPL
(JNIEnv * env, jobject obj){
	
	return (jboolean)1;
}


JNIEXPORT jboolean JNICALL Java_cn_Shell_1Module_Add_1Command
(JNIEnv * env, jobject obj, jstring cmd, jstring classname, jstring func_name){
	char* rtn = NULL;
	jclass clsstring = (*env)->FindClass(env,"java/lang/String");
	jstring strencode = (*env)->NewStringUTF(env,"utf-8");
	jmethodID mid = (*env)->GetMethodID(env,clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray)(*env)->CallObjectMethod(env, classname, mid, strencode);
	jsize alen = (*env)->GetArrayLength(env,barr);
	jbyte* ba = (*env)->GetByteArrayElements(env,barr, JNI_FALSE);
	if (alen > 0){
		rtn = (char*)malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	(*env)->ReleaseByteArrayElements(env,barr, ba, 0);
	//Add_command(ter->running_program, rtn, func);

	return (jboolean)1;
}
/*end Shell_Module Jni_Class*/

Receipt * Create_JVM_Management(void){
	JavaVMOption options[1];
	options[0].optionString = "-Djava.class.path=./scala-library.jar;./tools.jar;./Module_loader_System.jar";
	/*
	JNI_Class_Stack *jc = alloc_JNI_Class_Stack("cn/Module_Loader");
	jc->add_Methods(jc, (JNINativeMethod){ "Have_Module", "(Ljava/lang/String;)Z", Java_cn_Module_1Loader_Have_1Module });
	vm->Add_Class_Stack(vm->JVM_Handle, jc);
	*/
	JNI_Class_Stack *jc = alloc_JNI_Class_Stack("cn/Shell_Module");
	jc->add_Methods(jc, (JNINativeMethod){ "Init_REPL", "()Z", Java_cn_Shell_1Module_Init_1REPL });
	jc->add_Methods(jc, (JNINativeMethod){ "Run_REPL", "()Z", Java_cn_Shell_1Module_Run_1REPL });
	jc->add_Methods(jc, (JNINativeMethod){ "Add_Command", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", Java_cn_Shell_1Module_Add_1Command });
	VM_stack * vm = Create_VM(options, 1, "JVM_Management");
	vm->Add_Class_Stack(vm->JVM_Handle, jc);
	JVM_Handle = vm->JVM_Handle;
	vm->Run_Jni_Onload(vm->JVM_Handle);//装载JNI函数
	return Create_Receipt(Create_JVM_Management, SUCCESS, My_module_owner, "Init JVM Resources Controller Created!");
}


char * Run_JVM_Application(const Args_struct *const args, Repl_data_struct * const repl_data){//need Modify
	/*/	if (args->length == 0)return "空参数";
	JavaVMOption *options = malloc(sizeof(JavaVMOption *)*args->length);
	int len = 0;
	for (int i = 0; i < args->length; i++){
		if (args->args[i][0] == '-'){
			len++;
			options[i].optionString = args->args[i];
		}
	}
	VM_stack * vm_s = Create_VM(options, len, args->args[args->length]);
	//vm->Run_Jni_Onload(vm_s->JVM_Handle);
	Receipt* receipt = vm_s->Run_main_class_method(vm_s->JVM_Handle,NULL);
	if (receipt->security_level==ERROR)return "error";
	return "";
	*/

	char * cache = malloc(sizeof(char)*INIT_CACHE_SIZE*10);
	memset(cache, 0, sizeof(char)*INIT_CACHE_SIZE*10);
	for (int i = 0; i < args->length; i++){
		strcat(cache, args->args[i]);
	}
	char *cache_2 = malloc(sizeof(char)*(strlen(cache) + INIT_CACHE_SIZE));
	memset(cache_2, 0, sizeof(char)*INIT_CACHE_SIZE * 10);
	strcat(cache_2, "cmd /c .\\jdk1.8.0_31\\jre\\bin\\java.exe ");
	strcat(cache_2, cache);
	system(cache_2);
	free(cache);
	return "";

}
char * Run_jps(const Args_struct *const args, Repl_data_struct * const repl_data){//need Modify
	char * cache_2 = malloc(sizeof(char)*INIT_CACHE_SIZE);
	memset(cache_2, 0, sizeof(char)*INIT_CACHE_SIZE);
	strcat(cache_2, "cmd /c ");
	strcat(cache_2, ".\\jdk1.8.0_31\\bin\\jps.exe");
	system(cache_2);
	free(cache_2);
	return "";
}


char * Run_JVM_Memory_Checker(const Args_struct *const args, Repl_data_struct * const repl_data){//need Modify

	/*
	char *cache = malloc(sizeof(char)*INIT_CACHE_SIZE);
	memset(cache, 0, sizeof(char)*INIT_CACHE_SIZE);
	getcwd(cache, sizeof(char)*INIT_CACHE_SIZE);
	strcat(cache, "\\");
	strcat(cache, "jdk1.8.0_31\\jre\\bin\\java.exe\" -classpath=\"./scala-library.jar;./tools.jar;./Module_loader_System.jar\" Memory_Checker");
	*/
	char * cache_2 = malloc(sizeof(char)*INIT_CACHE_SIZE);
	memset(cache_2, 0, sizeof(char)*INIT_CACHE_SIZE);
	strcat(cache_2, "cmd /c ");
	strcat(cache_2, ".\\jdk1.8.0_31\\jre\\bin\\java.exe -classpath \"./scala-library.jar;./tools.jar;./Module_loader_System.jar\" Memory_Checker ");
	if (args!=NULL)if (args->length > 0)strcat(cache_2, args->args[0]);
	system(cache_2);
	free(cache_2);


	/*
	JavaVM *vm = Get_JavaVM(JVM_Handle);
	JNIEnv* env = NULL;
	jmethodID mid;
	if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_4) ==JNI_OK)
		if ((*vm)->AttachCurrentThread(vm, (void **)&env, NULL) == JNI_OK){
			jclass cls = (*env)->FindClass(env, "Memory_Checker");
			if (cls == NULL)(*vm)->DetachCurrentThread(vm);
			else{
				mid = (*env)->GetStaticMethodID(env, cls, "main", "([Ljava/lang/String;)V");
				if (mid == NULL)(*vm)->DetachCurrentThread(vm);
				else(*env)->CallStaticVoidMethod(env, cls, mid);
			}
		(*vm)->DetachCurrentThread(vm);
	}
	else return "run filed.";
	else return "run filed.";
	return "";
	*/
	return "";
}

Receipt *Run_JVM_Management(void){
	VM_stack *vm = Get_JVM_VM_stack(JVM_Handle);
	return vm->Run_main_class_method(vm->JVM_Handle,  NULL);
}
/*start this_Module*/

/*模块入口*/
Receipt * Init_JVM_Resources_Controller(Terminal_data *terminal){
	Module_Owner * module_owner = Register_Module_Info("Cullen Lee", "JVM_Resources_Controller", 0.1f);
	My_module_owner = module_owner->Module_Handle;
	CMD_list_stack *cmd_list_stack = Get_cmd_list_stack(terminal);
	ter = terminal;
	Add_command(cmd_list_stack, "java", Run_JVM_Application);
	Add_command(cmd_list_stack, "javamemory", Run_JVM_Memory_Checker);
	Add_command(cmd_list_stack, "jps", Run_jps);
	printf("Init JVM Module Success\n");
	return Create_Receipt(Init_JVM_Resources_Controller, SUCCESS, My_module_owner, "Init JVM Resources Controller success!");
}


/*end this_Module*/