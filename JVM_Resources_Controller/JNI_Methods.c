#include "Module_Loader/base.h"
#include<stdlib.h>
#include<string.h>
#include"control_fun.h"
#include <time.h>
/*需要留接口*/
typedef struct _JVM_Stack
{
	int jni_methods_array_len;
	JavaVM *vm;
	JNI_Methods_Stack ** vm_methods_array;
	struct _JVM_Stack * next;
}JVM_Stack;

static JVM_Stack * jvm_stack = NULL;



/*start JVM_Stack的操作函数*/
JVM_Stack * Get_JVM_Stack(JavaVM * vm){
	JVM_Stack * vm_stack;
	if (jvm_stack == NULL)vm_stack = malloc(sizeof(JVM_Stack));
	else {
		JVM_Stack * vm_stack_head = jvm_stack;
		vm_stack = jvm_stack;
		while (vm_stack->vm != vm)if ((vm_stack = vm_stack->next) == vm_stack_head)break;
		if (vm_stack != vm_stack_head)return vm_stack;
		else {
			vm_stack = malloc(sizeof(JVM_Stack));
			memset(vm_stack, 0, sizeof(JVM_Stack));
			vm_stack->jni_methods_array_len = 0;
			vm_stack->vm = vm;
			vm_stack->vm_methods_array = NULL;
			vm_stack->next = jvm_stack->next;
			vm_stack_head->next = vm_stack;
			return vm_stack;
		}
	}
}

/*end JVM_Stack control-fun*/
/*start JVM_Stack的指针函数*/
Receipt * add_jni_methods_stack(JavaVM * vm, JNI_Methods_Stack *jni_methods_stack){
	JVM_Stack *jvm_stack = Get_JVM_Stack(vm);
	if (jvm_stack->jni_methods_array_len == 0){
		if (jvm_stack->vm_methods_array != NULL)free(jvm_stack->vm_methods_array);
		jvm_stack->vm_methods_array = malloc(sizeof(JNI_Methods_Stack *));
		jvm_stack->vm_methods_array[0] = jni_methods_stack;
		jvm_stack->jni_methods_array_len += 1;
	}
	else{
			JNI_Methods_Stack ** jni_m = malloc(sizeof(JNI_Methods_Stack *)*(jvm_stack->jni_methods_array_len + 1));
			memmove(jni_m, jvm_stack, sizeof(JNI_Methods_Stack)*jvm_stack->jni_methods_array_len);
			jni_m[jvm_stack->jni_methods_array_len] = jni_methods_stack;
			free(jvm_stack->vm_methods_array);
			jvm_stack->vm_methods_array = jni_m;
			jvm_stack->jni_methods_array_len += 1;
	}
	/*work to there*/


}

/*end JVM_Stack point-fun*/




/*start JNI_Methods_Stack 的指针函数*/
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


/*end JNI_Methods_Stack  point-fun*/
JNI_Methods_Stack *alloc_JNI_Methods_Stack(JavaVM * vm,char * ClassName){
	JNI_Methods_Stack _jni_methods_stack = { (int)clock(), 0, ClassName, NULL, add_Method };
	JNI_Methods_Stack *jni_methods_stack = malloc(sizeof(JNI_Methods_Stack));
	memset(jni_methods_stack, 0, sizeof(JNI_Methods_Stack));
	memmove(jni_methods_stack, &_jni_methods_stack, sizeof(JNI_Methods_Stack));
	JVM_Stack *jvm_stack=Get_JVM_Stack(vm);
	jvm_stack->vm_methods_array = jni_methods_stack;
	return jni_methods_stack;
}