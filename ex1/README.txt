######################################################################################################################

GENERAL :
    
    ->COMPILE AND RUN :
        In order to compile use "make" . If you want to delete the files created by the compilation
        use "make clean".
        In order to run the program :
        ./bitcoin -a bitcoinBalancesFile.txt -t transactionsFile.txt  -v 100 -h1 5 -h2 5 -b 5
        
        -a: balances file
        -t: transaction file
        -v: bitcoin value
        -h1: sender hash table entries
        -h2: receiver hash table entries
        -b: entries that fit into buckets
        
        
    
######################################################################################################################

COMMANDS :

    ->You can give several commands to the program while it is running:
    
    1) /requestTransaction 
    You can make a transaction between two people. You can also set the date and time at which the transaction will
    happen 
    ex. /requestTransaction hackerman richard 12 12-12-2012 12:12
    
    If you omit either the date or the time then the date and the time will be set at the current date and time.
    ex. /requestTransaction hackerman richard 12
    
    If you add more or less parameters than needed the command will not go through
    
    2) /requestTransactions
    This command takes multiple transactions like so :
    ex /requestTransaction hackerman richard 12 12-12-2012 12:12 ; crystalsmith richard 15 12-11-2018 19:10 ; hackerman richard 102 ; richard crystalsmith 53
    DO NOT USE ENTER AFTER INPUTING ";" BECAUSE THE PROGRAM TAKES INPUT WITH getline SO IF YOU PRESS ENTER IT WILL
    START THE TRANSACTIONS.
    
    You can also use a file in order to do transactions like so:
    /requestTransactions -f (or --file) [filePath]
    ex. /requestTransactions -f ./newTransactions.txt
    
    The new file can have any form of information in the below format :
    14 richard hackerman 50 12-11-2019 23:11
    93 hackerman crystalsmith 19
    12 richard hackerman 12 ;
    119 crystalsmith hackerman 50 10-11-2022 21:11 ;
    
    You can omit the date and time
    
    3) /findEarnings 
    Finds earnings and who gave it to the person identified by the walletID given . You can use this command like so:
    /findEarnings [walletID]
    /findEarnings [walletID] [date1][date2]
    /findEarnings [walletID] [time1][time2]
    /findEarnings [walletID] [date1][date2][time1][time2]
    
    Other formats are not accepted.
    
    4) /findPayments
    Similar to the previous command /findPayments finds the payments and who was given money from the person identified by the walletID given.You can use it like so :
    /findPayments [walletID]
    /findPayments [walletID] [date1][date2]
    /findPayments [walletID] [time1][time2]
    /findPayments [walletID] [date1][date2][time1][time2]
    
    5) /walletStatus 
    It returns the balance of the wallet .
    ex. /walletStatus [walletID]
    
    6) /bitcoinStatus 
    It return the starting value of the bitcoin , the unspent value of it and the number of transactions it has been used.
    ex. /bitcoinStatus [walletID]
 
    /bitcoinStatus 692

    OUTPUT :
    Bitcoin's value is : 100 $
    Bitcoin's unspent value is : 70
    The number of transactions this bitcoin was used in is : 4

    7) /tracecoin
    Returns the history of the bitcoin given. 
    ex. /tracecoin 692
    
    OUTPUT:
    249 lookingforagoogusernametoo shirleysparkerlittlewallet 30 30-06-2018 15:26
    111 oneofthemanybitcoinminersforfreeinternet lookingforagoogusernametoo 26 01-01-2017 04:52
    78 hackerman oneofthemanybitcoinminersforfreeinternet 10 21-11-2018 16:09
    45 norvaldach hackerman 30 11-01-2017 14:46
    
    8) /exit
    Finally use /exit in order to quit the program
    
    
######################################################################################################################

    HASH FUNCTION :
    For a hash function %(mod) was used , by adding the ascii number of the string and the modding to the number of
    possible indexes in the hash table.
    
