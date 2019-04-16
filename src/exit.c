#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "fun.h"
#ifndef __structs_h_
#define __structs_h_

#include "struct.h"
#endif
#include "exit.h"
void exit1(struct BitCoinInfo** Bit_Table,int size){
	struct BitCoinInfo** start,*end;
	int i;
	for(i=0;i<size;i++){
		start=&Bit_Table[i];
		if(*start==NULL)
			continue;
		end=(*start)->next;
		while(1){
			free_tree((*start)->riza);
			free(*start);
			*start=end;
			if(*start==NULL)
				break;
			end=end->next;
		}
	}
}
void exit2(struct sender_bucket* Sender_Table,struct sender_bucket* Receiver_Table,int num_Records,int size1,int size2){
	struct sender_bucket* o;
	struct sender* start1;
	struct transaction_node* tran1,*tran2;
	int i,j;
	for(i=0;i<size1;i++){
		o=&Sender_Table[i];
		for(j=0;j<num_Records;j++){
			start1=&(o->sender_bucketptr[j]);
			if(start1->walletIdptr==NULL)
				break;
			tran1=start1->transaction_nodeptr;
			tran2=tran1->next;
			while(1){
				free(tran1);
				tran1=tran2;
				if(tran1==NULL)
					break;
				tran2=tran2->next;
			}
		}
	}
	for(i=0;i<size1;i++)
		free(Sender_Table[i].sender_bucketptr);
	free(Sender_Table);
	for(i=0;i<size2;i++){
		o=&Receiver_Table[i];
		for(j=0;j<num_Records;j++){
			start1=&(o->sender_bucketptr[j]);
			if(start1->walletIdptr==NULL)
				break;
			tran1=start1->transaction_nodeptr;
			tran2=tran1->next;
			while(1){
				free(tran1);
				tran1=tran2;
				if(tran1==NULL)
					break;
				tran2=tran2->next;
			}
		}
	}
	for(i=0;i<size2;i++)
		free(Receiver_Table[i].sender_bucketptr);
	free(Receiver_Table);
}