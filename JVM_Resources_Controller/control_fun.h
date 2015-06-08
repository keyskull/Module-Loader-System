#include<stdlib.h>
#include<jni.h>

/*
enum Library_Status{Loaded,Loadding,Unload,Unknow};
struct _Library_Loaded{
	enum Library_Status status;
	char *const name;
	void *const Binary_Library;
};*/



typedef struct _JNI_Class_Stack JNI_Class_Stack;
struct _JNI_Class_Stack{
	const int id;
	int len;
	char *const ClassName;
	JNINativeMethod **Methods;
	int(*add_Methods)(JNI_Class_Stack *, JNINativeMethod);
	int(*Remove_Methods)(JNI_Class_Stack *, JNINativeMethod);//未写
	int(*Clean_all_Methods)(JNI_Class_Stack *);//未写
};

typedef struct _VM_func_stack{
	void * JVM_Handle;
	int(*Run_main_method)(void * JVM_Handle);
	int(*Run_Jni_Onload)(void * JVM_Handle);
	int(*Stop_vm)(void * JVM_Handle);
	Receipt *(*Recycling_resources)(void * JVM_Handle);//未写
}VM_func_stack;

void * Get_This_Model_Owner_Key();

JNI_Class_Stack **(*Get_JNI_Onload_Methods_Stack)(JavaVM *);
JNI_Class_Stack *alloc_JNI_Class_Stack(JavaVM * vm, char * ClassName);
VM_func_stack* Create_VM(JavaVMOption options[], char * Main_ClassName);