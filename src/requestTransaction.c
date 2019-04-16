#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "fun.h"
#ifndef __structs_h_
#define __structs_h_

#include "struct.h"
#endif
#include "requestTransaction.h"
void requestTransaction1(char* sender,char* receiver,struct walletId* wallet,struct sender_bucket* Sender_Table,struct sender_bucket* Receiver_Table,int size_of_wallet,int sender_size,int receiver_size,int num_Records,struct transaction* Transaction,int number_of_lines_of_transcationsFile,int size_of_Bit_Table,struct BitCoinInfo**Bit_Table){
	int i,j;
	int pos=-1;
	int send=-1;
	for(i=0;i<size_of_wallet;i++)
		if(strcmp(sender,wallet[i].userID)==0){
				pos=i;
				break;
			}
	for(i=0;i<size_of_wallet;i++)
		if(strcmp(receiver,wallet[i].userID)==0){
				send=i;
				break;
		}
	if(pos==-1){
		printf("%s\n","Sender user does not exist");
		return ;
	}
	if(send==-1){
		printf("%s\n","Receiver user does not exist");
		return ;
	}
	if(wallet[pos].balance<Transaction[number_of_lines_of_transcationsFile-1].value){
			printf("%s\n","Transaction Error:Sender user does not have the specific amount of money");
			return ;
	}
	int thesi=hash_number_char(wallet[pos].userID,sender_size);
	struct sender* sen=NULL;
	struct sender* re=NULL;
	struct sender_bucket* bucketptr=&Sender_Table[thesi];
	int exit1=0;
	int found1=0;
	while(1){
			for(int k=0;k<num_Records;k++){
			if(bucketptr->sender_bucketptr[k].walletIdptr==NULL){
				exit1=1;
				break;
			}
			else{
				if(strcmp(wallet[pos].userID,bucketptr->sender_bucketptr[k].walletIdptr->userID)==0){
					sen=&bucketptr->sender_bucketptr[k];
					found1=1;
					exit1=1;
					break;
				}
			}
		}
		if(exit1==1)
			break;
		if(bucketptr->next==NULL)
			break;
		bucketptr=bucketptr->next;
	}
	if(found1==0){
		int exit2=0;
		int new_node=0;
		bucketptr=&Sender_Table[thesi];
		while(1){
			for(int k=0;k<num_Records;k++){
				if(bucketptr->sender_bucketptr[k].walletIdptr==NULL){
					bucketptr->sender_bucketptr[k].walletIdptr=&wallet[pos];
					exit2=1;
					sen=&bucketptr->sender_bucketptr[k];
					break;
				}
			}
			if(exit2==1)
				break;
			if(bucketptr->next==NULL){
				new_node=1;
				break;
			}
			bucketptr=bucketptr->next;
		}
		if(new_node==1){
			struct sender_bucket* b=malloc(sizeof(struct sender_bucket));
			b->next=NULL;
			b->sender_bucketptr=malloc(num_Records*sizeof(struct sender));
			for(int y=0;y<num_Records;y++){
				b->sender_bucketptr[y].walletIdptr=NULL;
				b->sender_bucketptr[y].transaction_nodeptr=NULL;
			}
			b->sender_bucketptr[0].walletIdptr=&wallet[pos];
			sen=&	b->sender_bucketptr[0];
			bucketptr->next=b;
		}
	}
	thesi=hash_number_char(wallet[send].userID,receiver_size);
	bucketptr=&Receiver_Table[thesi];
	exit1=0;
	found1=0;
	while(1){
		for(int k=0;k<num_Records;k++){
			if(bucketptr->sender_bucketptr[k].walletIdptr==NULL){
				exit1=1;
				break;
			}
			else{
				if(strcmp(wallet[send].userID,bucketptr->sender_bucketptr[k].walletIdptr->userID)==0){
					re=&bucketptr->sender_bucketptr[k];
					found1=1;
					exit1=1;
					break;
				}
			}
		}
		if(exit1==1)
			break;
		if(bucketptr->next==NULL)
				break;
		bucketptr=bucketptr->next;
	}
	if(found1==0){
		int exit2=0;
		int new_node=0;
		bucketptr=&Receiver_Table[thesi];
		while(1){
			for(int k=0;k<num_Records;k++){
				if(bucketptr->sender_bucketptr[k].walletIdptr==NULL){
					bucketptr->sender_bucketptr[k].walletIdptr=&wallet[send];
					exit2=1;
					re=&bucketptr->sender_bucketptr[k];
					break;
				}
			}
			if(exit2==1)
				break;
			if(bucketptr->next==NULL){
				new_node=1;
				break;
			}
			bucketptr=bucketptr->next;
		}
		if(new_node==1){
			struct sender_bucket* b=malloc(sizeof(struct sender_bucket));
			b->next=NULL;
			b->sender_bucketptr=malloc(num_Records*sizeof(struct sender));
			for(int y=0;y<num_Records;y++){
				b->sender_bucketptr[y].walletIdptr=NULL;
				b->sender_bucketptr[y].transaction_nodeptr=NULL;
			}
			b->sender_bucketptr[0].walletIdptr=&wallet[send];
			re=&b->sender_bucketptr[0];
			bucketptr->next=b;
		}
	}
	int val=Transaction[number_of_lines_of_transcationsFile-1].value;
	wallet[pos].balance-=Transaction[number_of_lines_of_transcationsFile-1].value;
	wallet[send].balance+=Transaction[number_of_lines_of_transcationsFile-1].value;
	for(;val!=0;){
			struct transaction_node* transaction_sen=malloc(sizeof(struct transaction_node));
			struct transaction_node* transaction_re=malloc(sizeof(struct transaction_node));
			transaction_sen->next=NULL;
			transaction_sen->transaction_ptr=&Transaction[number_of_lines_of_transcationsFile-1];
			transaction_re->next=NULL;
			transaction_re->transaction_ptr=&Transaction[number_of_lines_of_transcationsFile-1];
			struct bitCoinId* m=wallet[pos].bit_list;
			transaction_sen->Bit_Coin_Id=m->bitCoinId_num;
			transaction_re->Bit_Coin_Id=m->bitCoinId_num;
			struct node* b=malloc(sizeof(struct node));
			b->right=NULL;
			b->left=NULL;
			strcpy(b->name,wallet[send].userID);
			transaction_sen->node_ptr=b;
			transaction_re->node_ptr=b;
			if(sen->transaction_nodeptr==NULL)
				sen->transaction_nodeptr=transaction_sen;
			else{
				struct transaction_node* n=sen->transaction_nodeptr;
				while(1){
					if(n->next==NULL){
						n->next=transaction_sen;
						break;
					}
					n=n->next;
				}
			}
			if(re->transaction_nodeptr==NULL)
				re->transaction_nodeptr=transaction_re;
			else{
				struct transaction_node* n=re->transaction_nodeptr;
				while(1){
					if(n->next==NULL){
						n->next=transaction_re;
						break;
					}
					n=n->next;
				}
			}
			struct BitCoinInfo* x=BitCoinInfo_Node(m->bitCoinId_num,Bit_Table,size_of_Bit_Table);
			x->number_of_transaction++;
			if(val<m->BitCoin_Val){
				b->value=val;
				m->node_ptr->left=b;
				struct node* c=malloc(sizeof(struct node));
				c->left=NULL;
				c->right=NULL;
				c->value=m->BitCoin_Val-val;
				strcpy(c->name,wallet[pos].userID);
				m->node_ptr->right=c;
				m->node_ptr=c;
				m->BitCoin_Val-=val;
				struct bitCoinId* o=malloc(sizeof(struct bitCoinId));
				o->next=NULL;
				o->BitCoin_Val=val;
				o->bitCoinId_num=m->bitCoinId_num;
				o->node_ptr=b;
				struct bitCoinId* i=wallet[send].bit_list;
				val=0;
				if(i==NULL)
					wallet[send].bit_list=o;
				else{
					while(1){
						if(i->next==NULL){
							i->next=o;
							break;
						}
						i=i->next;
					}
				}
			}
			else if(val>=m->BitCoin_Val){
				b->value=m->BitCoin_Val;
				m->node_ptr->left=b;
				struct bitCoinId* tr=malloc(sizeof(struct bitCoinId));
				tr->next=NULL;
				tr->bitCoinId_num=m->bitCoinId_num;
				tr->node_ptr=b;
				tr->BitCoin_Val=m->BitCoin_Val;
				struct bitCoinId* e=wallet[send].bit_list;
				val=val-m->BitCoin_Val;
				if(e==NULL)
					e=tr;
				else{
					while(1){
						if(e->next==NULL){
							e->next=tr;
							break;
						}
						e=e->next;
					}
				}
				struct bitCoinId* o=wallet[pos].bit_list;	
				struct bitCoinId * io=o->next;
				wallet[pos].bit_list=io;
				free(o);
			}
	}
}


