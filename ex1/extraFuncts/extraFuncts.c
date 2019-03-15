#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "../wallet.h"
#include "../walletList/walletList.h"
#include "../bitcoinTreeList/bitcoinTreeList.h"
#include "../bitcoinSimpleList/bitcoinSimpleList.h"
#include "../hashtable/hashtable.h"
#include "../extraFuncts/extraFuncts.h"

int CheckIfValid(char* command , char  validCommands[8][23]){//checks if the command given is valid

    if(command == NULL){
        return 1;
    }

    for (int i = 0; i < 8 ; i++){
        if (strcmp(command , validCommands[i]) == 0){
            return 0;
        }
    }
    return 1;
}

int InterFace(char possibleCommands[8][23] , int* greatestTransID, char** latestDate , char** latestTime ,
WalletListNode* walletList , Tree_List_Node* treeList , Hashtable* senderHt , Hashtable* receiverHt){

    bool first = 1;
    char* point;
    char buf[1024];
    char tempBuf[1024];

    for (int k = 0 ; k < 1024 ; k++){
        buf[k] = '\0';
    }

    printf("\nGive Input : \n");

    fgets(buf , sizeof(buf) , stdin);
    buf[strlen(buf) - 1] = '\0';

    strcpy(tempBuf , buf);

    point = strtok(buf ," ");


    if (first == 1 && CheckIfValid(point , possibleCommands) == 0){//checks which command is given and executes it
        first = 0;
        if (strcmp(point , "/exit") == 0){
            return 1;
        }
        else if (strcmp(point , "/requestTransaction") == 0){
            
            if (RequestTransaction(&point , greatestTransID, latestDate , latestTime , walletList , treeList ,senderHt , receiverHt) != 0){
                return 2;
            }

        }
        else if (strcmp(point , "/requestTransactions") == 0){
            char* tempPoint = NULL;
            tempPoint = strtok(tempBuf , " ");
            tempPoint = strtok(NULL , " ");

            if (strcmp(tempPoint , "-f") == 0 || strcmp(tempPoint , "--file") == 0){
                if (RequestTransactionReadFromFile(&point , greatestTransID, latestDate , latestTime , walletList , treeList ,senderHt , receiverHt) != 0){
                    return 2;
                }
            }
            else {
                while (point != NULL){
                    if (RequestTransaction(&point , greatestTransID, latestDate , latestTime , walletList , treeList ,senderHt , receiverHt) != 0){
                        return 2;
                    }
                }
            }
        }
        else if (strcmp(point , "/findEarnings") == 0){
            if (FindEarnings(&point , walletList , receiverHt) != 0){
                return 3;
            }
        }
        else if (strcmp(point , "/findPayments") == 0){
            if (FindPayments(&point , walletList , senderHt) != 0){
                return 4;
            }
        }
        else if (strcmp(point , "/walletStatus") == 0){
            if (GetWalledIDStatus(&point , walletList) != 0){
                return 5;
            }
        }
        else if (strcmp(point , "/bitcoinStatus") == 0){
            if (GetBitcoinIDStatus(&point , treeList) != 0){
                return 6;
            }
        }
        else if (strcmp(point , "/tracecoin") == 0){
            if (Tracecoin(&point , treeList) != 0){
                return 7;
            }
        }
    }
    else if (CheckIfValid(point , possibleCommands) != 0){
        return -1;
    }

    return 0;
}

