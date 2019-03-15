#ifndef __LIST_H__
#define __LIST_H__

#include "../wallet.h"

typedef struct node {
    Wallet wal;
    struct node* next;
} WalletListNode; 

void WalletListPush(WalletListNode** , Wallet);
int WalletListDeleteFirst(WalletListNode**);
WalletListNode* WalletListFindBasedOnID(WalletListNode* , char*);
int WalletListFindBasedOnBitcoinID(WalletListNode* , int);
#endif