void requestTransaction2(int start,int number_of_lines_of_transcationsFile,struct walletId* wallet,struct sender_bucket* Sender_Table,struct sender_bucket* Receiver_Table,int number_of_lines_of_bitCoinBalancesFiles,struct transaction* Transaction,int sender_size,int receiver_size,int num_Records,int size_of_Bit_Table,struct BitCoinInfo** Bit_Table){
	int i;
	char user1[50],user2[50];
	for(i=start;i<number_of_lines_of_transcationsFile;i++){
		strcpy(user1,Transaction[i].senderWalletId);
		strcpy(user2,Transaction[i].receiverWalletId);
		int pos=-1;
		int send=-1;
		for(int j=0;j<number_of_lines_of_bitCoinBalancesFiles;j++)
			if(strcmp(user1,wallet[j].userID)==0){
				pos=j;
				break;
			}
		for(int j=0;j<number_of_lines_of_bitCoinBalancesFiles;j++)
			if(strcmp(user2,wallet[j].userID)==0){
				send=j;
				break;
			}
		if(wallet[pos].balance<Transaction[i].value){
			printf("%s\n","Transaction Error:Sender user does not have the specific amount of money");
			return ;
		}
		int thesi=hash_number_char(wallet[pos].userID,sender_size);
		struct sender* sen=NULL;
		struct sender* re=NULL;
		struct sender_bucket* bucketptr=&Sender_Table[thesi];
		int exit1=0;
		int found1=0;
		while(1){
			for(int k=0;k<num_Records;k++){
				if(bucketptr->sender_bucketptr[k].walletIdptr==NULL){
					exit1=1;
					break;
				}
				else{
					if(strcmp(wallet[pos].userID,bucketptr->sender_bucketptr[k].walletIdptr->userID)==0){
						sen=&bucketptr->sender_bucketptr[k];
						found1=1;
						exit1=1;
						break;
					}
				}
			}
			if(exit1==1)
				break;
			if(bucketptr->next==NULL)
				break;
			bucketptr=bucketptr->next;
		}
		if(found1==0){
			int exit2=0;
			int new_node=0;
			bucketptr=&Sender_Table[thesi];
			while(1){
				for(int k=0;k<num_Records;k++){
					if(bucketptr->sender_bucketptr[k].walletIdptr==NULL){
						bucketptr->sender_bucketptr[k].walletIdptr=&wallet[pos];
						exit2=1;
						sen=&bucketptr->sender_bucketptr[k];
						break;
					}
				}
				if(exit2==1)
					break;
				if(bucketptr->next==NULL){
					new_node=1;
					break;
				}
				bucketptr=bucketptr->next;
			}
			if(new_node==1){
				struct sender_bucket* b=malloc(sizeof(struct sender_bucket));
				b->next=NULL;
				b->sender_bucketptr=malloc(num_Records*sizeof(struct sender));
				for(int y=0;y<num_Records;y++){
					b->sender_bucketptr[y].walletIdptr=NULL;
					b->sender_bucketptr[y].transaction_nodeptr=NULL;
				}
				b->sender_bucketptr[0].walletIdptr=&wallet[pos];
				sen=&	b->sender_bucketptr[0];
				bucketptr->next=b;
			}
		}
		thesi=hash_number_char(wallet[send].userID,receiver_size);
		bucketptr=&Receiver_Table[thesi];
		exit1=0;
		found1=0;
		while(1){
			for(int k=0;k<num_Records;k++){
				if(bucketptr->sender_bucketptr[k].walletIdptr==NULL){
					exit1=1;
					break;
				}
				else{
					if(strcmp(wallet[send].userID,bucketptr->sender_bucketptr[k].walletIdptr->userID)==0){
						re=&bucketptr->sender_bucketptr[k];
						found1=1;
						exit1=1;
						break;
					}
				}
			}
			if(exit1==1)
				break;
			if(bucketptr->next==NULL)
				break;
			bucketptr=bucketptr->next;
		}
		if(found1==0){
			int exit2=0;
			int new_node=0;
			bucketptr=&Receiver_Table[thesi];
			while(1){
				for(int k=0;k<num_Records;k++){
					if(bucketptr->sender_bucketptr[k].walletIdptr==NULL){
						bucketptr->sender_bucketptr[k].walletIdptr=&wallet[send];
						exit2=1;
						re=&bucketptr->sender_bucketptr[k];
						break;
					}
				}
				if(exit2==1)
					break;
				if(bucketptr->next==NULL){
					new_node=1;
					break;
				}
				bucketptr=bucketptr->next;
			}
			if(new_node==1){
				struct sender_bucket* b=malloc(sizeof(struct sender_bucket));
				b->next=NULL;
				b->sender_bucketptr=malloc(num_Records*sizeof(struct sender));
				for(int y=0;y<num_Records;y++){
					b->sender_bucketptr[y].walletIdptr=NULL;
					b->sender_bucketptr[y].transaction_nodeptr=NULL;
				}
				b->sender_bucketptr[0].walletIdptr=&wallet[send];
				re=&b->sender_bucketptr[0];
				bucketptr->next=b;
			}
		}
		int val=Transaction[i].value;
		wallet[pos].balance-=Transaction[i].value;
		wallet[send].balance+=Transaction[i].value;
		for(;val!=0;){
			struct transaction_node* transaction_sen=malloc(sizeof(struct transaction_node));
			struct transaction_node* transaction_re=malloc(sizeof(struct transaction_node));
			transaction_sen->next=NULL;
			transaction_sen->transaction_ptr=&Transaction[i];
			transaction_re->next=NULL;
			transaction_re->transaction_ptr=&Transaction[i];
			struct bitCoinId* m=wallet[pos].bit_list;
			transaction_sen->Bit_Coin_Id=m->bitCoinId_num;
			transaction_re->Bit_Coin_Id=m->bitCoinId_num;
			struct node* b=malloc(sizeof(struct node));
			b->right=NULL;
			b->left=NULL;
			strcpy(b->name,wallet[send].userID);
			transaction_sen->node_ptr=b;
			transaction_re->node_ptr=b;
			if(sen->transaction_nodeptr==NULL)
				sen->transaction_nodeptr=transaction_sen;
			else{
				struct transaction_node* n=sen->transaction_nodeptr;
				while(1){
					if(n->next==NULL){
						n->next=transaction_sen;
						break;
					}
					n=n->next;
				}
			}
			if(re->transaction_nodeptr==NULL)
				re->transaction_nodeptr=transaction_re;
			else{
				struct transaction_node* n=re->transaction_nodeptr;
				while(1){
					if(n->next==NULL){
						n->next=transaction_re;
						break;
					}
					n=n->next;
				}
			}
			struct BitCoinInfo* x=BitCoinInfo_Node(m->bitCoinId_num,Bit_Table,size_of_Bit_Table);
			x->number_of_transaction++;
			if(val<m->BitCoin_Val){
				b->value=val;
				m->node_ptr->left=b;
				struct node* c=malloc(sizeof(struct node));
				c->left=NULL;
				c->right=NULL;
				c->value=m->BitCoin_Val-val;
				strcpy(c->name,wallet[pos].userID);
				m->node_ptr->right=c;
				m->node_ptr=c;
				m->BitCoin_Val-=val;
				struct bitCoinId* o=malloc(sizeof(struct bitCoinId));
				o->next=NULL;
				o->BitCoin_Val=val;
				o->bitCoinId_num=m->bitCoinId_num;
				o->node_ptr=b;
				struct bitCoinId* i=wallet[send].bit_list;
				val=0;
				if(i==NULL)
					wallet[send].bit_list=o;
				else{
					while(1){
						if(i->next==NULL){
							i->next=o;
							break;
						}
						i=i->next;
					}
				}
			}
			else if(val>=m->BitCoin_Val){
				b->value=m->BitCoin_Val;
				m->node_ptr->left=b;
				struct bitCoinId* tr=malloc(sizeof(struct bitCoinId));
				tr->next=NULL;
				tr->bitCoinId_num=m->bitCoinId_num;
				tr->node_ptr=b;
				tr->BitCoin_Val=m->BitCoin_Val;
				struct bitCoinId* e=wallet[send].bit_list;
				val=val-m->BitCoin_Val;
				if(e==NULL)
					e=tr;
				else{
					while(1){
						if(e->next==NULL){
							e->next=tr;
							break;
						}
						e=e->next;
					}
				}
				struct bitCoinId* o=wallet[pos].bit_list;	
				struct bitCoinId * io=o->next;
				wallet[pos].bit_list=io;
				free(o);
			}
		}
	}
}






















