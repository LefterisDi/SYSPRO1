#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include "../transactionList/transactionList.h"

typedef struct HtEntry{
    char* ID;
    TransactionListNode* tStatNext;
}HtEntry;

typedef struct Bucket{
    int emptySpaces;
    int maxSpaces;
    HtEntry** htEntries;
    struct Bucket* nextBucket;
}Bucket;

typedef struct Hashtable{
    Bucket** indexTable; 
    int bucketSize;  
    int numOfSlots;
}Hashtable;

Hashtable* CreateHashTable(int , int);
Bucket* HashTableAddBucket(Hashtable* , int);
int HashTableInsertEntry(Hashtable* , char* , TransactionStatus);
HtEntry* HashTableSearchForEntry(Hashtable* , char*);
int HashFunction(char* , int);
int HashTableDelete(Hashtable**);
int HashTableTransactionExists(Hashtable* , int);

#endif