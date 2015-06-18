#include <stdlib.h>
#include <string.h>
#include "Module_Loader/base.h"
#include "JVM_Resources_Controller/control_fun.h"
#include "handle.h"


typedef struct _JVM_Handle JVM_Handle;
struct _JVM_Handle{
	JavaVM* vm;
	char * mainclass;
	int jni_class_list_len;
	JNI_Class_Stack ** jni_class_list;
};
struct _JVM_Handle_stack{ JVM_Handle *jvm_handle; struct _JVM_Handle_stack *next; };
static struct _JVM_Handle_stack *JVM_Handle_POOL = NULL;//JVM数据池


//start Handle function
JVM_Handle *Get_JVM_Handle(JavaVM *vm){
	if (JVM_Handle_POOL == NULL)return NULL;
	else {
		struct _JVM_Handle_stack *jvm_handle_stack_head = JVM_Handle_POOL, *jvm_handle_stack = jvm_handle_stack_head;
		if (vm == jvm_handle_stack_head->jvm_handle->vm)return jvm_handle_stack_head->jvm_handle;
		else do{
			jvm_handle_stack = jvm_handle_stack->next;
		} while (jvm_handle_stack->jvm_handle->vm != vm || jvm_handle_stack_head->jvm_handle->vm != vm);
		if (vm == jvm_handle_stack_head->jvm_handle->vm)return NULL;
		else return jvm_handle_stack->jvm_handle;
	}
}



JVM_Handle * Check_JVM_Handle(void * JVM_Handle){
	if (JVM_Handle_POOL == NULL)return NULL;
	else {
		struct _JVM_Handle_stack *jvm_handle_stack_head = JVM_Handle_POOL, *jvm_handle_stack = jvm_handle_stack_head;
		if (JVM_Handle == jvm_handle_stack_head->jvm_handle)return jvm_handle_stack_head->jvm_handle;
		else do{
			jvm_handle_stack = jvm_handle_stack->next;
		} while (jvm_handle_stack->jvm_handle != JVM_Handle || jvm_handle_stack_head->jvm_handle != JVM_Handle);
		if (JVM_Handle == jvm_handle_stack_head)return NULL;
		else return jvm_handle_stack->jvm_handle;
	}
}

JVM_Handle *Create_JVM_Handle(JavaVM *vm, char * Main_ClassName){
	JVM_Handle * jvm_handle = malloc(sizeof(JVM_Handle));
	jvm_handle->vm = vm;
	jvm_handle->mainclass = Main_ClassName;
	jvm_handle->jni_class_list_len = 0;
	jvm_handle->jni_class_list = Get_JNI_Onload_Methods_Stack(vm);
	struct _JVM_Handle_stack * jvm_handle_stack = malloc(sizeof(struct _JVM_Handle_stack));
	jvm_handle_stack->jvm_handle = jvm_handle;
	if (JVM_Handle_POOL == NULL){
		jvm_handle_stack->next = jvm_handle_stack;
		JVM_Handle_POOL = jvm_handle_stack;
	}
	else{
		jvm_handle_stack->next = JVM_Handle_POOL->next;
		JVM_Handle_POOL->next = jvm_handle_stack;
	}
	return jvm_handle;
}
//end Handle function







/*start VM_func_stack的指针函数*/
int Add_Class_Stack(void * _JVM_Handle, JNI_Class_Stack * jni_class_stack){//未完成
	if (jni_class_stack == NULL)return EXIT_FAILURE;
	else{
		JVM_Handle * jvm_handle = Check_JVM_Handle(_JVM_Handle);
		if (jvm_handle->jni_class_list_len == 0)
			if (jvm_handle->jni_class_list == NULL){
				jvm_handle->jni_class_list = malloc(sizeof(JNI_Class_Stack *)*jvm_handle->jni_class_list_len + 1);
			}
			else free(jvm_handle->jni_class_list);

	}
}
int Run_main_method(void * _JVM_Handle){
	JVM_Handle * jvm_handle = Check_JVM_Handle(_JVM_Handle);
	JavaVM *vm = jvm_handle->vm;
	JNIEnv *env = (*vm)->GetEnv(vm, NULL, JNI_VERSION_1_8);
	jclass cls = (*env)->FindClass(env, jvm_handle->mainclass);
	jmethodID mid;
	if (cls != 0){
		mid = (*env)->GetStaticMethodID(env, cls, "main", "([Ljava/lang/String;)V");
		(*env)->CallStaticObjectMethod(env, cls, mid);
	}
	jthrowable throws = (*env)->ExceptionOccurred(env);
	if (throws != NULL)(*env)->ExceptionDescribe(env);
	(*vm)->DestroyJavaVM(vm);
	return 1;
}
int Run_Jni_Onload(void * _JVM_Handle){
	JVM_Handle * jvm_handle = Check_JVM_Handle(_JVM_Handle);
	JNI_OnLoad(jvm_handle->vm, NULL);
	return 1;
}
int Stop_vm(void * _JVM_Handle){

	return 1;
}

