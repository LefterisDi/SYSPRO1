#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../wallet.h"
#include "./transactionList.h"

TransactionListNode* TransactionListCreateStartingNode(TransactionStatus item){//creates the first node of the transaction list and
//initializes it
    
    TransactionListNode** temp;
    TransactionListNode* ret;
    temp = NULL;
    temp = malloc(sizeof(TransactionListNode*));
    *temp = malloc(sizeof(TransactionListNode));
    if (*temp == NULL) {
        return NULL;
    }

    (*temp)->tStat.date = malloc(strlen(item.date)+1);
    strcpy((*temp)->tStat.date , item.date);

    (*temp)->tStat.receiver = malloc(strlen(item.receiver)+1);
    strcpy((*temp)->tStat.receiver , item.receiver);

    (*temp)->tStat.sender = malloc(strlen(item.sender)+1);
    strcpy((*temp)->tStat.sender , item.sender);

    (*temp)->tStat.time = malloc(strlen(item.time)+1);
    strcpy((*temp)->tStat.time , item.time);

    (*temp)->tStat.transactionID = item.transactionID;
    (*temp)->tStat.value = item.value;

    (*temp)->next = NULL;

    ret = *temp;
    free(temp);

    return ret;
}

void TransactionListPush(TransactionListNode** start , TransactionStatus item){
    //pushed a transaction status item into the transaction list

    if (*start == NULL){
        *start = TransactionListCreateStartingNode(item);
        return;
    }
    
    TransactionListNode* newNode = NULL;
    newNode = malloc(sizeof(TransactionListNode));

    newNode->tStat.date = malloc(strlen(item.date)+1);
    strcpy(newNode->tStat.date , item.date);

    newNode->tStat.receiver = malloc(strlen(item.receiver)+1);
    strcpy(newNode->tStat.receiver , item.receiver);

    newNode->tStat.sender = malloc(strlen(item.sender)+1);
    strcpy(newNode->tStat.sender , item.sender);

    newNode->tStat.time = malloc(strlen(item.time)+1);
    strcpy(newNode->tStat.time , item.time);

    newNode->tStat.value = item.value;
    newNode->tStat.transactionID = item.transactionID;

    newNode->next = *start;
    *start = newNode;
}

int TransactionListDeleteFirst(TransactionListNode** start){
//deletes the first node of the list
    TransactionListNode* nextNode = NULL;

    if (*start == NULL) {
        return -1;
    }

    nextNode = (*start)->next;
    free((*start)->tStat.date);
    free((*start)->tStat.receiver);
    free((*start)->tStat.sender);
    free((*start)->tStat.time);
    free(*start);
    *start = nextNode;

    return 0;
}

TransactionListNode* TransactionListFindBasedOnTransaction(TransactionListNode* start , TransactionStatus* tStat) {
//finds a transaction node based on the whole transaction
    TransactionListNode* current = start;

    if (tStat == NULL){
        return NULL;
    }

    while (current != NULL) {
       
        if (tStat->transactionID == current->tStat.transactionID && \
            tStat->value == current->tStat.value && \
            strcmp(tStat->date , current->tStat.date) == 0 && \
            strcmp(tStat->receiver , current->tStat.receiver) == 0 && \
            strcmp(tStat->sender , current->tStat.sender) == 0 && \
            strcmp(tStat->time , current->tStat.time) == 0 ) 
        {
                
            return current;
        }

        current = current->next;
    }

    return NULL;
}

TransactionListNode* TransactionListFindBasedOnTID(TransactionListNode* start , int ID) {
    //finds a node based only on the transaction ID 
    //this is used to check if a transaction ID is unique

    TransactionListNode* current = start;

    while (current != NULL) {
       
        if (ID == current->tStat.transactionID){
            return current;
        }

        current = current->next;
    }

    return NULL;
}