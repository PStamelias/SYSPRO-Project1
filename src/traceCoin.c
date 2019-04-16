#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "fun.h"
#ifndef __structs_h_
#define __structs_h_

#include "struct.h"
#endif
#include "traceCoin.h"
void traceCoin(int bitCoinID,struct transaction* Transaction,struct sender_bucket* Sender_Hash_Table,int size,int num_Records){
	int i,j;/*Loop Variables*/
	for(i=0;i<size;i++){
		struct sender_bucket* sender_bucketptr=&Sender_Hash_Table[i];
		for(j=0;j<num_Records;j++){
			if(sender_bucketptr->sender_bucketptr[j].walletIdptr==NULL)
				break;
			else{
				struct transaction_node* start=sender_bucketptr->sender_bucketptr[j].transaction_nodeptr;
				while(1){
					if(start->Bit_Coin_Id==bitCoinID){
						printf("%s ",start->transaction_ptr->transactionId);
						printf("%s ",start->transaction_ptr->senderWalletId);
						printf("%s ",start->transaction_ptr->receiverWalletId);
						printf("%d ",start->transaction_ptr->value);
						printf("%s ",start->transaction_ptr->date);
						printf("%s\n",start->transaction_ptr->time);
					}
					if(start->next==NULL)
						break;
					start=start->next;
				}
			}
		}
	}
}