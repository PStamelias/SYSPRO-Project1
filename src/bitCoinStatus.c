#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "fun.h"
#ifndef __structs_h_
#define __structs_h_

#include "struct.h"
#endif
#include "bitCoinStatus.h"
void bitCoinStatus(int bitCoinID,struct BitCoinInfo** Bit_Table,int size,int start_value){
	int position=hash_number_int(bitCoinID,size);
	struct BitCoinInfo* BitCoinInfo_ptr=Bit_Table[position];
	struct node* node_ptr=NULL;
	while(1){
		if(BitCoinInfo_ptr->bitCoinVal==bitCoinID)
			break;
		BitCoinInfo_ptr=BitCoinInfo_ptr->next;
	}
	node_ptr=BitCoinInfo_ptr->riza;
	int end_value;
	if(node_ptr->right==NULL)
		end_value=0;
	else{
		while(1){
			if(node_ptr->right==NULL){
				if(node_ptr->left==NULL)
					end_value=node_ptr->value;
				else
					end_value=0;
				break;
			}
			node_ptr=node_ptr->right;
		}
	}
	printf("%d %d %d\n",bitCoinID,BitCoinInfo_ptr->number_of_transaction,end_value);
}