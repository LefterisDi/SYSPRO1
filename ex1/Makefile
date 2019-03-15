
#---------------------------------------------------------------------------------------------------------------------#

PRIM_OBJS = ./bitcoin.o ./walletList/walletList.o ./bitcoinTreeList/bitcoinTreeList.o ./hashtable/hashtable.o
SEC_OBJS = ./bitcoinSimpleList/bitcoinSimpleList.o ./transactionList/transactionList.o ./bitcoinTreeNodeQueue/bitcoinTreeNodeQueue.o ./bitcoinInnerTreeNodeList/bitcoinInnerTreeNodeList.o ./bfs/bfs.o ./extraFuncts/extraFuncts.o

PRIM_SOURCE = ./bitcoin.c /walletList/walletList.c ./bitcoinTreeList/bitcoinTreeList.c ./hashtable/hashtable.c
SEC_SOURCE = ./bitcoinSimpleList/bitcoinSimpleList.c ./transactionList/transactionList.c ./bitcoinTreeNodeQueue/bitcoinTreeNodeQueue.c ./bitcoinInnerTreeNodeList/bitcoinInnerTreeNodeList.c ./bfs/bfs.c ./extraFuncts/extraFuncts.c

OUT = bitcoin

CC = gcc
FLAGS = -c
DEBUG = -g3

#---------------------------------------------------------------------------------------------------------------------#

#Creating Executables

bitcoin : $(PRIM_OBJS) $(SEC_OBJS)
	$(CC) $(DEBUG) -o $(OUT) $(PRIM_OBJS) $(SEC_OBJS)

#Creating Object Files

./bitcoin.o : ./bitcoin.c
	$(CC) $(DEBUG) -o ./bitcoin.o $(FLAGS) ./bitcoin.c
	
./walletList/walletList.o : ./walletList/walletList.c
	$(CC) $(DEBUG) -o ./walletList/walletList.o $(FLAGS) ./walletList/walletList.c
	
./bitcoinTreeList/bitcoinTreeList.o : ./bitcoinTreeList/bitcoinTreeList.c
	$(CC) $(DEBUG) -o ./bitcoinTreeList/bitcoinTreeList.o $(FLAGS) ./bitcoinTreeList/bitcoinTreeList.c

./hashtable/hashtable.o : ./hashtable/hashtable.c
	$(CC) $(DEBUG) -o ./hashtable/hashtable.o $(FLAGS) ./hashtable/hashtable.c

./bitcoinSimpleList/bitcoinSimpleList.o : ./bitcoinSimpleList/bitcoinSimpleList.c
	$(CC) $(DEBUG) -o ./bitcoinSimpleList/bitcoinSimpleList.o $(FLAGS) ./bitcoinSimpleList/bitcoinSimpleList.c
	
./transactionList/transactionList.o : ./transactionList/transactionList.c
	$(CC) $(DEBUG) -o ./transactionList/transactionList.o $(FLAGS) ./transactionList/transactionList.c
	
./bitcoinTreeNodeQueue/bitcoinTreeNodeQueue.o : ./bitcoinTreeNodeQueue/bitcoinTreeNodeQueue.c
	$(CC) $(DEBUG) -o ./bitcoinTreeNodeQueue/bitcoinTreeNodeQueue.o $(FLAGS) ./bitcoinTreeNodeQueue/bitcoinTreeNodeQueue.c
	
./bitcoinInnerTreeNodeList/bitcoinInnerTreeNodeList.o : ./bitcoinInnerTreeNodeList/bitcoinInnerTreeNodeList.c
	$(CC) $(DEBUG) -o ./bitcoinInnerTreeNodeList/bitcoinInnerTreeNodeList.o $(FLAGS) ./bitcoinInnerTreeNodeList/bitcoinInnerTreeNodeList.c
	
./bfs/bfs.o : ./bfs/bfs.c
	$(CC) $(DEBUG) -o ./bfs/bfs.o $(FLAGS) ./bfs/bfs.c
	
./extraFuncts/extraFuncts.o : ./extraFuncts/extraFuncts.c
	$(CC) $(DEBUG) -o ./extraFuncts/extraFuncts.o $(FLAGS) ./extraFuncts/extraFuncts.c


	
	
#Clean

.PHONY: clean	
	
clean:
	rm $(PRIM_OBJS) $(SEC_OBJS)
