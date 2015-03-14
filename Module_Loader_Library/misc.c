#include "Module_Loader\base.h"
#include <time.h>
#include <string.h>


static char * _Tokenizer(char *args, const char Keyword){
	int i = 0;
	while (args[i] != Keyword && args[i] != '\0')i++;
	return &args[i];
}

//·Ö´ÊÆ÷
char **const Tokenizer(char *args, const char Keyword){


	int i = 0;
	if (args == NULL ||args[0] == '\0')return NULL;
	char *cache = args;
	char ** const cache_array = calloc(INIT_CACHE_SIZE / 2, sizeof(char *));

	while (1){
		int n = 0;
		char *tk = _Tokenizer(cache, Keyword);
		cache_array[i] = calloc(INIT_CACHE_SIZE, sizeof(char));
		for (int t = 0; &cache[t] < &tk[0]; t++){
			cache_array[i][t] = cache[t];
			cache_array[i][t + 1] = '\0';
		}
		i++;
		if (cache != tk && tk[0]==' ') cache = &tk[1]; 
		else break;
	}
	char **char_array = calloc(i+1, sizeof(char *));
	for (int n = 0; n < i; n++){
		char_array[n] = calloc(strlen(cache_array[n]), sizeof(char));
		strcpy(char_array[n], cache_array[n]);
	}
	char_array[i] = NULL;
	free(cache_array);
	return char_array;
}

char *Show_Time_CharFormat(time_t time, const char * format){
	char *date = calloc(INIT_CACHE_SIZE, sizeof(char));
	struct tm *times;
	times = localtime(&time);
	strftime(date, INIT_CACHE_SIZE, format, times);
	int len = strlen(date) + 1;
	char *date_s = calloc(len, sizeof(char));
	memmove(date_s, date, len);
	free(date);
	return date_s;
}

char *Get_date(const char * format){
	time_t now;
	time(&now);
	return Show_Time_CharFormat(now, format);
}

