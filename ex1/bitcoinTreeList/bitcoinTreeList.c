#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./bitcoinTreeList.h" 
#include "../wallet.h"
#include "../walletList/walletList.h"
#include "../bitcoinSimpleList/bitcoinSimpleList.h"
#include "../bitcoinInnerTreeNodeList/bitcoinInnerTreeNodeList.h"
#include "../bfs/bfs.h"
#include "../hashtable/hashtable.h"
#include "../transactionList/transactionList.h"


Tree_List_Node* TreeListCreateStartingNode(int item , char* owner , int balance){//creates and initializes a starting node
    
    Tree_List_Node** temp;
    Tree_List_Node* ret;
    temp = NULL;
    temp = malloc(sizeof(Tree_List_Node*));
    *temp = malloc(sizeof(Tree_List_Node));
    if (*temp == NULL) {
        return NULL;
    }

    (*temp)->rootNode.bitcoinID = item;
    (*temp)->rootNode.ownerID = malloc(strlen(owner) + 1);
    strcpy((*temp)->rootNode.ownerID , owner);
    (*temp)->rootNode.balance = balance;
   
    (*temp)->next = NULL;
    (*temp)->rootNode.leftChild = NULL;
    (*temp)->rootNode.rightChild = NULL;
    (*temp)->rootNode.transSender = NULL;
    (*temp)->rootNode.transReceiver = NULL;

    ret = *temp;
    free(temp);

    return ret;
}

void TreeListPush(Tree_List_Node** start , int bitcID , char* owner , int balance){//creates and pushes a node with the given characteristics

    if (*start == NULL){
        *start = TreeListCreateStartingNode(bitcID , owner , balance);
        return;
    }
    
    Tree_List_Node* newNode;
    newNode = malloc(sizeof(Tree_List_Node));

    newNode->rootNode.bitcoinID = bitcID;
    newNode->rootNode.ownerID = malloc(strlen(owner) + 1);
    strcpy(newNode->rootNode.ownerID , owner);
    newNode->rootNode.balance = balance;

    newNode->next = *start;
    newNode->rootNode.leftChild = NULL;
    newNode->rootNode.rightChild = NULL;
    newNode->rootNode.transReceiver = NULL;
    newNode->rootNode.transSender = NULL;
    *start = newNode;
}

Tree_Node* TreeDelete(Tree_Node* root){//deletes the whole tree created under a tree node
   
    if (root == NULL){
        return root;
    }

    if (root->leftChild != NULL){
        TreeDelete(root->leftChild);
    }
  
    if (root->rightChild != NULL){
        TreeDelete(root->rightChild);
    }

    free(root->ownerID);
    free(root);
    root = NULL;

    return root;
}

int TreeListDeleteFirst(Tree_List_Node** start){//this function deletes the whole list and every tree under every node of the list

    Tree_List_Node* nextNode = NULL;

    if (*start == NULL) {
        return -1;
    }

    nextNode = (*start)->next;
    free((*start)->rootNode.ownerID);
    TreeDelete((*start)->rootNode.leftChild);//since the root is a static structure , it cannot be freed so we need to call tree delete function for
    //both the left and right child
    TreeDelete((*start)->rootNode.rightChild);
    free(*start);
    *start = nextNode;

    return 0;
}

Tree_List_Node* TreeListFindBasedOnID(Tree_List_Node* start , int ID) {//finds a tree list bitcoin node with the corresponding ID

    Tree_List_Node* current = start;

    while (current != NULL) {
        if (ID == current->rootNode.bitcoinID){
            return current;
        }
        current = current->next;
    }

    return NULL;
}



