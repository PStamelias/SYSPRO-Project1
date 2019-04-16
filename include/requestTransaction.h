#ifndef __structs_h_
#define __structs_h_
#include "struct.h"
#endif
void requestTransaction1(char* sender,char* receiver,struct walletId* wallet,struct sender_bucket* Sender_Table,struct sender_bucket* Receiver_Table,int size_of_wallet,int sender_size,int receiver_size,int num_Records,struct transaction* Transaction,int number_of_lines_of_transcationsFile,int size_of_Bit_Table,struct BitCoinInfo**Bit_Table);
void requestTransaction2(int start,int number_of_lines_of_transcationsFile,struct walletId* wallet,struct sender_bucket* Sender_Table,struct sender_bucket* Receiver_Table,int number_of_lines_of_bitCoinBalancesFiles,struct transaction* Transaction,int sender_size,int receiver_size,int num_Records,int size_of_Bit_Table,struct BitCoinInfo** Bit_Table);