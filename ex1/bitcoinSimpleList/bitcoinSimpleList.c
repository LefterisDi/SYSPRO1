#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./bitcoinSimpleList.h" 

//simple bitcoin list 

BitcoinSimpleListNode* BitcoinSimpleListCreateStartingNode(int ID , int balance){// creates and initializes starting node of the list
    
    BitcoinSimpleListNode** temp;
    BitcoinSimpleListNode* ret;
    temp = NULL;
    temp = malloc(sizeof(BitcoinSimpleListNode*));
    *temp = malloc(sizeof(BitcoinSimpleListNode));
    if (*temp == NULL) {
        return NULL;
    }

    (*temp)->bitcoinID = ID;
    (*temp)->balance = balance;
   
    (*temp)->next = NULL;

    ret = *temp;
    free(temp);

    return ret;
}

BitcoinSimpleListNode* BitcoinSimpleListPush(BitcoinSimpleListNode** start , int bitcID  , int balance){

    if (*start == NULL){
        *start = BitcoinSimpleListCreateStartingNode(bitcID , balance);
        return *start;
    }

    BitcoinSimpleListNode* bitcoinNode = NULL;

    bitcoinNode = BitcoinSimpleListFindBasedOnID(*start , bitcID);//checks if the node already exists . If it does then the function updates its value
    //If the bitcoin doesn't exist then a new node is created and is initialized with the value given

    if (bitcoinNode == NULL){
        BitcoinSimpleListNode* newNode;
        newNode = malloc(sizeof(BitcoinSimpleListNode));

        newNode->bitcoinID = bitcID;
        newNode->balance = balance;

        newNode->next = *start;
        *start = newNode;

        return newNode;
    }
    else {
        bitcoinNode->balance += balance;

        return bitcoinNode;
    }

}

int BitcoinSimpleListDeleteFirst(BitcoinSimpleListNode** start){//deletes the first element of the list

    BitcoinSimpleListNode* nextNode = NULL;

    if (*start == NULL) {
        return -1;
    }

    nextNode = (*start)->next;
    free(*start);
    *start = nextNode;

    return 0;
}

BitcoinSimpleListNode* BitcoinSimpleListFindBasedOnID(BitcoinSimpleListNode* start , int ID) {//finds the bitcoin that has the given ID

    BitcoinSimpleListNode* current;

    current = start;

    while (current != NULL) {
        if (ID == current->bitcoinID){
            return current;
        }
        current = current->next;
    }

    return NULL;
} 


int BitcoinSimpleListRemoveByIndex(BitcoinSimpleListNode **start, int ID) {//removes the bitcoin that has the given ID
//if it exists

    BitcoinSimpleListNode* current;
    BitcoinSimpleListNode* temp = NULL;

    if (start == NULL) {
        return 1;
    }

    if(*start == NULL){
        return 1;
    }

    if ((*start)->next == NULL){
        free(*start);
        *start = NULL;
        return 0;
    }

    if ((*start)->next != NULL && ID == (*start)->bitcoinID){//if the bitcoin chosen is the very first node
        BitcoinSimpleListDeleteFirst(start);
        return 0;
    }

    current = *start;

    while (current != NULL) {
        if (ID == current->next->bitcoinID){
            temp = current->next;
            current->next = temp->next;
            free(temp);
            return 0;
        }
        current = current->next;
    }

   return 1;
}