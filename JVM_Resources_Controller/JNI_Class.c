#include "Module_Loader/base.h"
#include<stdlib.h>
#include<string.h>
#include <time.h>

/*停用此页*/
/*需要留接口*/
/*
typedef struct _JVM_Stack
{
	int jni_methods_array_len;
	JavaVM *vm;
	JNI_Class_Stack ** vm_class_stack;
	struct _JVM_Stack * next;
}JVM_Stack;

static JVM_Stack * jvm_stack = NULL;
*/


/*start JVM_Stack的操作函数*//*需要修改*/
/*
JVM_Stack * Get_JVM_Stack(JavaVM * vm){
	JVM_Stack * vm_stack;
	if (jvm_stack == NULL){
		vm_stack = malloc(sizeof(JVM_Stack));
	}
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
			vm_stack->vm_class_stack = NULL;
			vm_stack->next = jvm_stack->next;
			vm_stack_head->next = vm_stack;
			return vm_stack;
		}
	}
}
*/
/*end JVM_Stack control-fun*/


/*start JVM_Stack的指针函数*/
/*
Receipt * add_jni_methods_stack(JavaVM * vm, JNI_Class_Stack *jni_class_stack){
	JVM_Stack *jvm_stack = Get_JVM_Stack(vm);
	if (jvm_stack->jni_methods_array_len == 0){
		if (jvm_stack->vm_class_stack != NULL)free(jvm_stack->vm_class_stack);
		jvm_stack->vm_class_stack = malloc(sizeof(JNI_Class_Stack *));
		jvm_stack->vm_class_stack[0] = jni_class_stack;
		jvm_stack->jni_methods_array_len += 1;
		return Create_Receipt(add_jni_methods_stack, SUCCESS, Get_This_Model_Handle(), "Created New JNI Methods Stack!");
	}
	else{
		if (jvm_stack->jni_methods_array_len > 1000)
			return Create_Receipt(add_jni_methods_stack, ERROR, Get_This_Model_Handle(), "Memory error.");

		JNI_Class_Stack ** jni_m = malloc(sizeof(JNI_Class_Stack *)*(jvm_stack->jni_methods_array_len + 1));
		memmove(jni_m, jvm_stack, sizeof(JNI_Class_Stack)*jvm_stack->jni_methods_array_len);
		jni_m[jvm_stack->jni_methods_array_len] = jni_class_stack;
		free(jvm_stack->vm_class_stack);
		jvm_stack->vm_class_stack = jni_m;
		jvm_stack->jni_methods_array_len += 1;
		return Create_Receipt(add_jni_methods_stack, SUCCESS, Get_This_Model_Handle(), "Updated JNI Methods Stack!");
	}
}
*/
//end JVM_Stack point-fun