int TreeBalanceTransfer(TransactionStatus* tStat , WalletListNode* walletList , BitcoinSimpleListNode** bitcoinsTransferred){
    
    //this function basically takes every bitcoin needed for a transfer from the sender's wallet and transfers it to the receiver.

    if (tStat == NULL){
        return 1;
    }

    WalletListNode* senderWalletNode = NULL;
    WalletListNode* receiverWalletNode = NULL;

    senderWalletNode = WalletListFindBasedOnID( walletList , tStat->sender);

    if (senderWalletNode == NULL){//checks if the sender exists
        return 1;
    }

    receiverWalletNode = WalletListFindBasedOnID( walletList , tStat->receiver);

    if (receiverWalletNode == NULL){//checks if the receiver exists
        return 1;
    }

    if (senderWalletNode->wal.totalBalance < tStat->value){//checks if there is enough balance
        return 1;
    }
    else if (senderWalletNode->wal.totalBalance - tStat->value == 0){//if the sender's bitcoins are exactly enough for the transaction , then all of them 
    //are transferred to the receiver

        while(senderWalletNode->wal.bitcoinIDsAndBlcs != NULL){
            BitcoinSimpleListPush(bitcoinsTransferred , senderWalletNode->wal.bitcoinIDsAndBlcs->bitcoinID , senderWalletNode->wal.bitcoinIDsAndBlcs->balance);            
            BitcoinSimpleListPush(&receiverWalletNode->wal.bitcoinIDsAndBlcs , senderWalletNode->wal.bitcoinIDsAndBlcs->bitcoinID , senderWalletNode->wal.bitcoinIDsAndBlcs->balance);
            receiverWalletNode->wal.totalBalance += senderWalletNode->wal.bitcoinIDsAndBlcs->balance;
            senderWalletNode->wal.totalBalance -= senderWalletNode->wal.bitcoinIDsAndBlcs->balance;
            BitcoinSimpleListDeleteFirst(&senderWalletNode->wal.bitcoinIDsAndBlcs);
        }

    }
    else {//else not all bitcoins are taken from the sender , but some of them are chosen to be transferred to the receiver's wallet 
        int maxValue = tStat->value;

        while (maxValue > 0){
            int tempValue;
            int prevMaxVal = maxValue;
            tempValue = senderWalletNode->wal.bitcoinIDsAndBlcs->balance;
            maxValue = maxValue - tempValue;

            if (maxValue >= 0){//checks if the bitcoin has enough balance to complete the transaction
                BitcoinSimpleListPush(bitcoinsTransferred , senderWalletNode->wal.bitcoinIDsAndBlcs->bitcoinID , senderWalletNode->wal.bitcoinIDsAndBlcs->balance);
                BitcoinSimpleListPush(&receiverWalletNode->wal.bitcoinIDsAndBlcs , senderWalletNode->wal.bitcoinIDsAndBlcs->bitcoinID , senderWalletNode->wal.bitcoinIDsAndBlcs->balance);
                receiverWalletNode->wal.totalBalance += senderWalletNode->wal.bitcoinIDsAndBlcs->balance;
                senderWalletNode->wal.totalBalance -= senderWalletNode->wal.bitcoinIDsAndBlcs->balance;
                BitcoinSimpleListDeleteFirst(&senderWalletNode->wal.bitcoinIDsAndBlcs);
            }
            else {//if it doesnt then its whole balance is transferred to the receiver and the next bitcoin is chosen to be tested for "splitting".
            //ex if the bitcoin has 100 $ value and 30 $ value is requested then this bitcoin is split in 70 $ for the sender and 30 $ for the receiver
                BitcoinSimpleListPush(bitcoinsTransferred , senderWalletNode->wal.bitcoinIDsAndBlcs->bitcoinID , senderWalletNode->wal.bitcoinIDsAndBlcs->balance - prevMaxVal);
                BitcoinSimpleListPush(&receiverWalletNode->wal.bitcoinIDsAndBlcs , senderWalletNode->wal.bitcoinIDsAndBlcs->bitcoinID , prevMaxVal);
                BitcoinSimpleListPush(&senderWalletNode->wal.bitcoinIDsAndBlcs , senderWalletNode->wal.bitcoinIDsAndBlcs->bitcoinID , -1*prevMaxVal);
                receiverWalletNode->wal.totalBalance += prevMaxVal;
                senderWalletNode->wal.totalBalance -= prevMaxVal;
            }
        }
    }


    return 0;
}

Tree_Node* TreeAddNode(int bitcoinID , char* ownerID , int balance){//creates and adds a tree node to the tree
    
   Tree_Node* temp = NULL;

   temp = malloc(sizeof(Tree_Node));

   temp->bitcoinID = bitcoinID;
   temp->balance = balance;
   temp->ownerID = malloc(strlen(ownerID) + 1);
   strcpy(temp->ownerID , ownerID);

   temp->leftChild = NULL;
   temp->rightChild = NULL;
   temp->transReceiver = NULL;
   temp->transSender = NULL;

   return temp;
}