int RequestTransactionReadFromFile(char** point , int* greatestTransID, char** latestDate , char** latestTime ,
WalletListNode* walletList , Tree_List_Node* treeList , Hashtable* senderHt , Hashtable* receiverHt){

    int count = 1;
    char* filePath = NULL;
    FILE* file;
    char buf[1024];

    while (*point != NULL){

        *point = strtok(NULL ," ");

        if (*point == NULL && count < 1){
            return 2;
        }
        else if (count > 2){
            return 2;
        }

        switch (count){//takes the file path and checks if the file can be openned
            case 1:
                filePath = malloc(strlen(*point) + 1);            
                strcpy(filePath , *point);
                 file = fopen(filePath ,"rb");
                if (file == NULL) {
                    perror("Cannot open file");
                    return 2;
                }
                break;
        }

        count++;
    }

    for (int k = 0 ; k < 1024 ; k++){
        buf[k] = '\0';
    }

    while (fgets(buf, sizeof(buf), file) != NULL){
        
        char* bufPoint;
        TransactionStatus tStat;
        tStat.date = NULL;
        tStat.time = NULL;
        tStat.receiver = NULL;
        tStat.sender = NULL;
        int transactionCanBeDone = 1;

        buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
        
        bufPoint = strtok(buf ," ;");
        int j = 0;
        while(bufPoint != NULL){

            switch (j)
            {//checks the correctness of the data given
                case 0:
                    tStat.transactionID = atoi(bufPoint);
                    if (*greatestTransID < tStat.transactionID){
                        *greatestTransID = tStat.transactionID;
                    }
                    if (HashTableTransactionExists(senderHt , tStat.transactionID) != 0){
                        transactionCanBeDone = 0;
                        printf("Transaction ID %d already exists!\n", tStat.transactionID);
                    }
                    break;
            
                case 1:
                    tStat.sender = malloc(strlen(bufPoint) + 1);
                    strcpy(tStat.sender , bufPoint);             
                    break;

                case 2:
                    tStat.receiver = malloc(strlen(bufPoint) + 1);
                    strcpy(tStat.receiver , bufPoint);
                    if (strcmp(tStat.sender , tStat.receiver) == 0){
                        printf("Same sender and receiver ID!\n");
                        free(tStat.sender);
                        free(tStat.receiver);
                        return 2;
                    }
                    break;

                case 3:
                    tStat.value = atoi(bufPoint);
                    if (tStat.value <= 0){
                        printf("The amount cant be <= 0!\n");
                        free(tStat.sender);
                        free(tStat.receiver);
                        return 2;
                    }
                    break;
                
                case 4:
                    tStat.date = malloc(strlen(bufPoint) + 1);
                    strcpy(tStat.date , bufPoint);
                    break;

                case 5:
                    tStat.time = malloc(strlen(bufPoint) + 1);
                    strcpy(tStat.time , bufPoint);
                    break;
                
            }

            j++;
            bufPoint = strtok (NULL," ");
            
        }

        if (tStat.date == NULL || tStat.time == NULL){//if the is no time OR date given then the current date and time is used.
        //if only date is given or only time is given they are ignored
                    
            int day , month , year , hours , minutes;
                
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);

            year = tm.tm_year + 1900;
            month = tm.tm_mon + 1;
            day = tm.tm_mday;
            hours = tm.tm_hour;
            minutes = tm.tm_min;

            sprintf(*latestDate , "%02d-%02d-%d" , day , month , year);
            sprintf(*latestTime , "%02d:%02d" , hours , minutes);

            tStat.date = malloc(11*sizeof(char));
            tStat.time = malloc(6*sizeof(char));

            strcpy(tStat.date , *latestDate);
            strcpy(tStat.time ,*latestTime);
        }
        //checks whether the time and date given are after the last time and date given
        else if ((CompareDates(tStat.date , *latestDate) == 1 || (CompareDates(tStat.date , *latestDate) == 0 && CompareTimes(tStat.time , *latestTime) == 1)) == false){
            
            printf("Transaction with ID : %d can't be done !\n" ,tStat.transactionID);

            transactionCanBeDone = 0;
            
        }
        else {//if the date is valid checks the time validity and if the time is valid as well then the date and time data are saved
            if (TimeIsValid(tStat.time) != 0 || TimeIsValid(*latestTime) != 0){
               
                printf("Transaction with ID : %d can't be done !\n" ,tStat.transactionID);

                transactionCanBeDone = 0;
            }

            strcpy(*latestDate , tStat.date);
            strcpy(*latestTime , tStat.time);
        }

        if (transactionCanBeDone == 1){//commences the transaction
            if (TreeTransaction(&tStat , walletList , treeList , senderHt , receiverHt) != 0){
                printf("Transaction with ID : %d can't be done !\n" ,tStat.transactionID);
            }
            else {
                printf("Transaction with ID : %d was successful !\n" , tStat.transactionID);
            }
        }
        

        for (int k = 0 ; k < 1024 ; k++){
            buf[k] = 0;
        }

        free(tStat.sender);
        free(tStat.receiver);
        free(tStat.date);
        free(tStat.time);
    }
    fclose(file);

    return 0;

}

