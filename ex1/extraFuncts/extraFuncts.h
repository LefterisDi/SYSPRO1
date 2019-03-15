#ifndef __EXTRA_FUNCTS_H__
#define __EXTRA_FUNCTS_H__

#include "../walletList/walletList.h"
#include "../bitcoinTreeList/bitcoinTreeList.h"


int CheckIfValid(char* , char[8][23]);

int InterFace(char[8][23] , int* , char** , char** , WalletListNode* , Tree_List_Node* , Hashtable* , Hashtable*);

int CompareDates(char* , char*);
int CompareTimes(char* , char*);

int TreeListDelete(Tree_List_Node**);
int WalletListDelete(WalletListNode**); 

int TimeIsValid(char*);
int DateIsValid(char*);

int RequestTransaction(char** , int*, char**, char**,
WalletListNode*, Tree_List_Node* , Hashtable*, Hashtable*);

int RequestTransactionReadFromFile(char** , int*, char** , char** ,
WalletListNode* , Tree_List_Node* , Hashtable* , Hashtable*);

int FindEarnings(char** ,WalletListNode* , Hashtable*);
int FindPayments(char** point ,WalletListNode* , Hashtable*);
int FindEarnings_PaymentsInputIsValid(char* , char* , char* , char*);

int GetWalledIDStatus(char** , WalletListNode*);
int GetBitcoinIDStatus(char** , Tree_List_Node*);

int Tracecoin(char**, Tree_List_Node*);

#endif