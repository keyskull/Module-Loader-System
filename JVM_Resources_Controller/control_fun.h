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
JNI_Methods_Stack *alloc_JNI_Methods_Stack(JavaVM * vm, char * ClassName);


int Create_VM(JavaVMOption options[], char * Main_ClassName);