int RequestTransaction(char** point , int* greatestTransID, char** latestDate , char** latestTime ,
WalletListNode* walletList , Tree_List_Node* treeList , Hashtable* senderHt , Hashtable* receiverHt){

    int count = 1;
    TransactionStatus tStat;
            
    tStat.date = NULL;
    tStat.time = NULL;
    tStat.receiver = NULL;
    tStat.sender = NULL;
    while (*point != NULL){
        *point = strtok(NULL ," \n");
        if(*point == NULL && count < 4){
            if (tStat.sender != NULL){
                free(tStat.sender);
            }
            if (tStat.receiver != NULL){
                free(tStat.receiver);
            }
            return 2;
        }
        else if (*point == NULL && count >= 4){
            break;
        }
        else if ((strcmp(*point , ";") == 0 && count == 4) || (strcmp(*point , ";") == 0 && count == 6)){//checks for /requestTransactions
            break;
        }
        else if (count > 6){
            free(tStat.sender);
            free(tStat.receiver);
            free(tStat.date);
            free(tStat.time);
            return 2;
        }

        switch (count){//checks the correctness of the data given
            case 1:
                tStat.sender = malloc(strlen(*point) + 1);
                strcpy(tStat.sender , *point);
                break;

            case 2:
                tStat.receiver = malloc(strlen(*point) + 1);
                strcpy(tStat.receiver , *point);
                if (strcmp(tStat.sender , tStat.receiver) == 0){
                    printf("Same sender and receiver ID!\n");
                    free(tStat.sender);
                    free(tStat.receiver);
                    return 2;
                }
                break;

            case 3:
                tStat.value = atoi(*point);
                if (tStat.value <= 0){
                    printf("The amount cant be <= 0!\n");
                    free(tStat.sender);
                    free(tStat.receiver);
                    return 2;
                }
                break;
                
            case 4:
                tStat.date = malloc(strlen(*point) + 1);
                strcpy(tStat.date , *point);
                break;

            case 5:
                tStat.time = malloc(strlen(*point) + 1);
                strcpy(tStat.time , *point);
                break;
        }

        count++;
    }

    if (tStat.date == NULL || tStat.time == NULL){//if the is no time OR date given then the current date and time is used.
        //if only date is given or only time is given they are ignored
                
        int day , month , year , hours , minutes;
               
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        year = tm.tm_year + 1900;
        month = tm.tm_mon + 1;
        day = tm.tm_mday;
        hours = tm.tm_hour;
        minutes = tm.tm_min;

        sprintf(*latestDate , "%02d-%02d-%d" , day , month , year);
        sprintf(*latestTime , "%02d:%02d" , hours , minutes);

        tStat.date = malloc(11*sizeof(char));
        tStat.time = malloc(6*sizeof(char));

        strcpy(tStat.date , *latestDate);
        strcpy(tStat.time ,*latestTime);
    }
    //checks whether the time and date given are after the last time and date given
    else if ((CompareDates(tStat.date , *latestDate) == 1 || (CompareDates(tStat.date , *latestDate) == 0 && CompareTimes(tStat.time , *latestTime) == 1)) == false){
        free(tStat.sender);
        free(tStat.receiver);
        if (tStat.date != NULL){
            free(tStat.date);
        }
        if (tStat.time != NULL){
            free(tStat.receiver);
        }
        return 2;
    }
    else {//if the date is valid checks the time validity and if the time is valid as well then the date and time data are saved
        if (TimeIsValid(tStat.time) != 0 || TimeIsValid(*latestTime) != 0){
            free(tStat.sender);
            free(tStat.receiver);
            if (tStat.date != NULL){
                free(tStat.date);
            }
            if (tStat.time != NULL){
                free(tStat.receiver);
            }
            return 2;
        }
        strcpy(*latestDate , tStat.date);
        strcpy(*latestTime , tStat.time);
    }

    *greatestTransID += 1;//increases the greatest ID number by one so that the new ID number for the new transaction is unique 
    tStat.transactionID = *greatestTransID;

    if (TreeTransaction(&tStat , walletList , treeList , senderHt , receiverHt) != 0){//does the transaction
        *greatestTransID -= 1;
        printf("Transaction with ID : %d can't be done !\n" ,tStat.transactionID);
    }
    else {
        printf("Transaction with ID : %d was successful !\n" , tStat.transactionID);
    }


    free(tStat.sender);
    free(tStat.receiver);
    free(tStat.date);
    free(tStat.time);

    return 0;
}

