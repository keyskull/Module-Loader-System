
#include<stdlib.h>
#include<string.h>
#include"control_fun.h"


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

JNI_Methods_Stack *alloc_JNI_Methods_Stack(char * ClassName){
	JNI_Methods_Stack *jni_methods_stack = malloc(sizeof(JNI_Methods_Stack));
	memset(jni_methods_stack, 0, sizeof(JNI_Methods_Stack));
	jni_methods_stack->ClassName = ClassName;
	jni_methods_stack->len = 0;
	jni_methods_stack->add_Methods = add_Method;
	return jni_methods_stack;
}