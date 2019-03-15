#ifndef __BITCOIN_LIST_H__
#define __BITCOIN_LIST_H__

typedef struct BitcoinListNode {
    int bitcoinID;
    int balance;
    struct BitcoinListNode* next;
} BitcoinSimpleListNode;

BitcoinSimpleListNode* BitcoinSimpleListCreateStartingNode(int , int);
BitcoinSimpleListNode* BitcoinSimpleListPush(BitcoinSimpleListNode** , int , int);
int BitcoinSimpleListDeleteFirst(BitcoinSimpleListNode**);
BitcoinSimpleListNode* BitcoinSimpleListFindBasedOnID(BitcoinSimpleListNode* , int);
int BitcoinSimpleListRemoveByIndex(BitcoinSimpleListNode **, int);

#endif