int FindEarnings(char** point ,WalletListNode* walletList , Hashtable* receiverHt){

    int count = 1;
    int onlyDate = 0 , onlyTime = 0 , both = 0;
    char* walletID = NULL;
    char* date1 = NULL;
    char* date2 = NULL;
    char* time1 = NULL;
    char* time2 = NULL;

    while (*point != NULL){

        *point = strtok(NULL ," ");

        if (*point == NULL && count < 2){
            return 3;
        }
        else if (*point == NULL && count >= 2){
            break;
        }
        else if (count > 6){
            return 3;
        }

        switch (count){
            case 1:
                walletID = malloc(strlen(*point) + 1);            
                strcpy(walletID , *point);
                if (WalletListFindBasedOnID(walletList , walletID) == NULL){
                    free(walletID);
                    return 3;
                }
                break;

            case 2:
                date1 = malloc(strlen(*point) + 1);            
                strcpy(date1 , *point);
                break;
                
            case 3:
                date2 = malloc(strlen(*point) + 1);            
                strcpy(date2 , *point);
                break;
                
            case 4:
                time1 = malloc(strlen(*point) + 1);            
                strcpy(time1 , *point);
                break;
            
            case 5:
                time2 = malloc(strlen(*point) + 1);            
                strcpy(time2 , *point);
                break;

        }

        count++;
    }

   
    if (FindEarnings_PaymentsInputIsValid(date1 , date2 , time1 , time2) != 0){//checks the validity of the date given

        if (walletID != NULL){
            free(walletID);
        }
        if (date1 != NULL){
            free(date1);
        }
        if (date2 != NULL){
            free(date2);
        }
        if (time1 != NULL){
            free(time1);
        }
        if (time2 != NULL){
            free(time2);
        }
        return 3;
    }
    
    if (date1 != NULL && date2 != NULL && time1 != NULL && time2 != NULL){//checks aditional date about the dates and times
        if (strcmp(date1 , date2) == 0 && CompareTimes(time1 , time2) == 1){
            if (walletID != NULL){
                free(walletID);
            }
            if (date1 != NULL){
                free(date1);
            }
            if (date2 != NULL){
                free(date2);
            }
            if (time1 != NULL){
                free(time1);
            }
            if (time2 != NULL){
                free(time2);
            }
            return 3;
        }
        both = 1;
    }
    else if (date1 != NULL && date2 != NULL){
        if (CompareDates(date1 , date2) == 1){
            if (walletID != NULL){
                free(walletID);
            }
            if (date1 != NULL){
                free(date1);
            }
            if (date2 != NULL){
                free(date2);
            }
            if (time1 != NULL){
                free(time1);
            }
            if (time2 != NULL){
                free(time2);
            }
            return 3;
        }
        onlyDate = 1;
    }
    else if (date1 == NULL && date2 == NULL && time1 != NULL && time2 != NULL){
        if (CompareTimes(time1 , time2) == 1){
            if (walletID != NULL){
                free(walletID);
            }
            if (date1 != NULL){
                free(date1);
            }
            if (date2 != NULL){
                free(date2);
            }
            if (time1 != NULL){
                free(time1);
            }
            if (time2 != NULL){
                free(time2);
            }
            return 3;
        }
        onlyTime = 1;
    }


    HtEntry* entry;
    TransactionListNode* transPoint;
    int earnings = 0;

    entry = HashTableSearchForEntry(receiverHt , walletID);
    
    transPoint = entry->tStatNext;

    while(transPoint != NULL){

        if (both == 1){//checks all the transaction of the walletID given and returns date according to the parameters given
            if ((CompareDates(transPoint->tStat.date , date1) == 1 && CompareDates(transPoint->tStat.date , date2) == 2) || \
            (CompareDates(transPoint->tStat.date , date1) == 0 && CompareTimes(transPoint->tStat.time , time1) == 1 && CompareDates(date1 , date2) != 0) || \
            (CompareDates(transPoint->tStat.date , date2) == 0 && CompareTimes(transPoint->tStat.time , time2) == 2 && CompareDates(date1 , date2) != 0) || \
            (CompareDates(date1 , date2) != 0 && CompareDates(transPoint->tStat.date , date1) == 0 && CompareTimes(transPoint->tStat.time , time1) == 1 && \
            CompareTimes(transPoint->tStat.time , time2))){

                earnings += transPoint->tStat.value;
                printf("%s -> %s\n" , transPoint->tStat.sender , transPoint->tStat.receiver);
            }
        }
        else if (onlyDate == 1){
            if(CompareDates(transPoint->tStat.date , date1) <= 1 && CompareDates(date2 , transPoint->tStat.date) <= 1){
                earnings += transPoint->tStat.value; 
                printf("%s -> %s\n" , transPoint->tStat.sender , transPoint->tStat.receiver);

            }
        }
        else if (onlyTime == 1){
            if (CompareTimes(transPoint->tStat.time , date1) <= 1 && CompareTimes(time2 , transPoint->tStat.time) <= 1){
                earnings += transPoint->tStat.value;  
                printf("%s -> %s\n" , transPoint->tStat.sender , transPoint->tStat.receiver);

            }
        }
        else{
            earnings += transPoint->tStat.value;
            printf("%s -> %s\n" , transPoint->tStat.sender , transPoint->tStat.receiver);
        }

        transPoint = transPoint->next;
    }

    printf("Overall earnings : %d\n" , earnings);//prints overall earnings and prints info about the transactions

        if (walletID != NULL){
            free(walletID);
        }
        if (date1 != NULL){
            free(date1);
        }
        if (date2 != NULL){
            free(date2);
        }
        if (time1 != NULL){
            free(time1);
        }
        if (time2 != NULL){
            free(time2);
        }

    return 0;
}