int Recycling_resources(JavaVM *vm){
	//资源回收功能
	return NULL;
}
/*end point to VM_func_stack*/



//start JNI_Class_Stack 的指针函数
int add_Method(JNI_Class_Stack *jni_methods_stack, JNINativeMethod jnm){//可能有问题需要修改
	JNINativeMethod **jni_method;
	int len = jni_methods_stack->len + 1;
	if (jni_methods_stack != NULL){
		jni_method = malloc(sizeof(JNINativeMethod *)*(len));
		memset(jni_method, 0, sizeof(JNINativeMethod *)*(len));
		if (jni_methods_stack->Methods_List != NULL)memmove(jni_method, jni_methods_stack->Methods_List, sizeof(JNINativeMethod *)*(jni_methods_stack->len));
		memmove(jni_method[jni_methods_stack->len], &jnm, sizeof(JNINativeMethod));
		jni_methods_stack->len += 1;
		jni_methods_stack->Methods_List = jni_method;
		return EXIT_SUCCESS;
	}
	else
		return EXIT_FAILURE;
}
int Remove_Methods(JNI_Class_Stack *jni_methods_stack, JNINativeMethod jnm){
	//working
	return 0;
}
int Clean_all_Methods(JNI_Class_Stack *jni_methods_stack){
	//working
	return 0;
}
//end JNI_Class_Stack  point-fun



JNI_Class_Stack *alloc_JNI_Class_Stack(char * ClassName){//需要修改
	JNI_Class_Stack _jni_class_stack = { (int)clock(), 0, ClassName, NULL, add_Method, Remove_Methods, Clean_all_Methods };
	JNI_Class_Stack *jni_class_stack = malloc(sizeof(JNI_Class_Stack));
	memset(jni_class_stack, 0, sizeof(JNI_Class_Stack));
	memmove(jni_class_stack, &_jni_class_stack, sizeof(JNI_Class_Stack));
	return 	jni_class_stack;
}

VM_stack * alloc_VM_func_stack(JavaVM *vm, char * Main_ClassName){
	VM_stack * vm_func_stack = malloc(sizeof(VM_stack));
	vm_func_stack->JVM_Handle = Create_JVM_Handle(vm, Main_ClassName);
	vm_func_stack->Run_Jni_Onload = Run_Jni_Onload;
	vm_func_stack->Run_main_method = Run_main_method;
	vm_func_stack->Stop_vm = Stop_vm;
	vm_func_stack->Recycling_resources = Recycling_resources;
	return vm_func_stack;
}


JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void *reserved){ //这是JNI_OnLoad的声明，必须按照这样的方式声明
	JVM_Handle *jvm_handle = Get_JVM_Handle(vm);
	JNIEnv* env = NULL; //注册时在JNIEnv中实现的，所以必须首先获取它
	jclass cls = NULL;
	jint result = -1;
	if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_8) != JNI_OK)return -1; //从JavaVM获取JNIEnv，一般使用1.4的版本
	for (int i = 0; i < jvm_handle->jni_class_list_len; i++){
		cls = (*env)->FindClass(env, jvm_handle->jni_class_list[i]->ClassName); //这里可以找到要注册的类，前提是这个类已经加载到java虚拟机中。 这里说明，动态库和有native方法的类之间，没有任何对应关系。
		if (cls == NULL){
			printf("cannot get class:%s\n", jvm_handle->jni_class_list[i]->ClassName);
			return -1;
		}
		if ((*env)->RegisterNatives(env, cls, *jvm_handle->jni_class_list[i]->Methods_List, jvm_handle->jni_class_list[i]->len) != JNI_OK){ //这里就是关键了，把本地函数和一个java类方法关联起来。不管之前是否关联过，一律把之前的替换掉！
			printf("register native method failed!\n");
			return -1;
		}
	}
	return JNI_VERSION_1_8; //这里很重要，必须返回版本，否则加载会失败。
}


VM_stack * Create_VM(JavaVMOption options[], char * Main_ClassName){
	JNIEnv *env;
	JavaVM *jvm;
	JavaVMInitArgs vm_args;
	long status;
	jclass cls;
	jmethodID mid;
	jfieldID fid;
	jobject obj;

	memset(&vm_args, 0, sizeof(vm_args));
	vm_args.version = JNI_VERSION_1_8;
	vm_args.nOptions = 1;
	vm_args.options = options;

	status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);//创建虚拟机

	if (status != JNI_ERR){
		Create_Receipt(Create_VM, SUCCESS, My_module_owner, "Created JVM!");
		return alloc_VM_func_stack(jvm, Main_ClassName);
	}
	else{
		Create_Receipt(Create_VM, ERROR, My_module_owner, "Can not Create JVM!");
		return NULL;
	}

}