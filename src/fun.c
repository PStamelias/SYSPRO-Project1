#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "fun.h"
#ifndef __structs_h_
#define __structs_h_

#include "struct.h"
#endif
#define PRIME 401
#define AX 2
#define BX 7
int isvalid(char* curr_date,char* curr_time,char* time1,char* year1,char* time2,char* year2){/*Function checking for dates in findEarnings and findPayments*/
	int k;
	int enter=0;
	for(k=6;k<10;k++){
		if(curr_date[k]>year1[k]){
			enter=1;
			break;
		}
		else if(curr_date[k]==year1[k])
			continue;
		else 
			return 0;
	}
	if(enter==0){
		for(k=3;k<5;k++){
			if(curr_date[k]>year1[k]){
				enter=1;
				break;
			}
			else if(curr_date[k]==year1[k])
				continue;
			else 
				return 0;
		}
	}
	if(enter==0){
		for(k=0;k<2;k++){
			if(curr_date[k]>year1[k]){
				enter=1;
				break;
			}
			else if(curr_date[k]==year1[k])
				continue;
			else 
				return 0;
		}
	}
	if(enter==0){
		for(k=0;k<2;k++){
			if(curr_time[k]>time1[k]){
				enter=1;
				break;
			}
			else if(curr_time[k]==time1[k])
				continue;
			else
				break;
		}
	}
	if(enter==0){
		for(k=3;k<5;k++){
			if(curr_time[k]>time1[k]){
				enter=1;
				break;
			}
			else if(curr_time[k]==time1[k])
				continue;
			else
				break;
		}
	}
	if(enter==0)
		return 0;
	enter=0;
	for(k=6;k<10;k++){
		if(curr_date[k]<year2[k]){
			enter=1;
			break;
		}
		else if(curr_date[k]==year2[k])
			continue;
		else 
			return 0;
	}
	if(enter==1)
		return 1;
	if(enter==0){
		for(k=3;k<5;k++){
			if(curr_date[k]<year2[k]){
				enter=1;
				break;
			}
			else if(curr_date[k]==year2[k])
				continue;
			else 
				return 0;
		}
	}
	if(enter==1)
		return 1;
	if(enter==0){
		for(k=0;k<2;k++){
			if(curr_date[k]<year2[k]){
				enter=1;
				break;
			}
			else if(curr_date[k]==year2[k])
				continue;
			else 
				return 0;
		}
	}
	if(enter==1)
		return 1;
	if(enter==0){
		for(k=0;k<2;k++){
			if(curr_time[k]<time2[k]){
				enter=1;
				break;
			}
			else if(curr_time[k]==time1[k])
				continue;
			else
				break;
		}
	}
	if(enter==1)
		return 1;
	if(enter==0){
		for(k=3;k<5;k++){
			if(curr_time[k]<time2[k]){
				enter=1;
				break;
			}
			else if(curr_time[k]==time1[k])
				continue;
			else
				break;
		}
	}
	return 1;
}
int hash_number_char(char* symbol,int buckets){/*Hash Function for Names*/
	char current;
	int h=0;
	int a=10;
	int i;
	int result;
	int size=strlen(symbol)+1;
	for(i=0;i<size;i++){
		current=symbol[i];
		h+=(h*a+current)%PRIME;
	}
	return h%buckets;
}
int hash_number_int(int id,int M){/*Hash Function for BitCoin Ids*/
	return ((AX*id+BX)%PRIME)%M;
}
void insert_in_Hash_Table_of_BitCoin(int position,int value,struct BitCoinInfo** BitTable,char name[],int bitCoinValue){
	struct BitCoinInfo* bitCoinInfo=malloc(sizeof(struct BitCoinInfo));
	bitCoinInfo->bitCoinVal=value;
	bitCoinInfo->next=NULL;
	bitCoinInfo->number_of_transaction=0;
	struct node* Node=malloc(sizeof(struct node));
	Node->value=bitCoinValue;
	Node->right=NULL;
	Node->left=NULL;
	strcpy(Node->name,name);
	bitCoinInfo->riza=Node;
	if(BitTable[position]==NULL)
		BitTable[position]=bitCoinInfo;
	else{
		struct BitCoinInfo* start=BitTable[position];
		while(1){
			if(start->next==NULL){
				start->next=bitCoinInfo;
				break;
			}
			start=start->next;
		}
	}
}
struct node* riza_Node(int key,struct BitCoinInfo** BitTable,int size){
	int position=hash_number_int(key,size);
	struct BitCoinInfo* start=BitTable[position];
	while(1){
		if(start->bitCoinVal==key)
			return start->riza;
		start=start->next;
	}
}
struct BitCoinInfo* BitCoinInfo_Node(int key,struct BitCoinInfo** BitTable,int size){
	int position=hash_number_int(key,size);
	struct BitCoinInfo* start=BitTable[position];
	while(1){
		if(start->bitCoinVal==key)
			return start;
		start=start->next;
	}
}
void insert_in_list_wallet(struct walletId* walletId,int* table,int size,int bit_val){
	int i;
	for(i=0;i<size;i++){
		struct bitCoinId* BitCoinId=malloc(sizeof(struct bitCoinId));
		BitCoinId->bitCoinId_num=table[i];
		BitCoinId->BitCoin_Val=bit_val;
		BitCoinId->node_ptr=NULL;
		BitCoinId->next=NULL;
		if(i==0)
			walletId->bit_list=BitCoinId;
		else{
			struct bitCoinId* start=walletId->bit_list;
			while(1){
				if(start->next==NULL){
					start->next=BitCoinId;
					break;
				}
				start=start->next;
			}
		}
	}
}
void free_list(struct walletId* walletId){
	struct bitCoinId* b=walletId->bit_list;
	if(b==NULL)
		return ;
	struct bitCoinId* a=b->next;
	while(1){
		free(b);
		if(a==NULL)
			break;
		b=a;
		a=b->next;
	}
}
void free_tree(struct node* node){
	if(node->left!=NULL)
		free_tree(node->left);
	if(node->right!=NULL)
		free_tree(node->right);
	if(node!=NULL)
		free(node);
}