int FindPayments(char** point ,WalletListNode* walletList , Hashtable* senderHt){

    int count = 1;
    int onlyDate = 0 , onlyTime = 0 , both = 0;
    char* walletID = NULL;
    char* date1 = NULL;
    char* date2 = NULL;
    char* time1 = NULL;
    char* time2 = NULL;

    while (*point != NULL){

        *point = strtok(NULL ," ");

        if (*point == NULL && count < 2){
            return 4;
        }
        else if (*point == NULL && count >= 2){
            break;
        }
        else if (count > 6){
            if (walletID != NULL){
                free(walletID);
            }
            if (date1 != NULL){
                free(date1);
            }
            if (date2 != NULL){
                free(date2);
            }
            if (time1 != NULL){
                free(time1);
            }
            if (time2 != NULL){
                free(time2);
            }
            return 4;
        }

        switch (count){
            case 1:
                walletID = malloc(strlen(*point) + 1);            
                strcpy(walletID , *point);
                if (WalletListFindBasedOnID(walletList , walletID) == NULL){
                    free(walletID);
                    return 4;
                }
                break;

            case 2:
                date1 = malloc(strlen(*point) + 1);            
                strcpy(date1 , *point);
                break;
                
            case 3:
                date2 = malloc(strlen(*point) + 1);            
                strcpy(date2 , *point);
                break;
                
            case 4:
                time1 = malloc(strlen(*point) + 1);            
                strcpy(time1 , *point);
                break;
            
            case 5:
                time2 = malloc(strlen(*point) + 1);            
                strcpy(time2 , *point);
                break;

        }

        count++;
    }

   
    if (FindEarnings_PaymentsInputIsValid(date1 , date2 , time1 , time2) != 0){//checks the validity of the date given
        if (walletID != NULL){
            free(walletID);
        }
        if (date1 != NULL){
            free(date1);
        }
        if (date2 != NULL){
            free(date2);
        }
        if (time1 != NULL){
            free(time1);
        }
        if (time2 != NULL){
            free(time2);
        }
        return 4;
    }
    
    if (date1 != NULL && date2 != NULL && time1 != NULL && time2 != NULL){//additional checks
        if (strcmp(date1 , date2) == 0 && CompareTimes(time1 , time2) == 1){
            if (walletID != NULL){
                free(walletID);
            }
            if (date1 != NULL){
                free(date1);
            }
            if (date2 != NULL){
                free(date2);
            }
            if (time1 != NULL){
                free(time1);
            }
            if (time2 != NULL){
                free(time2);
            }
            return 4;
        }
        both = 1;
    }
    else if (date1 != NULL && date2 != NULL){
        if (CompareDates(date1 , date2) == 1){
            if (walletID != NULL){
                free(walletID);
            }
            if (date1 != NULL){
                free(date1);
            }
            if (date2 != NULL){
                free(date2);
            }
            if (time1 != NULL){
                free(time1);
            }
            if (time2 != NULL){
                free(time2);
            }
            return 4;
        }
        onlyDate = 1;
    }
    else if (date1 == NULL && date2 == NULL && time1 != NULL && time2 != NULL){
        if (CompareTimes(time1 , time2) == 1){
            if (walletID != NULL){
                free(walletID);
            }
            if (date1 != NULL){
                free(date1);
            }
            if (date2 != NULL){
                free(date2);
            }
            if (time1 != NULL){
                free(time1);
            }
            if (time2 != NULL){
                free(time2);
            }
            return 4;
        }
        onlyTime = 1;
    }


    HtEntry* entry;
    TransactionListNode* transPoint;
    int payments = 0;

    entry = HashTableSearchForEntry(senderHt , walletID);
    
    transPoint = entry->tStatNext;

    while(transPoint != NULL){

        if (both == 1){//checks all the transaction of the walletID given and returns date according to the parameters given
            if ((CompareDates(transPoint->tStat.date , date1) == 1 && CompareDates(transPoint->tStat.date , date2) == 2) || \
            (CompareDates(transPoint->tStat.date , date1) == 0 && CompareTimes(transPoint->tStat.time , time1) == 1 && CompareDates(date1 , date2) != 0) || \
            (CompareDates(transPoint->tStat.date , date2) == 0 && CompareTimes(transPoint->tStat.time , time2) == 2 && CompareDates(date1 , date2) != 0) || \
            (CompareDates(date1 , date2) != 0 && CompareDates(transPoint->tStat.date , date1) == 0 && CompareTimes(transPoint->tStat.time , time1) == 1 && \
            CompareTimes(transPoint->tStat.time , time2))){

                payments += transPoint->tStat.value;
                printf("%s -> %s\n" , transPoint->tStat.sender , transPoint->tStat.receiver);
            }
        }
        else if (onlyDate == 1){
            if(CompareDates(transPoint->tStat.date , date1) <= 1 && CompareDates(date2 , transPoint->tStat.date) <= 1){
                payments += transPoint->tStat.value; 
                printf("%s -> %s\n" , transPoint->tStat.sender , transPoint->tStat.receiver);

            }
        }
        else if (onlyTime == 1){
            if (CompareTimes(transPoint->tStat.time , date1) <= 1 && CompareTimes(time2 , transPoint->tStat.time) <= 1){
                payments += transPoint->tStat.value;  
                printf("%s -> %s\n" , transPoint->tStat.sender , transPoint->tStat.receiver);

            }
        }
        else{
            payments += transPoint->tStat.value;
            printf("%s -> %s\n" , transPoint->tStat.sender , transPoint->tStat.receiver);
        }

        transPoint = transPoint->next;
    }

    printf("Overall payments : %d\n" , payments);//prints overall payments and prints info about the transactions

    if (walletID != NULL){
        free(walletID);
    }
    if (date1 != NULL){
        free(date1);
    }
    if (date2 != NULL){
        free(date2);
    }
    if (time1 != NULL){
        free(time1);
    }
    if (time2 != NULL){
        free(time2);
    }

    return 0;
}

