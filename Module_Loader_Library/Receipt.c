#include"Module_Loader/base.h"
#include<time.h>
#include<string.h>


 typedef struct _Receipt_stack{
	 Receipt *const receipt;
	 struct _Receipt_stack *Left;
	 struct _Receipt_stack *Right;
}Receipt_stack;
static _Bool Receipt_Log=1;
static Receipt_stack *Receipt_Pool = NULL;


Receipt *const Create_Receipt_extends(const void *const used_func, const enum SecurityLevel security_level,void *const father ,const char *const additional_info){
	const time_t now = time(NULL);
	Receipt *const receipt = malloc(sizeof(Receipt));
	Receipt receipt_s = { now, used_func, additional_info, { security_level, father } };
	memmove(receipt,&receipt_s,sizeof(receipt_s));
	Receipt_stack * stack_s = malloc(sizeof(Receipt_stack));
	if (Receipt_Pool == NULL){
		Receipt_stack stack = { receipt, stack_s, stack_s };
		memmove(stack_s, &stack, sizeof(stack));
		Receipt_Pool = stack_s;
	}
	else{
		Receipt_Pool->Left = stack_s;
		Receipt_stack stack = { receipt, stack_s , Receipt_Pool };
		memmove(stack_s, &stack, sizeof(stack));
		Receipt_Pool= stack_s;
	}

	if (Receipt_Log);
	return receipt;
}
Receipt *const Create_Receipt(const void *const used_func, const enum SecurityLevel security_level, const char *const additional_info){
	return Create_Receipt_extends(used_func, security_level, NULL, additional_info);
}
void Check_Receipt(){

}