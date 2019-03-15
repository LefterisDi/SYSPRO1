#ifndef __TRANSACTIONLIST_H__
#define __TRANSACTIONLIST_H__


#include "../wallet.h"

typedef struct TransactionListNode{
    TransactionStatus tStat;
    struct TransactionListNode* next;
}TransactionListNode;

TransactionListNode* TransactionListCreateStartingNode(TransactionStatus);
void TransactionListPush(TransactionListNode** , TransactionStatus);
int TransactionListDeleteFirst(TransactionListNode**);
TransactionListNode* TransactionListFindBasedOnTransaction(TransactionListNode*, TransactionStatus*);
TransactionListNode* TransactionListFindBasedOnTID(TransactionListNode* , int);

#endif