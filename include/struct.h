struct walletId{
	char userID[50];
	int balance;
	int size_of_bitCoin;
	struct bitCoinId* bit_list;
};
struct bitCoinId{
	int bitCoinId_num;
	int BitCoin_Val;
	struct node* node_ptr;
	struct bitCoinId* next;
};
struct transaction{
	char date[10];
	char* transactionId;
	char* senderWalletId;
	char* receiverWalletId;
	int value;
	char time[5];
};
struct node{
	char name[50];
	int value;
	struct node* left;
	struct node* right;
};
struct transaction_node{
	struct transaction_node* next;
	struct transaction* transaction_ptr;
	int Bit_Coin_Id;
	struct node* node_ptr;
};
struct BitCoinInfo{
	int bitCoinVal;
	int number_of_transaction;
	struct BitCoinInfo* next;
	struct node* riza;
};
struct sender{
	struct transaction_node* transaction_nodeptr;
	struct walletId* walletIdptr;
};
struct sender_bucket{
	struct sender_bucket* next;
	struct sender* sender_bucketptr;
};