int TreeTransaction(TransactionStatus* tStat , WalletListNode* walletList , \
 Tree_List_Node* treeList , Hashtable* senderHt , Hashtable* receiverHt){//this function is the "soul" of this whole structure as it completes
 //the transaction. It creates the tree nodes needed and it pushes the transactions to the hashtables .
    
     BitcoinSimpleListNode* bitcoinsTransferred = NULL;

    if (TreeBalanceTransfer(tStat , walletList , &bitcoinsTransferred) == 1){
        return 1;
    }

    Tree_List_Node* bitcoinTreeListNode = NULL;
    Tree_Node* bitcoinRootNode = NULL;
    InnerTreeListNode* allInnerTreeList = NULL;
    InnerTreeListNode* temp = NULL;
    TransactionListNode* tempTransactionNode = NULL;
    HtEntry* tempEntry = NULL;

    BitcoinSimpleListNode* lastNode = NULL;

    int maxValue = tStat->value;
    int senderKeeps;
    int receiverKeeps;

    if (bitcoinsTransferred != NULL && bitcoinsTransferred->next != NULL){//removes the first node from the bitcoin transferred list because 
    //this bitcoin might have been split and since the list pushes every element at the beginning to save time , the "splitted" coin would give
    //its whole value to the receiver rather than the actual value it is supposed to give.
        BitcoinSimpleListPush(&lastNode , bitcoinsTransferred->bitcoinID , bitcoinsTransferred->balance);
        BitcoinSimpleListDeleteFirst(&bitcoinsTransferred);
    }

    while (bitcoinsTransferred != NULL){

        bitcoinTreeListNode = TreeListFindBasedOnID(treeList , bitcoinsTransferred->bitcoinID);

        if (bitcoinTreeListNode == NULL){//searches for the bitcoin node in the tree list
            return 1;
        }

        bitcoinRootNode = &bitcoinTreeListNode->rootNode;

        if(Bfs(bitcoinRootNode , tStat->sender , &allInnerTreeList) == 1){
            return 1;
        }//calls BFS in order to find every leaf node that is used to compute the bitcoin's value

        temp = allInnerTreeList;

        while (temp != NULL){

            if (maxValue - temp->node->balance > 0){//if the bitcoin's balance is less than the requested value then the whole bitcoin is given to the 
            //receiver
                senderKeeps = 0;
                receiverKeeps = temp->node->balance;
                maxValue = maxValue - temp->node->balance;


                temp->node->rightChild = TreeAddNode(bitcoinsTransferred->bitcoinID , tStat->sender , senderKeeps);
                temp->node->leftChild = TreeAddNode(bitcoinsTransferred->bitcoinID , tStat->receiver , receiverKeeps);
                
                if (HashTableInsertEntry(senderHt , tStat->sender , *tStat) != 0){//pushes the transaction into the senders' hash table
                    return 1;
                }

                tempEntry = HashTableSearchForEntry(senderHt , tStat->sender);
                tempTransactionNode = TransactionListFindBasedOnTransaction(tempEntry->tStatNext , tStat);

                temp->node->leftChild->transSender = tempTransactionNode;//every tree node has got a pointer that points to the transaction it participated in
                //this happens in both the senders' and receivers' hash table

                if (HashTableInsertEntry(receiverHt , tStat->receiver , *tStat) != 0){//pushes the transaction into the receiver' hash table
                    return 1;
                }
                tempEntry = HashTableSearchForEntry(receiverHt , tStat->receiver);
                tempTransactionNode = TransactionListFindBasedOnTransaction(tempEntry->tStatNext , tStat);

                temp->node->leftChild->transReceiver = tempTransactionNode;

                temp = temp->next;

            }
            else {

                senderKeeps = temp->node->balance - maxValue;
                receiverKeeps = maxValue;            
                maxValue = 0;

                temp->node->rightChild = TreeAddNode(bitcoinsTransferred->bitcoinID , tStat->sender , senderKeeps);
                temp->node->leftChild = TreeAddNode(bitcoinsTransferred->bitcoinID , tStat->receiver , receiverKeeps);
                
                if (HashTableInsertEntry(senderHt , tStat->sender , *tStat) != 0){//pushes the transaction into the senders' hash table
                    return 1;
                }
                tempEntry = HashTableSearchForEntry(senderHt , tStat->sender);
                tempTransactionNode = TransactionListFindBasedOnTransaction(tempEntry->tStatNext , tStat);

                temp->node->leftChild->transSender = tempTransactionNode;//every tree node has got a pointer that points to the transaction it participated in
                //this happens in both the senders' and receivers' hash table

                if (HashTableInsertEntry(receiverHt , tStat->receiver , *tStat) != 0){//pushes the transaction into the receiver' hash table
                    return 1;
                }
                tempEntry = HashTableSearchForEntry(receiverHt , tStat->receiver);
                tempTransactionNode = TransactionListFindBasedOnTransaction(tempEntry->tStatNext , tStat);

                temp->node->leftChild->transReceiver = tempTransactionNode;

                break;

            }
            
        }

        BitcoinSimpleListDeleteFirst(&bitcoinsTransferred);

        if (bitcoinsTransferred == NULL && lastNode != NULL){//finally the removed from the list bitcoin is given to the list when its real turn has come
            BitcoinSimpleListPush(&bitcoinsTransferred , lastNode->bitcoinID , lastNode->balance);
            BitcoinSimpleListDeleteFirst(&lastNode);
        }

        InnerTreeListDeleteFirst(&allInnerTreeList);

    }

    
    return 0;
}


int TreeTraverse(Tree_Node* root , int* counter){//traverses the tree and increases the counter by 1 if there is a left child
//this shows in how many transactions the bitcoin has participated in

    if (root == NULL){
        return 0;
    }

    *counter += TreeTraverse(root->leftChild , counter);

    TreeTraverse(root->rightChild , counter);

    return 1;
}  

int TreeTraversePrint(Tree_Node* root){//finds every transaction that the bitcoin has participated into and prints the information about it

    if (root == NULL){
        return 0;
    }

    TreeTraversePrint(root->leftChild);

    TreeTraversePrint(root->rightChild);

    if (root->transReceiver != NULL && root->transSender != NULL){
        printf("%d %s %s %d %s %s\n" , root->transReceiver->tStat.transactionID , root->transReceiver->tStat.sender , \
        root->transReceiver->tStat.receiver , root->transReceiver->tStat.value , root->transReceiver->tStat.date , root->transReceiver->tStat.time);
    }

    return 1;
}