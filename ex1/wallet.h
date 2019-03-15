#ifndef __WALLET_H__
#define __WALLET_H__

#include "./bitcoinSimpleList/bitcoinSimpleList.h"

typedef struct Wallet{
    char* walletOwnerID;
    int totalBalance;
    int numberOfBitcoins;
    BitcoinSimpleListNode* bitcoinIDsAndBlcs;
}Wallet;

typedef struct TransactionStatus{
    int transactionID;
    char* sender;
    char* receiver;
    int value;
    char* date;
    char* time;
}TransactionStatus;

#endif
