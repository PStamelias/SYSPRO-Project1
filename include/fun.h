#ifndef __structs_h_
#define __structs_h_
#include "struct.h"
#endif	
int isvalid(char* curr_date,char* curr_time,char* time1,char* year1,char* time2,char* year2);
int hash_number_char(char* symbol,int buckets);
int hash_number_int(int id,int M);
void insert_in_Hash_Table_of_BitCoin(int position,int value,struct BitCoinInfo** BitTable,char name[],int bitCoinValue);
struct node* riza_Node(int key,struct BitCoinInfo** BitTable,int size);
struct BitCoinInfo* BitCoinInfo_Node(int key,struct BitCoinInfo** BitTable,int size);
void insert_in_list_wallet(struct walletId*  walletId,int* table,int size,int bit_val);
void free_list(struct walletId* walletId);
void free_tree(struct node* node);