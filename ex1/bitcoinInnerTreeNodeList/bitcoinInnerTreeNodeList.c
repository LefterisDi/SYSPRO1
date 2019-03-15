#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./bitcoinInnerTreeNodeList.h"
#include "../bitcoinTreeList/bitcoinTreeList.h"

InnerTreeListNode* InnerTreeListCreateStartingNode(Tree_Node* item){//creates the starting node of the list and initializes it
    
    InnerTreeListNode** temp;
    InnerTreeListNode* ret;
    temp = NULL;
    temp = malloc(sizeof(InnerTreeListNode*));
    *temp = malloc(sizeof(InnerTreeListNode));
    if (*temp == NULL) {
        return NULL;
    }

    (*temp)->node = item;
   
    (*temp)->next = NULL;

    ret = *temp;
    free(temp);

    return ret;
}

void InnerTreeListPush(InnerTreeListNode** start , Tree_Node* item){//pushes the item into the list

    if (*start == NULL){
        *start = InnerTreeListCreateStartingNode(item);
        return;
    }
    
    InnerTreeListNode* newNode;
    newNode = malloc(sizeof(InnerTreeListNode));

    newNode->node = item;

    newNode->next = *start;
    *start = newNode;
}

int InnerTreeListDeleteFirst(InnerTreeListNode** start){//deletes the first item of the list

    InnerTreeListNode* nextNode = NULL;

    if (*start == NULL) {
        return -1;
    }

    nextNode = (*start)->next;
    free(*start);
    *start = nextNode;

    return 0;
}