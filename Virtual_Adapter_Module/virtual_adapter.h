
#ifndef _VIRTUAL_ADAPTER_H_
#define _VIRTUAL_ADAPTER_H_
#include<stdlib.h>

typedef unsigned char byte;
enum ip_version{ IPv4, IPv6 };
typedef struct _IP{
	const enum ip_version version;
	union Protocol_type{
		byte IPv4[4];
		byte IPv6[16];
	};
}IP_Protocol;

typedef struct _Virtual_Network_Adapter_struct{
	const char *const Virtual_Adapter_name;
	const short int Adapter_ID;
	const byte MAC[6];
	IP_Protocol IP;
	IP_Protocol DNS1;
	IP_Protocol DNS2;
}Virtual_Adapter_Struct;

typedef	const unsigned char Key_data[4];
#endif