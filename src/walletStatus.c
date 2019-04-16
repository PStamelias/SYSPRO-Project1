#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "fun.h"
#ifndef __structs_h_
#define __structs_h_

#include "struct.h"
#endif
#include "walletStatus.h"
int walletStatus(struct walletId* wallet,int size,char* name){
	int i;
	int enter=0;
	for(i=0;i<size;i++){
		if(!strcmp(wallet[i].userID,name)){
			enter=1;
			return wallet[i].balance;
		}
	}
	if(enter==0)
		return 0;
}