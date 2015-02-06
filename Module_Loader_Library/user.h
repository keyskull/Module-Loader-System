
#ifndef __USER_H__
#define __USER_H__




typedef struct {
	const unsigned int uid;
	char * const user_name;
}User_Info;

typedef struct{
	time_t * const last_login_date;
}User_log;
typedef struct {
	const long owner;
	User_log * const user_log;
	User_Info *const user_info;
}User_struct;


User_struct * Add_User(char *name);
int Remove_User(User_struct *user);
#endif