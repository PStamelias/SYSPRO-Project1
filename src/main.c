/*main.c*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#ifndef __structs_h_
#define __structs_h_

#include "struct.h"
#endif
#ifndef __fun_h_
#define __fun_h_

#include "fun.h"
#endif
#ifndef __walletStatus_h_
#define __walletStatus_h_

#include "walletStatus.h"
#endif
#ifndef __bitCoinStatus_h_
#define __bitCoinStatus_h_

#include "bitCoinStatus.h"
#endif
#ifndef __traceCoin_h_
#define __traceCoin_h_

#include "traceCoin.h"
#endif
#ifndef __exit_h
#define __exit_h

#include "exit.h"
#endif

#ifndef __findPayments_h_
#define __findPayments_h_

#include "findPayments.h"
#endif
#ifndef __findEarnings_h_
#define __findEarnings_h_

#include "findEarnings.h"
#endif
#ifndef __requestTransaction_h_
#define __requestTransaction_h_

#include "requestTransaction.h"
#endif
int main(int argc,char** argv)
{
	int i,j,k;/*Loop Variables*/
	int f1=0,f2=0,v=0;
	FILE* file1,*file2;
	int rH=0,sH=0;
	char user1[50];
	int new_node;
	char user2[50];
	int b=0;
	int bitCoinValue;
	int c;
	int senderHashtable1NumOfEntries,receiverHashtable1NumOfEntries;
	int bucketSize;
	char curr_date[]="00-00-0000";
	char curr_time[]="00:00";
	char* bitCoinBalancesFile,*transactionsFile;
	int exit;
	int r,e,f;
	int enter=0;
	int val;
	int pos,send;
	int thesi,found1;
	if(argc!=13){
		printf("Wrong given arguments\n");
		return EXIT_FAILURE;
	}
	for(i=1;;++i){
		if((strcmp(argv[i],"-a")==0)&&f1==0){
			f1=1;
			bitCoinBalancesFile=malloc((strlen(argv[i+1])+1)*sizeof(char));
			strcpy(bitCoinBalancesFile,argv[i+1]);
		}
		if((strcmp(argv[i],"-t")==0)&&f2==0){
			f2=1;
			transactionsFile=malloc((strlen(argv[i+1]+1)*sizeof(char)));
			strcpy(transactionsFile,argv[i+1]);
		}
		if((strcmp(argv[i],"-v")==0)&&v==0){
			v=1;
			bitCoinValue=atoi(argv[i+1]);
		}
		if(strcmp(argv[i],"-h1")==0&&sH==0){
			sH=1;
			senderHashtable1NumOfEntries=atoi(argv[i+1]);
		}
		if((strcmp(argv[i],"-h2")==0)&&rH==0){
			rH=1;
			receiverHashtable1NumOfEntries=atoi(argv[i+1]);
		}
		if((strcmp(argv[i],"-b")==0)&&b==0){
			b=1;
			bucketSize=atoi(argv[i+1]);
		}
		if(f1==1&&f2==1&&b==1&&v==1&&rH==1&&sH==1)
			break;
	}
	if((file1=fopen(bitCoinBalancesFile,"r"))==NULL){
		perror("File1 Error\n");	
		return EXIT_FAILURE;
	}
	if((file2=fopen(transactionsFile,"r"))==NULL){
		perror("File2 Error");
		return EXIT_FAILURE;
	}
	/* WalletId Data Reading*/
	int number_of_lines_of_bitCoinBalancesFiles=0;
	while((c=fgetc(file1))!=EOF){
		if(c=='\n')
			number_of_lines_of_bitCoinBalancesFiles++;
	}
	rewind(file1);
	struct walletId* wallet=malloc(number_of_lines_of_bitCoinBalancesFiles*sizeof(struct walletId));/*Struct for users*/
	int sum=1;
	for(i=0;i<number_of_lines_of_bitCoinBalancesFiles;i++){
		int counter1=0;
		while(1){
			c=fgetc(file1);
			if(c==' '||c=='\t'||c=='\n'){
				wallet[i].userID[counter1]='\0';
				break;
			}
			else if(c>='A'&&c<='Z'||c>='a'&&c<='z')
				wallet[i].userID[counter1++]=c;
		}
		if(c=='\n'){/*Specific user does not have BitCoins*/
			wallet[i].balance=0;
			wallet[i].bit_list=NULL;
			continue;
		}
		int size_of_BitCoinId=1;
		int* bitCoinId=malloc(size_of_BitCoinId*sizeof(int));
		bitCoinId[size_of_BitCoinId-1]=0;
		int enter=1;
		while(1){
			c=fgetc(file1);
			if(c=='\n'){
				for(k=size_of_BitCoinId-1;k>=0;k--)
					if(bitCoinId[k]==0)
						size_of_BitCoinId--;
				break;
			}
			if(c==' '||c=='\t'){
				if(enter==0)
					continue;
				size_of_BitCoinId++;
				bitCoinId=realloc(bitCoinId,size_of_BitCoinId*sizeof(int));
				bitCoinId[size_of_BitCoinId-1]=0;
				enter=0;
			}
			else{
				enter=1;
				bitCoinId[size_of_BitCoinId-1]=bitCoinId[size_of_BitCoinId-1]*10+c-'0';
			}
		}	
		sum+=size_of_BitCoinId;
		wallet[i].balance=size_of_BitCoinId*bitCoinValue;
		wallet[i].size_of_bitCoin=size_of_BitCoinId;
		insert_in_list_wallet(&wallet[i],bitCoinId,size_of_BitCoinId,bitCoinValue);
		free(bitCoinId);
	}
	/////////////////////////////////////////////////////////////////////////
	for(i=0;i<number_of_lines_of_bitCoinBalancesFiles;i++){/*Checking if i have the same wallet userID twice or more in file*/
		for(j=0;j<number_of_lines_of_bitCoinBalancesFiles;j++){
			if(i==j)
				continue;
			if(strcmp(wallet[i].userID,wallet[j].userID)==0){
				printf("Error:Cannot have the same names on file\n");
				return -1;
			}
		}
	}
	int counter1=0;
	int* all_Bit_Coin=malloc(sum*sizeof(int));
	for(i=0;i<number_of_lines_of_bitCoinBalancesFiles;i++){
		struct bitCoinId* a=wallet[i].bit_list;
		for(j=0;j<wallet[i].size_of_bitCoin;j++){
			all_Bit_Coin[counter1++]=a->bitCoinId_num;
			a=a->next;
		}
	}
	for(i=0;i<counter1;i++){
		for(j=0;j<counter1;j++){
			if(i==j)
				continue;
			else
				if(all_Bit_Coin[i]==all_Bit_Coin[j]){
					printf("Error:Cannot have two BitCoins with the same ID\n");
					return -1;
				}
		}
	}
	/*Transaction Data Reading*/
	int number_of_lines_of_transcationsFile=0;
	while((c=fgetc(file2))!=EOF){
		if(c=='\n')
			number_of_lines_of_transcationsFile++;
	}
	rewind(file2);
	struct transaction* Transaction=malloc(number_of_lines_of_transcationsFile*sizeof(struct transaction));
	for(i=0;i<number_of_lines_of_transcationsFile;i++){
		counter1=0;
		Transaction[i].transactionId=malloc((counter1+1)*sizeof(char));
		while(1){
			c=fgetc(file2);
			if(c==' '||c=='\t'){
				Transaction[i].transactionId[counter1]='\0';
				break;
			}
			Transaction[i].transactionId[counter1++]=c;
			Transaction[i].transactionId=realloc(Transaction[i].transactionId,(counter1+1)*sizeof(char));
		}	
		counter1=0;
		Transaction[i].senderWalletId=malloc((counter1+1)*sizeof(char));
		while(1){
			c=fgetc(file2);
			if(c==' '||c=='\t'){
				Transaction[i].senderWalletId[counter1]='\0';
				break;
			}
			Transaction[i].senderWalletId[counter1++]=c;
			Transaction[i].senderWalletId=realloc(Transaction[i].senderWalletId,(counter1+1)*sizeof(char));
		}
		counter1=0;
		Transaction[i].receiverWalletId=malloc((counter1+1)*sizeof(char));
		while(1){
			c=fgetc(file2);
			if(c==' '||c=='\t'){
				Transaction[i].receiverWalletId[counter1]='\0';
				break;
			}
			Transaction[i].receiverWalletId[counter1++]=c;
			Transaction[i].receiverWalletId=realloc(Transaction[i].receiverWalletId,(counter1+1)*sizeof(char));
		}
		Transaction[i].value=0;

		while(1){
			c=fgetc(file2);
			if(c==' '||c=='\t')
				break;
			Transaction[i].value=Transaction[i].value*10+c-'0';
		}
		counter1=0;
		while(1){
			c=fgetc(file2);
			if(c==' '||c=='\t'){
				Transaction[i].date[counter1]='\0';
				break;
			}
			Transaction[i].date[counter1++]=c;
		}
		counter1=0;
		while(1){
			c=fgetc(file2);
			if(c=='\n'){
				Transaction[i].time[counter1]='\0';
				break;
			}
			Transaction[i].time[counter1++]=c;
		}
		if(c=='\n')
			continue;
		else{
			printf("%s\n","Error in format of File");
			return EXIT_FAILURE;
		}
	}
	/////////////////////////////////////////////////////////////////
	for(i=0;i<number_of_lines_of_transcationsFile;i++){
		if(strcmp(Transaction[i].senderWalletId,Transaction[i].receiverWalletId)==0){
			printf("Error:Cannot have the same sender and receiver\n");
			return -1;
		}
	}
	for(i=0;i<number_of_lines_of_transcationsFile;i++){
		char user1[50],user2[50];
		strcpy(user1,Transaction[i].senderWalletId);
		strcpy(user2,Transaction[i].receiverWalletId);
		int pos=-1;
		for(j=0;j<number_of_lines_of_bitCoinBalancesFiles;j++)
			if(strcmp(user2,wallet[j].userID)==0){
				pos=j;
				break;
			}
		if(pos==-1){
			printf("Error:Receiver user does not exist\n");
			return -1;
		}
		pos=-1;
		for(j=0;j<number_of_lines_of_bitCoinBalancesFiles;j++){
			if(strcmp(user1,wallet[j].userID)==0){
				pos=j;
				break;
			}
		}
		if(pos==-1){
			printf("Error:Sender user does not exist\n");
			return -1;
		}
	}
	/*Checking for Date of Transactions*/
	for(i=0;i<number_of_lines_of_transcationsFile;i++){
		enter=0;
		for(k=6;k<10;k++){
			if(curr_date[k]==Transaction[i].date[k])
				continue;
			if(curr_date[k]>Transaction[i].date[k]){
				printf("Error:Transaction date is wrong\n");
				return -1;
			}
			if(curr_date[k]<Transaction[i].date[k]){
				strcpy(curr_date,Transaction[i].date);
				strcpy(curr_time,Transaction[i].time);
				enter=1;
				break;
			}
		}
		if(enter==1)
			continue;
		for(k=3;k<5;k++){
			if(curr_date[k]==Transaction[i].date[k])
				continue;
			if(curr_date[k]>Transaction[i].date[k]){
				printf("Error:Transaction date is wrong\n");
				return -1;
			}
			if(curr_date[k]<Transaction[i].date[k]){
				strcpy(curr_date,Transaction[i].date);
				strcpy(curr_time,Transaction[i].time);
				enter=1;
				break;
			}
		}
		if(enter==1)
			continue;
		for(k=0;k<2;k++){
			if(curr_date[k]==Transaction[i].date[k])
				continue;
			if(curr_date[k]>Transaction[i].date[k]){
				printf("Error:Transaction date is wrong\n");
				return -1;
			}
			if(curr_date[k]<Transaction[i].date[k]){
				strcpy(curr_date,Transaction[i].date);
				strcpy(curr_time,Transaction[i].time);
				enter=1;
				break;
			}
		}
		if(enter==1)
			continue;
		for(k=0;k<2;k++){
			if(curr_time[k]==Transaction[i].time[k])
				continue;
			if(curr_time[k]>Transaction[i].time[k]){
				printf("Error:Transaction time is wrong\n");
				return -1;
			}
			if(curr_time[k]<Transaction[i].time[k]){
				strcpy(curr_date,Transaction[i].date);
				strcpy(curr_time,Transaction[i].time);
				enter=1;
				break;
			}
		}
		if(enter==1)
			continue;
		for(k=3;k<5;k++){
			if(curr_time[k]==Transaction[i].time[k])
				continue;
			if(curr_time[k]>Transaction[i].time[k]){
				printf("Error:Transaction time is wrong\n");
				return -1;
			}
			if(curr_time[k]<Transaction[i].time[k]){
				strcpy(curr_date,Transaction[i].date);
				strcpy(curr_time,Transaction[i].time);
				enter=1;
				break;
			}
		}
		if(enter==0){
			printf("Error:Cannot have two transactions at the same time\n");
			return -1;
		}
	}
	///////////////////////////////////////////////////////////////
	for(i=0;i<number_of_lines_of_transcationsFile;i++){
		for(j=0;j<number_of_lines_of_transcationsFile;j++){
			if(i==j)
				continue;
			else{
				if(strcmp(Transaction[i].transactionId,Transaction[j].transactionId)==0){
					printf("%s\n","Cannot have two transactions with the same ID\n");
					return EXIT_FAILURE;
				}
			}
		}
	}
	/*Initialization of Hash_Table of BitCoins*/
	int size_of_Bit_Table=rand()%sum+1;
	struct BitCoinInfo** Bit_Table=malloc(size_of_Bit_Table*sizeof(struct BitCoinInfo*));
	for(i=0;i<size_of_Bit_Table;i++)
		Bit_Table[i]=NULL;
	for(i=0;i<number_of_lines_of_bitCoinBalancesFiles;i++){
		struct bitCoinId* a=wallet[i].bit_list;
		for(j=0;j<wallet[i].size_of_bitCoin;j++){
			int Bit_value=a->bitCoinId_num;
			int position=hash_number_int(Bit_value,size_of_Bit_Table);
			insert_in_Hash_Table_of_BitCoin(position,Bit_value,Bit_Table,wallet[i].userID,bitCoinValue);
			a=a->next;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if((bucketSize-sizeof(struct sender_bucket*))<=sizeof(struct sender)){
		printf("%s\n","Error:BucketSize");
		return EXIT_FAILURE;
	}
	int num_Records=((bucketSize-sizeof(struct sender_bucket*))/sizeof(struct sender));
	/*Hash Table of Senders initialization*/
	struct sender_bucket* Sender_Table=malloc(senderHashtable1NumOfEntries*bucketSize);
	for(int i=0;i<senderHashtable1NumOfEntries;i++){
		Sender_Table[i].next=NULL;
		Sender_Table[i].sender_bucketptr=malloc(num_Records*sizeof(struct sender));
		for(j=0;j<num_Records;j++){
			Sender_Table[i].sender_bucketptr[j].walletIdptr=NULL;
			Sender_Table[i].sender_bucketptr[j].transaction_nodeptr=NULL;
		}
	}
	///////////////////////////////////////
	for(i=0;i<number_of_lines_of_bitCoinBalancesFiles;i++){
		struct bitCoinId* a=wallet[i].bit_list;
		for(j=0;j<wallet[i].size_of_bitCoin;j++){
			a->node_ptr=riza_Node(a->bitCoinId_num,Bit_Table,size_of_Bit_Table);
			if(a->next==NULL)
				break;
			a=a->next;
		}
	}
	/*Hash Table of Receivers initialization*/
	struct sender_bucket* Receiver_table=malloc(receiverHashtable1NumOfEntries*bucketSize);
	for(int i=0;i<receiverHashtable1NumOfEntries;i++){
		Receiver_table[i].next=NULL;
		Receiver_table[i].sender_bucketptr=malloc(num_Records*sizeof(struct sender));
		for(j=0;j<num_Records;j++){
			Receiver_table[i].sender_bucketptr[j].transaction_nodeptr=NULL;
			Receiver_table[i].sender_bucketptr[j].walletIdptr=NULL;
		}
	}
	////////////////////////////////////////////
	/*Make Transactions of transactionsFile*/
	for(i=0;i<number_of_lines_of_transcationsFile;i++){
		strcpy(user1,Transaction[i].senderWalletId);
		strcpy(user2,Transaction[i].receiverWalletId);
		pos=-1;
		send=-1;
		for(j=0;j<number_of_lines_of_bitCoinBalancesFiles;j++)
			if(strcmp(user1,wallet[j].userID)==0){
				pos=j;
				break;
			}
		for(j=0;j<number_of_lines_of_bitCoinBalancesFiles;j++)
			if(strcmp(user2,wallet[j].userID)==0){
				send=j;
				break;
			}
		if(wallet[pos].balance<Transaction[i].value){
			printf("%s\n","Transaction Error:Sender user does not have the specific amount of money");
			return EXIT_FAILURE;
		}
		/*Finding sender and receiver on HaSH Tables*/
		int thesi=hash_number_char(user1,senderHashtable1NumOfEntries);
		struct sender* re=NULL;
		struct sender* sen=NULL;
		struct sender_bucket* node_bucket=&Sender_Table[thesi];
		int exit=0;
		int found1=0;
		while(1){
			for(k=0;k<num_Records;k++){
				if(node_bucket->sender_bucketptr[k].walletIdptr==NULL){
					exit=1;
					break;
				}
				else{
						if(strcmp(wallet[pos].userID,node_bucket->sender_bucketptr[k].walletIdptr->userID)==0){
							sen=&node_bucket->sender_bucketptr[k];
							found1=1;
							exit=1;
							break;
					}
				}
			}
			if(exit==1)
				break;
			if(node_bucket->next==NULL)
				break;
			node_bucket=node_bucket->next;
		}
		int new_node;
		if(found1==0){
			int exit1=0;
			new_node=0;
			node_bucket=&Sender_Table[thesi];
			while(1){
				for(k=0;k<num_Records;k++){
					if(node_bucket->sender_bucketptr[k].walletIdptr==NULL){
							node_bucket->sender_bucketptr[k].walletIdptr=&wallet[pos];
							exit1=1;
							sen=&node_bucket->sender_bucketptr[k];
							break;
					}
				}
				if(exit1==1)
					break;
				if(node_bucket->next==NULL){
					new_node=1;
					break;
				}
				node_bucket=node_bucket->next;
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
				node_bucket->next=b;
			}
		}
		thesi=hash_number_char(user2,receiverHashtable1NumOfEntries);
		node_bucket=&Receiver_table[thesi];
		found1=0;
		int exit1=0;
		while(1){
			for(k=0;k<num_Records;k++){
				if(node_bucket->sender_bucketptr[k].walletIdptr==NULL){
					exit1=1;
					break;
				}
				if(strcmp(wallet[send].userID,node_bucket->sender_bucketptr[k].walletIdptr->userID)==0){
					re=&node_bucket->sender_bucketptr[k];
					found1=1;
					exit1=1;
					break;
				}
			}
			if(exit1==1)
				break;
			if(node_bucket->next==NULL)
				break;
			node_bucket=node_bucket->next;
		}
		if(found1==0){
			int exit1=0;
			new_node=0;
			node_bucket=&Receiver_table[thesi];
			while(1){
				for(k=0;k<num_Records;k++){
					if(node_bucket->sender_bucketptr[k].walletIdptr==NULL){
						node_bucket->sender_bucketptr[k].walletIdptr=&wallet[send];
						exit1=1;
						re=&node_bucket->sender_bucketptr[k];
						break;
					}
				}
				if(exit1==1)
					break;
				if(node_bucket->next==NULL){
					new_node=1;
					break;
				}
				node_bucket=node_bucket->next;
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
				node_bucket->next=b;
			}
		}
		//////////////////////////////////////////////
		val=Transaction[i].value;
		wallet[pos].balance-=Transaction[i].value;
		wallet[send].balance+=Transaction[i].value;
		/*Simulation of Transaction and update the structs*/
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
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*Menu*/
	char* last_id=malloc((strlen(Transaction[number_of_lines_of_transcationsFile-1].transactionId)+1)*sizeof(char));
	strcpy(last_id,Transaction[number_of_lines_of_transcationsFile-1].transactionId);
	last_id[strlen(last_id)-1]=last_id[strlen(last_id)-1]+'1'-'0';
	char* leksi[6];
	for(int k=0;k<6;k++)
		leksi[k]=malloc(50*sizeof(char));
	while(1){
		int rt=0;
		int thesi=0;
		while(1){
			int e=fgetc(stdin);
			if(thesi<=5){
				if(e=='\n'){
					leksi[thesi][rt]='\0';
					break;
				}
				if(e==' '){
					leksi[thesi][rt]='\0';
					thesi++;
					rt=0;
				}
				if(e!=' '&&thesi!=6)
					leksi[thesi][rt++]=e;
			}
			if(e=='\n')
				break;
		}
		if(strcmp("-/requestTransaction",leksi[0])==0){
			for(int h=1;h<4;h++){
				if(!strcmp(leksi[h],"")){
					printf("%s\n","Error:No Input");
					return EXIT_FAILURE;
				}
			}
			if(!strcmp(leksi[4],"")){
				struct transaction* tran=malloc(sizeof(struct transaction));
				strcpy(tran->date,curr_date);
				curr_time[strlen(curr_time)-1]=curr_time[strlen(curr_time)-1]+'1'-'0';
				strcpy(tran->time,curr_time);
				tran->senderWalletId=malloc((strlen(leksi[1])+1)*sizeof(char));
				tran->receiverWalletId=malloc((strlen(leksi[2])+1)*sizeof(char));
				tran->value=atoi(leksi[3]);
				strcpy(tran->senderWalletId,leksi[1]);
				strcpy(tran->receiverWalletId,leksi[2]);
				tran->transactionId=malloc((strlen(last_id)+1)*sizeof(char));
				strcpy(tran->transactionId,last_id);
				last_id[strlen(last_id)-1]=last_id[strlen(last_id)-1]+'1'-'0';
				Transaction=realloc(Transaction,(number_of_lines_of_transcationsFile+1)*sizeof(struct transaction));
				Transaction[number_of_lines_of_transcationsFile]=*tran;
				number_of_lines_of_transcationsFile++;
				requestTransaction1(leksi[1],leksi[2],wallet,Sender_Table,Receiver_table,number_of_lines_of_bitCoinBalancesFiles,senderHashtable1NumOfEntries,receiverHashtable1NumOfEntries,num_Records,Transaction,number_of_lines_of_transcationsFile,size_of_Bit_Table,Bit_Table);
				free(tran);
			}
			else{
				int enter=0;
				for(int k=6;k<10;k++){
					if(curr_date[k]<leksi[4][k]){
						enter=1;
						break;
					}
					if(curr_date[k]==leksi[4][k])
						continue;
					else {
						enter=-1;
						printf("%s\n","Request rejected");
						break;
					}
				}
				if(enter==-1)
					continue;
				if(enter==0){
					for(int k=3;k<5;k++){
						if(curr_date[k]<leksi[4][k]){
							enter=1;
							break;
						}
						if(curr_date[k]==leksi[4][k])
							continue;
						else {
							enter=-1;
							printf("%s\n","Request rejected");
							break;
						}
					}
				}
				if(enter==-1)
					continue;
				if(enter==0){
					for(int k=0;k<2;k++){
						if(curr_date[k]<leksi[4][k]){
							enter=1;
							break;
						}
						if(curr_date[k]==leksi[4][k])
							continue;
						else {
							enter=-1;
							printf("%s\n","Request rejected");
							break;
						}
					}
				}
				if(enter==-1)
					continue;
				if(enter==0){
					for(int k=0;k<2;k++){
						if(curr_time[k]<leksi[5][k]){
							enter=1;
							break;
						}
						if(curr_time[k]==leksi[5][k])
							continue;
						else {
							enter=-1;
							printf("%s\n","Request rejected");
							break;
						}
					}
				}
				if(enter==-1)
					continue;
				if(enter==0){
					for(int k=3;k<5;k++){
						if(curr_time[k]<leksi[5][k]){
							enter=1;
							break;
						}
						if(curr_time[k]==leksi[5][k])
							continue;
						else {
							enter=-1;
							printf("%s\n","Request rejected");
							break;
						}
					}
				}
				if(enter==-1)
					continue;
				struct transaction* tran=malloc(sizeof(struct transaction));
				strcpy(tran->date,leksi[4]);
				strcpy(tran->time,leksi[5]);
				tran->senderWalletId=malloc((strlen(leksi[1])+1)*sizeof(char));
				tran->receiverWalletId=malloc((strlen(leksi[2])+1)*sizeof(char));
				tran->value=atoi(leksi[3]);
				strcpy(tran->senderWalletId,leksi[1]);
				strcpy(tran->receiverWalletId,leksi[2]);
				tran->transactionId=malloc((strlen(last_id)+1)*sizeof(char));
				strcpy(tran->transactionId,last_id);
				Transaction=realloc(Transaction,(number_of_lines_of_transcationsFile+1)*sizeof(struct transaction));
				Transaction[number_of_lines_of_transcationsFile]=*tran;
				number_of_lines_of_transcationsFile++;
				requestTransaction1(leksi[1],leksi[2],wallet,Sender_Table,Receiver_table,number_of_lines_of_bitCoinBalancesFiles,senderHashtable1NumOfEntries,receiverHashtable1NumOfEntries,num_Records,Transaction,number_of_lines_of_transcationsFile,size_of_Bit_Table,Bit_Table);
				free(tran);
			}
		}
		else if(strcmp("-/requestTransactions",leksi[0])==0){
			if(!strcmp(leksi[1],"")){
				printf("%s\n","Error:No Input");
				return EXIT_FAILURE;
			}
			if(strstr(leksi[1],".txt")!=NULL){
				FILE* file1;
				file1=fopen(leksi[1],"r");
				int line=0;
				while((c=fgetc(file1))!=EOF){
					if(c=='\n')
						line++;
				}
				int start=number_of_lines_of_transcationsFile;
				Transaction=realloc(Transaction,(start+line)*sizeof(struct transaction));
				number_of_lines_of_transcationsFile=start+line;
				rewind(file1);
				for(int i=start;i<start+line;i++){
					counter1=0;
					Transaction[i].transactionId=malloc((counter1+1)*sizeof(char));
					while(1){
						c=fgetc(file1);
						if(c==' '||c=='\t'){
							Transaction[i].transactionId[counter1]='\0';
							break;
						}
						Transaction[i].transactionId[counter1++]=c;
						Transaction[i].transactionId=realloc(Transaction[i].transactionId,(counter1+1)*sizeof(char));
					}	
					counter1=0;
					Transaction[i].senderWalletId=malloc((counter1+1)*sizeof(char));
					while(1){
						c=fgetc(file1);
						if(c==' '||c=='\t'){
							Transaction[i].senderWalletId[counter1]='\0';
							break;
						}
						Transaction[i].senderWalletId[counter1++]=c;
						Transaction[i].senderWalletId=realloc(Transaction[i].senderWalletId,(counter1+1)*sizeof(char));
					}
					counter1=0;
					Transaction[i].receiverWalletId=malloc((counter1+1)*sizeof(char));
					while(1){
						c=fgetc(file1);
						if(c==' '||c=='\t'){
							Transaction[i].receiverWalletId[counter1]='\0';
							break;
						}
						Transaction[i].receiverWalletId[counter1++]=c;
						Transaction[i].receiverWalletId=realloc(Transaction[i].receiverWalletId,(counter1+1)*sizeof(char));
					}
					Transaction[i].value=0;
					while(1){
						c=fgetc(file1);
						if(c==' '||c=='\t')
							break;
						Transaction[i].value=Transaction[i].value*10+c-'0';
					}
					counter1=0;
					while(1){
						c=fgetc(file1);
						if(c==' '||c=='\t'){
							Transaction[i].date[counter1]='\0';
							break;
						}
						Transaction[i].date[counter1++]=c;
					}
					counter1=0;
					while(1){
						c=fgetc(file1);
						if(c=='\n'){
							Transaction[i].time[counter1]='\0';
							break;
						}
						Transaction[i].time[counter1++]=c;
					}
					if(c=='\n')
						continue;
					else{
						printf("%s\n","Error in format of File");
						return EXIT_FAILURE;
					}
				}
				fclose(file1);
				for(int i=start;i<start+line;i++){
						if(strcmp(Transaction[i].senderWalletId,Transaction[i].receiverWalletId)==0){
								printf("Error:Cannot have the same sender and receiver\n");
								return -1;
						}
				}
				for(int i=start;i<number_of_lines_of_transcationsFile;i++){
					char user1[50],user2[50];
					strcpy(user1,Transaction[i].senderWalletId);
					strcpy(user2,Transaction[i].receiverWalletId);
					int pos=-1;
					for(j=0;j<number_of_lines_of_bitCoinBalancesFiles;j++)
						if(strcmp(user2,wallet[j].userID)==0){
							pos=j;
							break;
						}
					if(pos==-1){
						printf("Error:Receiver user does not exist\n");
						return -1;
					}
					pos=-1;
					for(j=0;j<number_of_lines_of_bitCoinBalancesFiles;j++){
						if(strcmp(user1,wallet[j].userID)==0){
							pos=j;
							break;
						}
					}
					if(pos==-1){
						printf("Error:Sender user does not exist\n");
						return -1;
					}
				}
				requestTransaction2(start,number_of_lines_of_transcationsFile,wallet,Sender_Table,Receiver_table,number_of_lines_of_bitCoinBalancesFiles,Transaction,senderHashtable1NumOfEntries,receiverHashtable1NumOfEntries,num_Records,size_of_Bit_Table,Bit_Table);
			}
			else{
				

			}
		}	
		else if(strcmp("-/findEarnings",leksi[0])==0){
			if(strcmp(leksi[1],"")==0){
				printf("%s\n","Error:No Input");
				return EXIT_FAILURE;
			}
			int amount;
			if(strcmp(leksi[2],"")==0)
				amount=findEarnings1(leksi[1],Receiver_table,receiverHashtable1NumOfEntries,num_Records);
			else
				amount=findEarnings2(leksi[1],leksi[2],leksi[3],leksi[4],leksi[5],Receiver_table,receiverHashtable1NumOfEntries,num_Records);
			printf("amount=%d\n",amount);
		}
		else if(strcmp("-/findPayments",leksi[0])==0){
			if(strcmp(leksi[1],"")==0){
				printf("%s\n","Error:No Input");
				return EXIT_FAILURE;
			}
			int amount;
			if(strcmp(leksi[2],"")==0)
				amount=findPayments1(leksi[1],Sender_Table,senderHashtable1NumOfEntries,num_Records);
			else
				amount=findPayments2(leksi[1],leksi[2],leksi[3],leksi[4],leksi[5],Sender_Table,senderHashtable1NumOfEntries,num_Records);
			printf("amount=%d\n",amount);

		}
		else if(strcmp("-/walletStatus",leksi[0])==0){
			if(strcmp(leksi[1],"")==0){
				printf("%s\n","Error:No Input");
				return EXIT_FAILURE;
			}
			printf("%d\n",walletStatus(wallet,number_of_lines_of_bitCoinBalancesFiles,leksi[1]));
		}
		else if(strcmp("-/bitCoinStatus",leksi[0])==0){
			if(strcmp(leksi[1],"")==0){
				printf("%s\n","Error:No Input");
				return EXIT_FAILURE;
			}
			int ID=atoi(leksi[1]);
			bitCoinStatus(ID,Bit_Table,size_of_Bit_Table,bitCoinValue);
		}
		else if(strcmp("-/traceCoin",leksi[0])==0){
			if(strcmp(leksi[1],"")==0){
				printf("%s\n","Error:No Input");
				return EXIT_FAILURE;
			}
			int ID=atoi(leksi[1]);
			traceCoin(ID,Transaction,Sender_Table,senderHashtable1NumOfEntries,num_Records);
		}
		else if(strcmp("-/exit",leksi[0])==0){
			free(last_id);
			exit1(Bit_Table,size_of_Bit_Table);
			exit2(Sender_Table,Receiver_table,num_Records,senderHashtable1NumOfEntries,receiverHashtable1NumOfEntries);
			free(Bit_Table);
			for(i=0;i<6;i++)
				free(leksi[i]);
			for(i=0;i<number_of_lines_of_transcationsFile;i++){
				free(Transaction[i].receiverWalletId);
				free(Transaction[i].senderWalletId);
				free(Transaction[i].transactionId);
			}
			free(Transaction);
			for(i=0;i<number_of_lines_of_bitCoinBalancesFiles;i++)
				free_list(&wallet[i]);
			free(wallet);
			free(all_Bit_Coin);
			free(bitCoinBalancesFile);
			free(transactionsFile);
			fclose(file2);
			fclose(file1);
			return EXIT_SUCCESS;
		}
		else{
			printf("%s\n","Unknown Command");
			return EXIT_FAILURE;
		}
		for(int y=0;y<6;y++)
			strcpy(leksi[y],"");
	}
	////////////////////////////////////////////////////////////////////////////////////////
}