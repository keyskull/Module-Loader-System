
#include<stdlib.h>
#include<string.h>
#include"control_fun.h"
/*需要留接口*/
typedef struct _JVM_Stack
{
	int jni_methods_array_len;
	JavaVM *vm;
	JNI_Methods_Stack ** vm_methods_array;
	struct _JVM_Stack * next;
}JVM_Stack;
static JVM_Stack * jvm_stack = NULL;

JVM_Stack * Get_JVM_Stack(JavaVM * vm){
	JVM_Stack * vm_stack;
	if (jvm_stack == NULL)vm_stack=malloc(sizeof(JVM_Stack));
	else {
		vm_stack = jvm_stack;
		do{
			if(vm_stack->vm!=vm)
				vm_stack=vm_stack->next;
		}
		while(vm_stack->vm != vm)

	}
}

/*JNI_Methods_Stack 的指针函数*/
int add_Method(JNI_Methods_Stack *jni_methods_stack, JNINativeMethod jnm){
	JNINativeMethod **jni_method;
	if (jni_methods_stack != NULL){
		jni_method = malloc(sizeof(JNINativeMethod)*(jni_methods_stack->len + 1));
		memset(jni_method, 0, sizeof(JNINativeMethod)*(jni_methods_stack->len + 1));
		if (jni_methods_stack->Methods != NULL)memmove(jni_method, jni_methods_stack->Methods, sizeof(JNINativeMethod)*(jni_methods_stack->len));
		memmove(&jnm, jni_method[jni_methods_stack->len], sizeof(JNINativeMethod));
		jni_methods_stack->len += 1;
		jni_methods_stack->Methods = jni_method;
		return EXIT_SUCCESS;
	}
	else 
	return EXIT_FAILURE;
}


/*end   JNI_Methods_Stack 的指针函数*/
JNI_Methods_Stack *alloc_JNI_Methods_Stack(JavaVM * vm,char * ClassName){
	JNI_Methods_Stack *jni_methods_stack = malloc(sizeof(JNI_Methods_Stack));
	memset(jni_methods_stack, 0, sizeof(JNI_Methods_Stack));
	jni_methods_stack->ClassName = ClassName;
	jni_methods_stack->len = 0;
	jni_methods_stack->add_Methods = add_Method;
	return jni_methods_stack;
}