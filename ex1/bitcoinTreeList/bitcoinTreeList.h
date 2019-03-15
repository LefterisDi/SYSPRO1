#ifndef __TREE_LIST_H__
#define __TREE_LIST_H__

#include "../hashtable/hashtable.h"
#include "../walletList/walletList.h"
#include "../transactionList/transactionList.h"


typedef struct TreeNode {
    int bitcoinID;
    char* ownerID;
    int balance;
    TransactionListNode* transReceiver;
    TransactionListNode* transSender;
    struct TreeNode* leftChild;
    struct TreeNode* rightChild; 
}Tree_Node;

typedef struct TreeListNode {
    struct TreeNode rootNode; 
    struct TreeListNode* next;
} Tree_List_Node;

void TreeListPush(Tree_List_Node** , int , char*, int);
int TreeListDeleteFirst(Tree_List_Node**);
Tree_List_Node* TreeListFindBasedOnID(Tree_List_Node* , int);
int TreeTransaction(TransactionStatus* , WalletListNode* , Tree_List_Node* , Hashtable* , Hashtable*);

int TreeTraverse(Tree_Node* , int*);
int TreeTraversePrint(Tree_Node*);
#endif