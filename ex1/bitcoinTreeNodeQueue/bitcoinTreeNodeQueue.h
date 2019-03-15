#ifndef __TREE_LIST_QUEUE_H__
#define __TREE_LIST_QUEUE_H__

#include "../bitcoinTreeList/bitcoinTreeList.h"

typedef struct QueueNode { 
    Tree_Node* key; 
    struct QueueNode *next; 
}QueueNode; 

typedef struct Queue { 
    QueueNode* front; 
    QueueNode* back;
}Queue; 


QueueNode* BitcoinTreeQueueNewNode(Tree_Node*);
Queue* BitcoinTreeQueueCreate();
int BitcoinTreeQueueInsert(Queue*, Tree_Node*);
QueueNode* BitcoinTreeQueuePop(Queue*);
void BitcoinTreeQueueDestroyNode(QueueNode*);
int BitcoinTreeQueueEmpty(Queue*);

#endif