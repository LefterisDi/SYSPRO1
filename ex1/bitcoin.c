#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./wallet.h"
#include "./walletList/walletList.h"
#include "./bitcoinTreeList/bitcoinTreeList.h"
#include "./bitcoinSimpleList/bitcoinSimpleList.h"
#include "./hashtable/hashtable.h"
#include "./extraFuncts/extraFuncts.h"

int FindinArray(int** , int, int);


int main(int argc , char* argv[]){
   
    int bitcoinValue = -1;
    int bucketSize = -1;
    int senderHtNumOfEntries = -1;
    int receiverHtNumOfEntries = -1;
    
    char* latestDate = malloc(11*sizeof(char));
    strcpy(latestDate , "01-01-1970");
    latestDate[10] = '\0';
    char* latestTime = malloc(6*sizeof(char));
    strcpy(latestTime , "00:00");
    latestTime[5] = '\0';
    int greatestTransID = 0;

    int programExit = 0;
    char possibleCommands[8][23] = {"/requestTransaction" , "/requestTransactions" , "/findEarnings" , "/findPayments" , "/walletStatus" , \
    "/bitcoinStatus" , "/tracecoin" , "/exit"};//every possible command the user can give
    
    FILE* bitcoinBalancesFile = NULL;
    char* bitcoinBFName = NULL;

    FILE* transactionFile = NULL;
    char* tFName = NULL;

    char buf[1024];//buffer for reading from file

    WalletListNode* walletList = NULL;
    Tree_List_Node* bitcoinTreeList = NULL;
    Hashtable* senderHT = NULL;
    Hashtable* receiverHT = NULL;

    while(--argc > 0){
        if (strstr(*argv , "-v") != NULL){
            argv++;
            argc--;
            bitcoinValue = atoi(*argv);
        }

        if (strstr(*argv , "-h1") != NULL){
            argv++;
            argc--;
            senderHtNumOfEntries = atoi(*argv);
        }

        if (strstr(*argv , "-h2") != NULL){
            argv++;
            argc--;
            receiverHtNumOfEntries = atoi(*argv);
        }

        if (strstr(*argv , "-b") != NULL){
            argv++;
            argc--;
            bucketSize = atoi(*argv);
        }

        if (strstr(*argv , "-a") != NULL){
            argv++;
            argc--;
            bitcoinBFName = *argv;
        }

        if (strstr(*argv , "-t") != NULL){
            argv++;
            argc--;
            tFName = *argv;
        }

        if (argc > 0){
            argv++;
        }
    }

    bitcoinBalancesFile = fopen(bitcoinBFName ,"rb");
    if (bitcoinBalancesFile == NULL) {
        perror("Cannot open file");
        return -1;
    }

    transactionFile = fopen(tFName ,"rb");
    if (transactionFile == NULL) {
        perror("Cannot open file");
        return -1;
    }

    senderHT = CreateHashTable(senderHtNumOfEntries , bucketSize);
    receiverHT = CreateHashTable(receiverHtNumOfEntries , bucketSize);

    while (fgets(buf, sizeof(buf), bitcoinBalancesFile) != NULL){

        int i = -1;
        Wallet* wallet;
        char* point;
        char countBuf[1024];

        buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores

        strcpy(countBuf , buf);

        point = strtok(countBuf , " ");
        while(point != NULL){
            i++;
            point = strtok (NULL, " ");
        }

        wallet = malloc(sizeof(Wallet)); 
        wallet->bitcoinIDsAndBlcs = NULL;
        wallet->walletOwnerID = NULL;

        point = strtok(buf , " ");
        bool name = true;
        int j = 0;
        while(point != NULL){//while the buffer has still data
            if (name == true){//first copies the wallet ID
                wallet->walletOwnerID = malloc(sizeof(char)*(strlen(point)+1));
                strcpy(wallet->walletOwnerID , point);
                name = false;
            }
            else{//checks if the wallet ID exists , in which case the wallet entry is discarded
                if (WalletListFindBasedOnBitcoinID(walletList , atoi(point)) == 1 && BitcoinSimpleListFindBasedOnID(wallet->bitcoinIDsAndBlcs , atoi(point)) == NULL){
                    
                   BitcoinSimpleListPush(&wallet->bitcoinIDsAndBlcs , atoi(point) , bitcoinValue); //stores the wallet's bitcoins

                    TreeListPush(&bitcoinTreeList , atoi(point) , wallet->walletOwnerID , bitcoinValue);//stores every bitcoin in a list in order to be used for
                    //each bitcoin's history , when the commands /tracecoin and /bitcoinStatus are given

                    j++;
                    
                }  
            }
            point = strtok (NULL, " ");
        }

        wallet->numberOfBitcoins = j;//counts the number of bitcoins of a wallet
        wallet->totalBalance = j * bitcoinValue;

        WalletListPush(&walletList , *wallet);//stores the wallet to the wallet list

        free(wallet);

    }
    fclose(bitcoinBalancesFile);


    for (int k = 0 ; k < 1024 ; k++){
        buf[k] = '\0';
    }

    while (fgets(buf, sizeof(buf), transactionFile) != NULL){
        
        char* point;
        TransactionStatus tStat;
        int transactionCanBeDone = 1;

        buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
        
        point = strtok(buf ," ");
        int j = 0;
        while(point != NULL){

            switch (j)
            {
                case 0:
                    tStat.transactionID = atoi(point);
                    if (greatestTransID < tStat.transactionID){
                        greatestTransID = tStat.transactionID;
                    }
                    if (HashTableTransactionExists(senderHT , tStat.transactionID) != 0){//checks whether a transaction with the same ID already took place
                        transactionCanBeDone = 0;
                        printf("Transaction ID %d already exists!\n" , tStat.transactionID);
                    }
                    break;
            
                case 1:
                    tStat.sender = malloc(strlen(point) + 1);
                    strcpy(tStat.sender , point);
                    break;

                case 2:
                    tStat.receiver = malloc(strlen(point) + 1);
                    strcpy(tStat.receiver , point);
                    if (strcmp(tStat.sender , tStat.receiver) == 0){
                        printf("Same sender and receiver ID!\n");
                        transactionCanBeDone = 0;
                    }

                    break;

                case 3:
                    tStat.value = atoi(point);
                    if (tStat.value <= 0){
                        printf("The amount cant be <= 0!\n");
                        transactionCanBeDone = 0;
                    }
                    break;
                
                case 4:
                    tStat.date = malloc(strlen(point) + 1);
                    strcpy(tStat.date , point);
                    break;

                case 5:
                    tStat.time = malloc(strlen(point) + 1);
                    strcpy(tStat.time , point);
                    break;
                
            }
            

            j++;
            point = strtok (NULL," ");
            
    }
        if (transactionCanBeDone == 1){//if the transaction is accepted then the date is compared and the latest date and time is stored

            if (CompareDates(tStat.date , latestDate) == 1 || (CompareDates(tStat.date , latestDate) == 0 && CompareTimes(tStat.time , latestTime) == 1)){
                strcpy(latestDate , tStat.date);
                strcpy(latestTime , tStat.time);
            }

            if (TreeTransaction(&tStat , walletList , bitcoinTreeList , senderHT , receiverHT) != 0){//tries to complete the transaction
                printf("Transaction with ID : %d can't be done !\n" ,tStat.transactionID);
            }
            else {
                printf("Transaction with ID : %d was successful !\n" , tStat.transactionID);
            }
        }


        for (int k = 0 ; k < 1024 ; k++){//clears the buffer
            buf[k] = 0;
        }

        free(tStat.sender);
        free(tStat.receiver);
        free(tStat.date);
        free(tStat.time);
    }
    fclose(transactionFile);

    


    while (programExit != 1){

        programExit = InterFace(possibleCommands , &greatestTransID , &latestDate , &latestTime , walletList , bitcoinTreeList , senderHT , receiverHT);
        if (programExit == 2){
            printf("Data given to /requestTransaction or /requestTransactions were wrong!\n");
        }
        else if (programExit == 3){
            printf("Data given to /findEarnings were wrong!\n");
        }
        else if (programExit == 4){
            printf("Data given to /findPayments were wrong!\n");
        }
        else if (programExit == 5){
            printf("Data given to /walletStatus were wrong!\n");
        }
        else if (programExit == 6){
            printf("Data given to /bitcoinStatus were wrong!\n");
        }
        else if (programExit == 7){
            printf("Data given to /tracecoin were wrong!\n");            
        }
        else if (programExit == -1){
            printf("Wrong command!\n");
        }
    }   

 
    free(latestTime);
    free(latestDate);
    HashTableDelete(&receiverHT);
    HashTableDelete(&senderHT);
    WalletListDelete(&walletList);
    TreeListDelete(&bitcoinTreeList);

    return 0;
}

