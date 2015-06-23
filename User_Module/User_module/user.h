
#ifndef __USER_H__
#define __USER_H__




typedef const struct _User_Info {
	const unsigned int uid;
	char * user_name;
}User_Info;

typedef struct{
	time_t * const last_login_date;
}User_log;


typedef struct {
	void * handle;
	User_log *const user_log;
	User_Info *const user_info;
	Receipt *(*Change_Name)(void * handle);
	Receipt *(*Logout)(void * handle);
}User_data;


User_data * Add_User(char *name);
int Remove_User(User_data *user);

#endif