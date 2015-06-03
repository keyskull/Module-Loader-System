#include<stdlib.h>
#include<jni.h>

enum Library_Status{Loaded,Loadding,Unload,Unknow};
struct _Library_Loaded{
	enum Library_Status status;
	char *const name;
	void *const Binary_Library;
};

typedef struct _JNI_Methods_Stack{
	const int id;
	int len;
	char *const ClassName;
	JNINativeMethod **Methods;
	int(*add_Methods)(JNI_Methods_Stack *, JNINativeMethod);
}JNI_Methods_Stack;

typedef struct _VM_func_stack{
	JavaVM *const vm;
	int(*Run_main_method)(JavaVM *vm);
	int(*Run_Jni_Onload)(JavaVM *vm);
	int(*Stop_vm)(JavaVM *vm);

}VM_func_stack;
VM_func_stack * alloc_VM_func_stack(JavaVM *vm);
JNI_Methods_Stack *alloc_JNI_Methods_Stack(JavaVM * vm, char * ClassName);


VM_func_stack* Create_VM(JavaVMOption options[], char * Main_ClassName);