/*Transaction=realloc(Transaction,(number_of_lines_of_transcationsFile+1)*sizeof(struct transaction));
	strcpy(Transaction[number_of_lines_of_transcationsFile].date,date);
	strcpy(Transaction[number_of_lines_of_transcationsFile].time,time);
	Transaction[number_of_lines_of_transcationsFile].value=amount;
	Transaction[number_of_lines_of_transcationsFile].senderWalletId=malloc((strlen(sender)+1)*sizeof(char));
	strcpy(Transaction[number_of_lines_of_transcationsFile].senderWalletId,sender);
	Transaction[number_of_lines_of_transcationsFile].receiverWalletId=malloc((strlen(receiver)+1)*sizeof(char));
	strcpy(Transaction[number_of_lines_of_transcationsFile].receiverWalletId,receiver);
	return ;
	int new_node;
	for(i=0;i<size_of_wallet;i++){
		if(!strcmp(wallet[i].userID,sender))
			pos=i;
		if(!strcmp(wallet[i].userID,receiver))
			send=i;
	}
	if(wallet[pos].balance<amount){
		printf("%s\n","Error:Sender does not have the requested amount of money");
		return ;
	}
	if(pos==-1){
		printf("%s\n","Given sender does note exist");
		return ;
	}
	if(send==-1){
		printf("%s\n","Given receiver does not exist");
		return ;
	}
	int thesi=hash_number_char(wallet[pos].userID,sender_size);
	struct sender* sen=NULL;
	struct sender* re=NULL;
	struct sender_bucket* bucketptr=&Sender_Table[thesi];
	int exit1=0;
	int found1=0;
	while(1){
		for(k=0;k<num_Records;k++){
			if(bucketptr->sender_bucketptr[k].walletIdptr==NULL){
				exit1=1;
				break;
			}
			else{
				if(strcmp(wallet[pos].userID,bucketptr->sender_bucketptr[k].walletIdptr->userID)==0){
					sen=&bucketptr->sender_bucketptr[k];
					found1=1;
					exit1=1;
					break;
				}
			}
		}
		if(exit1==1)
			break;
		if(bucketptr->next==NULL)
			break;
		bucketptr=bucketptr->next;
	}
	if(found1==0){
		int exit2=0;
		new_node=0;
		bucketptr=&Sender_Table[thesi];
		while(1){
			for(k=0;k<num_Records;k++){
				if(bucketptr->sender_bucketptr[k].walletIdptr==NULL){
					bucketptr->sender_bucketptr[k].walletIdptr=&wallet[pos];
					exit2=1;
					sen=&bucketptr->sender_bucketptr[k];
					break;
				}
			}
			if(exit2==1)
				break;
			if(bucketptr->next==NULL){
				new_node=1;
				break;
			}
			bucketptr=bucketptr->next;
		}
		if(new_node==1){
			struct sender_bucket* b=malloc(sizeof(struct sender_bucket));
			b->next=NULL;
			b->sender_bucketptr=malloc(num_Records*sizeof(struct sender));
			for(int y=0;y<num_Records;y++){
				b->sender_bucketptr[y].walletIdptr=NULL;
				b->sender_bucketptr[y].transaction_nodeptr=NULL;
			}
			b->sender_bucketptr[0].walletIdptr=&wallet[pos];
			sen=&	b->sender_bucketptr[0];
			bucketptr->next=b;
		}
	}
	thesi=hash_number_char(wallet[send].userID,receiver_size);
	bucketptr=&Receiver_Table[thesi];
	exit1=0;
	found1=0;
	while(1){
		for(k=0;k<num_Records;k++){
			if(bucketptr->sender_bucketptr[k].walletIdptr==NULL){
				exit1=1;
				break;
			}
			else{
				if(strcmp(wallet[send].userID,bucketptr->sender_bucketptr[k].walletIdptr->userID)==0){
					re=&bucketptr->sender_bucketptr[k];
					found1=1;
					exit1=1;
					break;
				}
			}
		}
		if(exit1==1)
			break;
		if(bucketptr->next==NULL)
			break;
		bucketptr=bucketptr->next;
	}
	if(found1==0){
		int exit2=0;
		new_node=0;
		bucketptr=&Receiver_Table[thesi];
		while(1){
			for(k=0;k<num_Records;k++){
				if(bucketptr->sender_bucketptr[k].walletIdptr==NULL){
					bucketptr->sender_bucketptr[k].walletIdptr=&wallet[send];
					exit2=1;
					re=&bucketptr->sender_bucketptr[k];
					break;
				}
			}
			if(exit2==1)
				break;
			if(bucketptr->next==NULL){
				new_node=1;
				break;
			}
			bucketptr=bucketptr->next;
		}
		if(new_node==1){
			struct sender_bucket* b=malloc(sizeof(struct sender_bucket));
			b->next=NULL;
			b->sender_bucketptr=malloc(num_Records*sizeof(struct sender));
			for(int y=0;y<num_Records;y++){
				b->sender_bucketptr[y].walletIdptr=NULL;
				b->sender_bucketptr[y].transaction_nodeptr=NULL;
			}
			b->sender_bucketptr[0].walletIdptr=&wallet[send];
			re=&b->sender_bucketptr[0];
			bucketptr->next=b;
		}
	}
	printf("%s\n",sen->walletIdptr->userID);
	printf("%s\n",re->walletIdptr->userID);
	wallet[pos].balance-=amount;
	wallet[send].balance+=amount;
	for(;amount!=0;){

	}*/