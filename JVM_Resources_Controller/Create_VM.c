#include <stdlib.h> 

#include"control_fun.h"
// 环境变量PATH在windows下和linux下的分割符定义


JNIEXPORT jint JNICALL intMethod
(JNIEnv *a, jobject b, jint c){
	printf("test ssss%d", c);
	return c;
}

typedef struct VM_JNI_Stack{	

	JNI_Methods_Stack **(*Get_JNI_Onload_Methods_Stack)(JavaVM *);
}VM_JNI_Stack;



static JNI_Methods_Stack *jni_methods_stack=NULL;

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void *reserved) //这是JNI_OnLoad的声明，必须按照这样的方式声明
{
	if (jni_methods_stack == NULL)return JNI_VERSION_1_8;
	JNIEnv* env = NULL; //注册时在JNIEnv中实现的，所以必须首先获取它
	jint result = -1;
	if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_8) != JNI_OK)return -1; //从JavaVM获取JNIEnv，一般使用1.4的版本

	jclass cls = (*env)->FindClass(env, jni_methods_stack->ClassName); //这里可以找到要注册的类，前提是这个类已经加载到java虚拟机中。 这里说明，动态库和有native方法的类之间，没有任何对应关系。
	if (cls == NULL){
		printf("cannot get class:%s\n", jni_methods_stack->ClassName);
		return -1;
	}
	if ((*env)->RegisterNatives(env, cls, jni_methods_stack->Methods,jni_methods_stack->len) != JNI_OK){ //这里就是关键了，把本地函数和一个java类方法关联起来。不管之前是否关联过，一律把之前的替换掉！
		printf("register native method failed!\n");
		return -1;
	}

	return JNI_VERSION_1_8; //这里很重要，必须返回版本，否则加载会失败。
}

int Create_VM(JavaVMOption options[], char * Main_ClassName){

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

	// 启动虚拟机  
	status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);

	if (status != JNI_ERR){
		// 先获得class对象  
		cls = (*env)->FindClass(env, Main_ClassName);
		if (cls != 0){
			// 获取方法ID, 通过方法名和签名, 调用静态方法  
			mid = (*env)->GetStaticMethodID(env, cls, "main", "([Ljava/lang/String;)V");
			if (mid != 0){
				(*env)->CallStaticObjectMethod(env, cls, mid);
				JNI_OnLoad(jvm, NULL);
				(*env)->CallStaticObjectMethod(env, cls, mid);
			}
		}
		jthrowable throws = (*env)->ExceptionOccurred(env);
		if (throws != NULL)(*env)->ExceptionDescribe(env);
		(*jvm)->DestroyJavaVM(jvm);
		system("pause");
		return 0;
	}
	else
	{
		printf("JVM Created failed!\n");
		system("pause");
		return -1;

	}

}