int GetWalledIDStatus(char** point , WalletListNode* walletList){//finds the data from the wallet list and prints them

    int count = 1;
    char* walletID = NULL;
    WalletListNode* node = NULL;

     while (*point != NULL){

        *point = strtok(NULL ," ");

        if (*point == NULL && count < 1){
            return 5;
        }
        else if (count > 2){
            return 5;
        }

        switch (count){
            case 1:
                walletID = malloc(strlen(*point) + 1);            
                strcpy(walletID , *point);
                if ((node = WalletListFindBasedOnID(walletList , walletID)) == NULL){
                    return 5;
                }
                break;
        }

        count++;
    }

    printf("Wallet's Bitcoin price is equal to : %d $\n" , node->wal.totalBalance);

    return 0;
}

int GetBitcoinIDStatus(char** point , Tree_List_Node* bitcoinTreeList){

    int count = 1;
    int bitcoinID = -1;
    Tree_List_Node* bitcoinNode = NULL;
    Tree_Node* treeNode = NULL;

    while (*point != NULL){

        *point = strtok(NULL ," ");

        if (*point == NULL && count < 1){
            return 6;
        }
        else if (count > 2){
            return 6;
        }

        switch (count){
            case 1:
                bitcoinID = atoi(*point);          
                if ((bitcoinNode = TreeListFindBasedOnID(bitcoinTreeList , bitcoinID)) == NULL){
                    return 6;
                }
                break;
        }

        count++;
    }

    treeNode = &bitcoinNode->rootNode;

    printf("Bitcoin's value is : %d $\n" , treeNode->balance);

    while(treeNode != NULL){//finds the right-most leaf of the bitcoin tree and prints its value since this is the unspent value
        if (treeNode->rightChild != NULL){
            treeNode = treeNode->rightChild;
        }
        else{
            break;
        }
    }

    printf("Bitcoin's unspent value is : %d\n" , treeNode->balance);

    int numOfTransactions = 0;

    TreeTraverse(&bitcoinNode->rootNode , &numOfTransactions);//traverses the bitcoin tree and increases the numOfTransactions for every left child of a
    //node , because the transactions are as many as these nodes

    printf("The number of transactions this bitcoin was used in is : %d\n" , numOfTransactions);

    return 0;
}

