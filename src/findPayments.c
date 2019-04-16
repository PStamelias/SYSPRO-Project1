#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "fun.h"
#ifndef __structs_h_
#define __structs_h_

#include "struct.h"
#endif
#include "findPayments.h"
int findPayments1(char* sender,struct sender_bucket* Sender_Table,int size,int num_Records){
	int position=hash_number_char(sender,size);
	int i;
	struct sender* start=NULL;
	struct sender* sen=NULL;
	struct sender_bucket* bucketptr=&Sender_Table[position];
	int sum=0;
	int found=0;
	while(1){
		for(i=0;i<num_Records;i++){
			start=&bucketptr->sender_bucketptr[i];
			if(start->walletIdptr==NULL){
				found=1;
				break;
			}
			if(strcmp(start->walletIdptr->userID,sender)==0){
				sen=start;
				found=1;
				break;
			}
		}
		if(found==1)
			break;
		if(bucketptr->next==NULL)
			break;
		bucketptr=bucketptr->next;
	}
	if(sen==NULL){
		printf("%s\n","User does not exist");
		return sum;
	}
	char current[50];
	strcpy(current,"");
	struct transaction_node* beg=sen->transaction_nodeptr;
	while(1){
		sum+=beg->node_ptr->value;
		if(strcmp(beg->transaction_ptr->transactionId,current)!=0){
			strcpy(current,beg->transaction_ptr->transactionId);
			printf("%s ",beg->transaction_ptr->transactionId);
			printf("%s ",beg->transaction_ptr->senderWalletId);
			printf("%s ",beg->transaction_ptr->receiverWalletId);
			printf("%d ",beg->transaction_ptr->value);
			printf("%s ",beg->transaction_ptr->date);
			printf("%s\n",beg->transaction_ptr->time);
		}
		if(beg->next==NULL)
			break;
		beg=beg->next;
	}
	return sum;
}
int findPayments2(char* sender,char* time1,char* year1,char* time2,char* year2,struct sender_bucket* Sender_Table,int size,int num_Records){
	int position=hash_number_char(sender,size);
	int i;
	struct sender* start=NULL;
	struct sender* sen=NULL;
	struct sender_bucket* bucketptr=&Sender_Table[position];
	int sum=0;
	int found=0;
	while(1){
		for(i=0;i<num_Records;i++){
			start=&bucketptr->sender_bucketptr[i];
			if(start->walletIdptr==NULL){
				found=1;
				break;
			}
			if(strcmp(start->walletIdptr->userID,sender)==0){
				sen=start;
				found=1;
				break;
			}
		}
		if(found==1)
			break;
		if(bucketptr->next==NULL)
			break;
		bucketptr=bucketptr->next;
	}
	if(sen==NULL){
		printf("%s\n","User does not exist");
		return sum;
	}
	char current[50];
	strcpy(current,"");
	struct transaction_node* beg=sen->transaction_nodeptr;
	while(1){
		sum+=beg->node_ptr->value;
		if(strcmp(beg->transaction_ptr->transactionId,current)!=0&&isvalid(beg->transaction_ptr->date,beg->transaction_ptr->time,time1,year1,time2,year2)==1){
			strcpy(current,beg->transaction_ptr->transactionId);
			printf("%s ",beg->transaction_ptr->transactionId);
			printf("%s ",beg->transaction_ptr->senderWalletId);
			printf("%s ",beg->transaction_ptr->receiverWalletId);
			printf("%d ",beg->transaction_ptr->value);
			printf("%s ",beg->transaction_ptr->date);
			printf("%s\n",beg->transaction_ptr->time);
		}
		if(beg->next==NULL)
			break;
		beg=beg->next;
	}
	return sum;
}