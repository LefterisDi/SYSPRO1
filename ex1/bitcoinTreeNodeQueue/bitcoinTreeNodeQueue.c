#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./bitcoinTreeNodeQueue.h"
#include "../bitcoinTreeList/bitcoinTreeList.h"

 
QueueNode* BitcoinTreeQueueNewNode(Tree_Node* key){// a simple bitcoin queue

    QueueNode *temp = (QueueNode*)malloc(sizeof(QueueNode)); 

    temp->key = key; 
    temp->next = NULL; 
    
    return temp;  
} 

Queue* BitcoinTreeQueueCreate(){//creates the queue

    Queue* q = (struct Queue*)malloc(sizeof(struct Queue)); 
    q->front = NULL;
    q->back = NULL; 
    return q; 
} 

int BitcoinTreeQueueInsert(Queue* q, Tree_Node* key){//inserts a bitcoin into the queue

    if (key == NULL){
        return 1;
    }

    QueueNode* temp = BitcoinTreeQueueNewNode(key); 
  
    if (q->back == NULL) 
    { 
       q->front = temp; 
       q->back = temp;

       return 0; 
    } 
  
    q->back->next = temp; 
    q->back = temp; 
    return 0;
} 

QueueNode* BitcoinTreeQueuePop(Queue* q){ 

    if (q->front == NULL){
       return NULL; 
    }
  
    // Store previous front and move front one node ahead 
    QueueNode* temp = q->front; 
    q->front = q->front->next; 
  
    // If front becomes NULL, then changes the back also as NULL 
    if (q->front == NULL) 
       q->back = NULL; 

    return temp; 
} 

void BitcoinTreeQueueDestroyNode(QueueNode* qNode){
    free(qNode);
    qNode = NULL;
}

int BitcoinTreeQueueEmpty(Queue* q){

    if (q->back == NULL){
        return 1;
    }

    return 0;
}