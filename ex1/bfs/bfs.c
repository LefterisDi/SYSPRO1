#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./bfs.h"
#include "../bitcoinTreeNodeQueue/bitcoinTreeNodeQueue.h"
#include "../bitcoinTreeList/bitcoinTreeList.h"
#include "../bitcoinInnerTreeNodeList/bitcoinInnerTreeNodeList.h"


int Bfs(Tree_Node* root , char* ID , InnerTreeListNode** listStart){//this version of BFS searches for the leaf nodes and checks if the ID given
//is the same as the leaf node's ID . If so the leaf node is stored in a list

    Queue* q = NULL;
    QueueNode* temp = NULL;

    if (root == NULL){
        return 1;
    }

    q = BitcoinTreeQueueCreate();//BFS uses a queue

    if (BitcoinTreeQueueInsert(q , root) == 1){
        return 1;
    }

    while(BitcoinTreeQueueEmpty(q) == 0){

        temp = BitcoinTreeQueuePop(q);

        if (temp->key->leftChild == NULL && temp->key->rightChild == NULL){//checks if the leaf node's ID is the same as the ID given
            if (strcmp(temp->key->ownerID , ID) == 0){
                InnerTreeListPush(listStart , temp->key);
            }
        }

        if (temp->key->leftChild != NULL){
            if (BitcoinTreeQueueInsert(q , temp->key->leftChild) == 1){
                return 1;
            }
        }

        if (temp->key->rightChild != NULL){
            if (BitcoinTreeQueueInsert(q , temp->key->rightChild) == 1){
                return 1;
            }
        }

        BitcoinTreeQueueDestroyNode(temp);
    }

    free(q);

    return 0;
}
