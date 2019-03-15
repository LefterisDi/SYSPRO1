 #ifndef __INNERTREENODELIST_H__
#define __INNERTREENODELIST_H__

#include "../bitcoinTreeList/bitcoinTreeList.h"

typedef struct InnerTreeListNode{
    Tree_Node* node;
    struct InnerTreeListNode* next;
}InnerTreeListNode;

void InnerTreeListPush(InnerTreeListNode** , Tree_Node*);
int InnerTreeListDeleteFirst(InnerTreeListNode** start);

#endif