/*
 * config.c
 *
 *  Created on: 2014年11月14日
 *      Author: key_q
 */

#include "Module_Loader\base.h"
#include <assert.h>
#include <string.h>
typedef struct _Parameter_stack{
	const unsigned char *Pname;
	const char * content;
	struct _Parameter_stack * next;
}Parameter_stack;

static Parameter_stack * loaded_config;
Parameter_stack * Analysis_cofig(char * code){
	return NULL;
}
FILE *Check_conf(FILE * config_file){

	return NULL;
}

int Init_conf(FILE config_file){


return EXIT_SUCCESS;
}