int Tracecoin(char** point , Tree_List_Node* bitcoinTreeList){
    
    int count = 1;
    int bitcoinID = -1;
    Tree_List_Node* bitcoinNode = NULL;

    while (*point != NULL){

        *point = strtok(NULL ," ");

        if (*point == NULL && count < 1){
            return 7;
        }
        else if (count > 2){
            return 7;
        }

        switch (count){
            case 1:
                bitcoinID = atoi(*point);          
                if ((bitcoinNode = TreeListFindBasedOnID(bitcoinTreeList , bitcoinID)) == NULL){
                    return 7;
                }
                break;
        }

        count++;
    }

    TreeTraversePrint(&bitcoinNode->rootNode);//ttraverses the tree and prints the transaction data of every node on the bitcoin tree

    return 0;
}

void swap(char** par1 , char** par2){
    char* temp;
    temp = *par1;
    *par1 = *par2;
    *par2 = temp;
}

int FindEarnings_PaymentsInputIsValid(char* date1 , char* date2 , char* time1 , char* time2){
    //checks whether only time or date is given or both and if only time is given then 
    //the time values are passed to corresponded variables
    //also time and date validity is checked 
     
    if (date1 == NULL && date2 == NULL && time1 == NULL && time2 == NULL){
        return 0;
    }

    if (date1 != NULL && date2 != NULL){
        if (DateIsValid(date1) == 0){
            if (DateIsValid(date2) != 0){
                return 3;
            }
        }
        else if (TimeIsValid(date1) == 0){
            swap(&time1 , &date1);
            swap(&time2 , &date2);
        }
        else {
            return 3;
        }
    }
    else {
        return 3;
    }
    
    if (time1 != NULL && time2 != NULL){
        if (TimeIsValid(time1) == 0){
            if (TimeIsValid(time2) != 0){
                return 3;
            }
        }
        else {
            return 3;
        }
    }
    else if ((time1 != NULL && time2 == NULL) || time1 == NULL && time2 != NULL){
        return 3;
    }

    return 0;
}

int CompareTimes(char* rtime1 , char* rtime2){
    //return -1 if there is an error , 1 if rtime1 is greater then rtime2 , 
    //0 if rtime1 is the same as rtime2 , and 2 if rtime2 is greater than rtime1

    if (rtime1 == NULL || rtime2 == NULL){
        return -1;
    }

    if (TimeIsValid(rtime1) != 0 || TimeIsValid(rtime2) != 0){
        return -1;
    }

    char* point1 = NULL;
    char* point2 = NULL;
    char* time1 = NULL;
    char* time2 = NULL;
    int minutes1 = 0 , hours1 = 0;
    int minutes2 = 0 , hours2 = 0;

    time1 = malloc(strlen(rtime1) + 1);
    time2 = malloc(strlen(rtime2) + 1);

    strcpy(time1 , rtime1);
    strcpy(time2 , rtime2);

    point1 = strtok(time1 ,":");

    int count = 0;

    while(point1 != NULL){
        switch (count){
            case 0:
                hours1 = atoi(point1);
                break;
        
            case 1:
                minutes1 = atoi(point1);
                break;
        }
        point1 = strtok(NULL ,":");
        count++;
    }    

    count = 0;
    point2 = strtok(time2 ,":");

    while(point2 != NULL){
        
         switch (count){
            case 0:
                hours2 = atoi(point2);
                break;
        
            case 1:
                minutes2 = atoi(point2);
                break;
        }
        point2 = strtok(NULL ,":");
        count++;
    }

    free(time1);
    free(time2);

    
    //the real comparison
    if (hours1 > hours2){
        return 1;
    }
    else if (hours1 == hours2){
        if (minutes1 > minutes2){
            return 1;
        }
        else if (minutes1 == minutes2){
            return 0;
        }
        else {
            return 2;
        }
    }
    else {
        return 2;
    }
    
    return -1;
}

