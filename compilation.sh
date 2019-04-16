run: main.c struct.h fun.c walletStatus.c bitCoinStatus.c traceCoin.c exit.c findPayments.c findEarnings.c requestTransaction.c
	gcc -o run main.c struct.h fun.c walletStatus.c bitCoinStatus.c traceCoin.c exit.c findPayments.c findEarnings.c requestTransaction.c
clean:
	rm  main