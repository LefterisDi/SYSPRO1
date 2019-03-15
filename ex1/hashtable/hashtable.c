
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "./hashtable.h"
#include "../transactionList/transactionList.h"

Hashtable* CreateHashTable(int numOfSlots , int bucketSize){//creates and initializes the hash table
    
    Hashtable* ht = malloc(sizeof(Hashtable));
    ht->indexTable = malloc(sizeof(Bucket*)*numOfSlots);
    for (int i = 0 ; i < numOfSlots ; i++){
        ht->indexTable[i] = NULL;
    }
    ht->bucketSize = bucketSize;
    ht->numOfSlots = numOfSlots;

    return ht;
    
}

Bucket* HashTableAddBucket(Hashtable* ht , int index){//adds a bucket at the indexTable position indicated by index
    
    Bucket* temp;
    Bucket* last;
    Bucket* newBucket;

    temp = ht->indexTable[index];

    while(temp != NULL){
        last = temp;
        temp = temp->nextBucket;
    }

    newBucket = malloc(sizeof(Bucket));
    newBucket->nextBucket = NULL;
    newBucket->htEntries = malloc(ht->bucketSize * sizeof(HtEntry*));
    for (int i = 0 ; i < ht->bucketSize ; i++){
        newBucket->htEntries[i] = NULL;
    }
    newBucket->emptySpaces = ht->bucketSize;
    newBucket->maxSpaces = ht->bucketSize;

    if (ht->indexTable[index] == NULL){
        ht->indexTable[index] = newBucket;
    }
    else{
        last->nextBucket = newBucket;
    }

    return newBucket;
    
}

int HashTableInsertEntry(Hashtable* ht , char* hashKey , TransactionStatus tStat){

    Bucket* temp;
    Bucket* last;
    HtEntry* tempEntry;
    int bucketEmptySpaceIndex;
    int index;

    index = HashFunction(hashKey , ht->numOfSlots);

    tempEntry = HashTableSearchForEntry(ht , hashKey);

    if (tempEntry == NULL){//checks wether the entry exists or not
    //if it doesnt a new entry is created so that we can store the transaction status
    //if it does then we search for it and we add the transaction status on the list of transaction statuses that
    //every entry has

        temp = ht->indexTable[index];

        if (temp == NULL){//if the indexTable cell is NULL or the existing buckets cant host another entry then a new bucket is created
            HashTableAddBucket(ht , index);
            temp = ht->indexTable[index];
        }

        while(temp != NULL){
            last = temp;
            temp = temp->nextBucket;
        }

        if (last->emptySpaces == 0){
            last = HashTableAddBucket(ht , index);
            bucketEmptySpaceIndex = 0;
        }
        else{
            bucketEmptySpaceIndex = last->maxSpaces - last->emptySpaces;            
        }

        HtEntry* newHtEntry = malloc(sizeof(HtEntry));
        newHtEntry->tStatNext = NULL;

        newHtEntry->ID = malloc(strlen(hashKey)+1);
        strcpy(newHtEntry->ID , hashKey);

        TransactionListPush(&newHtEntry->tStatNext , tStat);//the new transaction is pushed into the transaction list

        last->htEntries[bucketEmptySpaceIndex] = newHtEntry;
        last->emptySpaces--;
    }
    else {
        TransactionListPush(&tempEntry->tStatNext , tStat);
    }

    return 0;

}

HtEntry* HashTableSearchForEntry(Hashtable* ht , char* hashKey){//searches for an entry into the hashtable

    Bucket* temp;
    Bucket* last;
    int index;

    index = HashFunction(hashKey , ht->numOfSlots);

    temp = ht->indexTable[index];

    if (temp == NULL){
        return NULL;
    }    

    while(temp != NULL){

        HtEntry* tempEntry;

        for (int i = 0 ; i < temp->maxSpaces - temp->emptySpaces ; i++){//searches every bucket of a certain index
            
            tempEntry = temp->htEntries[i];
            
            if (strcmp(tempEntry->ID , hashKey) == 0){
                return tempEntry;
            }
        }

        last = temp;
        temp = temp->nextBucket;
    }

    return NULL;
    
}

int HashTableTransactionExists(Hashtable* ht , int transID){//checks if a transaction ID exists in the hashtable
//searches every entry in the hashtable
    
    Bucket* temp;
    Bucket* last;
    int index;

    for (index = 0 ; index < ht->numOfSlots ; index++){
        
        temp = ht->indexTable[index];

        while(temp != NULL){

            HtEntry* tempEntry;

            for (int i = 0 ; i < temp->maxSpaces - temp->emptySpaces ; i++){
                
                tempEntry = temp->htEntries[i];
                
                if (tempEntry != NULL){
                    if (TransactionListFindBasedOnTID(tempEntry->tStatNext , transID) != NULL){
                            return 1;
                    }
                }
            }

            last = temp;
            temp = temp->nextBucket;
        }
    }

    return 0;
}


int HashFunction(char* elemIndex , int numOfSlots){//calculates and produces in index for the reference to the hash table

  int sum = 0;
  for (int i = 0 ; i < 5 ; i++){
    sum += elemIndex[i];
  }

  return sum % numOfSlots;
} 

int HashTableDelete(Hashtable** ht){//deletes the whole hashtable , the buckets , the entries and the transaction lists

    for (int i = 0 ; i < (*ht)->numOfSlots ; i++){
        if ((*ht)->indexTable[i] != NULL){

            for (int k = 0 ; k < (*ht)->indexTable[i]->maxSpaces - (*ht)->indexTable[i]->emptySpaces ; k++){
                while((*ht)->indexTable[i]->htEntries[k]->tStatNext != NULL){
                    TransactionListDeleteFirst(&(*ht)->indexTable[i]->htEntries[k]->tStatNext);
                }
                free((*ht)->indexTable[i]->htEntries[k]->ID);
                free((*ht)->indexTable[i]->htEntries[k]);
            }


            free((*ht)->indexTable[i]->htEntries);
            Bucket* temp = NULL;
            Bucket* last = NULL;
            temp = (*ht)->indexTable[i];
            while (temp != NULL){
                last = temp;
                temp = temp->nextBucket;
               free(last);
            }
        }   
        
    }

    free((*ht)->indexTable);
    free(*ht);
    *ht = NULL;
    return 0;
}