int TimeIsValid(char* rtime){//checks whether the time given is logical (not over 24 hours or 60 mins etc)

    if (rtime == NULL){
        return 1;
    }

    char* point = NULL;
    char* time1 = NULL;
    int minutes = 0 , hours = 0;

    time1 = malloc(strlen(rtime) + 1);

    strcpy(time1 , rtime);

    point = strtok(time1 ,":");

    int count = 0;

    while(point != NULL){
        switch (count){
            case 0:
                hours = atoi(point);
                break;
        
            case 1:
                minutes= atoi(point);
                break;
        }
        point = strtok(NULL ,":");
        count++;
    }

    free(time1);

    if (hours < 0 || hours > 23){
        return 1;
    }
    else if (minutes < 0 || minutes > 59){
        return 1;
    }

    return 0;    
}

int CompareDates(char* rdate1 , char* rdate2){//return -1 if there is an error , 1 if rdate1 is greater then rdate2 , 
    //0 if rdate1 is the same as rdate2 , and 2 if rdate2 is greater than rdate1

    if (rdate1 == NULL || rdate2 == NULL){
        return -1;
    }

    if (DateIsValid(rdate1) != 0 || DateIsValid(rdate2) != 0){
        return -1;
    }

    char* point1 = NULL;
    char* point2 = NULL;
    char* date1 = NULL;
    char* date2 = NULL;
    int day1 = 0 , month1 = 0 , year1 = 0;
    int day2 = 0 , month2 = 0 , year2 = 0;

    date1 = malloc(strlen(rdate1) + 1);
    date2 = malloc(strlen(rdate2) + 1);

    strcpy(date1 , rdate1);
    strcpy(date2 , rdate2);

    point1 = strtok(date1 ,"-");
    int count = 0;

    while(point1 != NULL){
        switch (count){
            case 0:
                day1 = atoi(point1);
                break;
        
            case 1:
                month1 = atoi(point1);
                break;
            
            case 2:
                year1 = atoi(point1);
                break;
        }
        point1 = strtok(NULL ,"-");
        count++;
    }


    count = 0;
    point2 = strtok(date2 ,"-");

    while(point2 != NULL){
        
         switch (count){
            case 0:
                day2 = atoi(point2);
                break;
        
            case 1:
                month2 = atoi(point2);
                break;
            
            case 2:
                year2 = atoi(point2);
                break;
        }
        point2 = strtok(NULL ,"-");
        count++;
    }

    free(date1);
    free(date2);

    if (day2 == 0 || month2 == 0 || year2 == 0 || day1 == 0 || month1 == 0 || year1 == 0){
        return -1;
    }

    if (year1 > year2){
        return 1;
    }
    else if (year1 == year2){
        if (month1 > month2){
            return 1;
        }
        else if (month1 == month2){
            if (day1 > day2){
                return 1;
            }
            else if (day1 == day2){
                return 0;
            }
            else {
                return 2;
            }
        }
        else {
            return 2;
        }
    }
    else {
        return 2;
    }
    
    return -1;
}

int DateIsValid(char* rdate){//checks whether the date is logical or not (ex not 29-13-2019 or 29-2-2019)

    if (rdate == NULL){
        return 1;
    }

    char* point = NULL;
    char* date = NULL;
    int day = 0 , month = 0 , year = 0;

    date = malloc(strlen(rdate) + 1);

    strcpy(date , rdate);

    point = strtok(date ,"-");
    int count = 0;

    while(point != NULL){
        switch (count){
            case 0:
                day = atoi(point);
                break;
        
            case 1:
                month = atoi(point);
                break;
            
            case 2:
                year = atoi(point);
                break;
        }
        point = strtok(NULL ,"-");
        count++;
    }

    free(date);

    if (day < 1 || day > 30){
        return 1;
    }
    else if (month < 1 || month > 12){
        return 1;
    }
    else if (month == 2 && day > 28){
        return 1;
    }

    return 0;
}

int TreeListDelete(Tree_List_Node** treeList){

    while(*treeList != NULL){
        TreeListDeleteFirst(treeList);
    }

}

int WalletListDelete(WalletListNode** walletList){

    while(*walletList != NULL){
        WalletListDeleteFirst(walletList);
    }

} 
