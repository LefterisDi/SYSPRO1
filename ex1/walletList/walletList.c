#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./walletList.h" 
#include "../bitcoinSimpleList/bitcoinSimpleList.h"


WalletListNode* WalletListCreateStartingNode(Wallet item){//creates the first node of the wallet list and initializes it
    
    WalletListNode** temp;
    WalletListNode* ret;
    temp = NULL;
    temp = malloc(sizeof(WalletListNode*));
    *temp = malloc(sizeof(WalletListNode));
    if (*temp == NULL) {
        return NULL;
    }

    (*temp)->wal = item;
   
    (*temp)->next = NULL;

    ret = *temp;
    free(temp);

    return ret;
}

void WalletListPush(WalletListNode** start , Wallet item){//pushes a wallet into the list

    if (*start == NULL){
        *start = WalletListCreateStartingNode(item);
        return;
    }
    
    WalletListNode* newNode;
    newNode = malloc(sizeof(WalletListNode));

    newNode->wal = item;

    newNode->next = *start;
    *start = newNode;
}

int WalletListDeleteFirst(WalletListNode** start){//deletes the first item of the list

    WalletListNode* nextNode = NULL;

    if (*start == NULL) {
        return -1;
    }

    nextNode = (*start)->next;
    while((*start)->wal.bitcoinIDsAndBlcs != NULL){//it also frees the bitcoins the have been stored into this wallet
        BitcoinSimpleListDeleteFirst(&(*start)->wal.bitcoinIDsAndBlcs);
    }
    free((*start)->wal.walletOwnerID);
    free(*start);
    *start = nextNode;

    return 0;
}

WalletListNode* WalletListFindBasedOnID(WalletListNode* start , char* ID) {//finds a wallet based on the walletID

    if (ID == NULL){
        return NULL;
    }

    WalletListNode* current = start;

    while (current != NULL) {
        if (strcmp(ID,current->wal.walletOwnerID) == 0){
            return current;
        }
        current = current->next;
    }

    return NULL;
}


int WalletListFindBasedOnBitcoinID(WalletListNode* start , int bitcoinID) {//Finds if a bitcoin exists in a wallet

    WalletListNode* current = start;

    while (current != NULL) {
        for (int i = 0 ; i < current->wal.numberOfBitcoins ; i++){
            if (BitcoinSimpleListFindBasedOnID(current->wal.bitcoinIDsAndBlcs, bitcoinID) != NULL){
                return 0;
            }
        }
        current = current->next;
    }

    return 1;
}