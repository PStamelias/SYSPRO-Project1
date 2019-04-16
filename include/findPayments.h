#ifndef __structs_h_
#define __structs_h_
#include "struct.h"
#endif	
int findPayments1(char* sender,struct sender_bucket* Sender_Table,int  size,int num_Records);
int findPayments2(char* sender,char* time1,char* year1,char* time2,char* year2,struct sender_bucket* Sender_Table,int size,int num_Records);
