
#include "Module_Loader/base.h"
#include "User_module/authentication.h"
#include <string.h>
#include <time.h>

typedef struct _User_Handle{
	int status;
	User_Info * user_info;
	User_log * user_log;
}User_Handle;

typedef struct _User_stack{
	User_Handle *const user_handle;
	struct _User_stack *next;
}User_stack;



typedef struct _Event{
	void * const func;
	char * const user_name;
	User_data *user_struct;
}Event;

int Find_User(char *name);


static int User_manager(Event *event){
	static User_stack *user_stack = NULL;
	static void *Event_cache = NULL;
	clock_t _clock = clock() + 30000;
	while (Event_cache != NULL)	if (clock()>_clock)return -3;//需要修改
	Event_cache = event->func;

	User_stack *const _user_stack = malloc(sizeof(User_stack));
	if (event->func == Add_User){
		unsigned int uid = 0;
		if (user_stack != NULL){	
			User_stack *u = user_stack;
			do{	++uid;
			if (strcmp(event->user_name, u->user_handle->user_info->user_name) != 0)return EXIT_FAILURE;
			} while ((u = u->next) != user_stack);
			_user_stack->next = user_stack;
		}else _user_stack->next = _user_stack;
		User_stack __user_stack = { malloc(sizeof(User_data)), _user_stack->next };
		User_Info _user = { uid, event->user_name };
		User_Info *const user = malloc(sizeof(User_Info));
		memcpy(user, &_user, sizeof(User_Info));
		User_data user_struct = { (const long)_clock, malloc(sizeof(User_log)), user };
		memmove(__user_stack.user_handle, &user_struct, sizeof(User_data));
		memcpy(_user_stack, &__user_stack, sizeof(User_stack));
		event->user_struct = _user_stack->user_handle;
		user_stack = _user_stack;
	}
	else if (event->func == Remove_User){

	}
	else if (event->func == Find_User){

	}
	else{
		Event_cache = NULL;
		return EXIT_FAILURE;
	}
	Event_cache = NULL;
	return EXIT_SUCCESS;
}


User_data * Add_User(char *name){
	Event event = { Add_User, name, NULL };
	if (User_manager(&event) == EXIT_SUCCESS)return event.user_struct;
	else return 2;//需要修改
}

int Remove_User(User_data *user){
	Event event = { Remove_User, NULL, user };
	return User_manager(&event);
}

User_data * Get_User_Permission(Validated_License  *name){
	return EXIT_SUCCESS;
}

int Find_User(char *name){
	return EXIT_SUCCESS;
}
