#include "bankManager.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

int idNumbers = 1000;

void *myMalloc(size_t nbytes){
    /*
    Wrapper function for malloc
    Param: n bytes
    Return: void pointer
    */
    void* ptr;
    ptr = malloc(nbytes);
    if(ptr == NULL){
        printf("Failed allocating memory!\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

FILE *openFile(char *fileName, char *mode){
    /*
    Wrapper function for fopen
    Param: file name, mode to be opened
    Return: file pointer
    */
    FILE *fp;
    fp = fopen(fileName, mode);
    if(fp == NULL){
        printf("Cannot open file %s!\n", fileName);
        exit(EXIT_FAILURE);
    }
    return fp;
}

bankAccount *getAccounts(char *fileName){
    /*
    Gets the current account list from a binary file
    Param: file name
    return: accounts
    */

    FILE *fp;
    bankAccount *accounts;
    int n;

    fp = openFile(fileName, "rb");
    n = getNumAccounts(fileName);
    accounts = myMalloc(sizeof(bankAccount) * n);
    fread(accounts, sizeof(bankAccount), n, fp);
    fclose(fp);

    return accounts;
}

int getNumAccounts(char *fileName){
    /*
    Gets the current number of accounts from a binary file
    Param: file name
    return: number of accounts
    */
    int n;
    FILE *fp;

    fp = openFile(fileName, "rb");
    fseek(fp, 0l, SEEK_END);
    n = ftell(fp) / sizeof(bankAccount);
    fclose(fp);

    return n;
}

void saveData(char *fileName, bankAccount *account){
    /*
    Saves the current information of an account to a binary file
    Param: file name, bank account
    return: None
    */

    FILE *fp;
    bankAccount *accounts;
    int n, i;

    fp = openFile(fileName, "rb");
    n = getNumAccounts(fileName);
    accounts = getAccounts(fileName);
    fread(accounts, sizeof(accounts[0]), n, fp);
    fclose(fp);

    for(i = 0; i < n; i++){
        if(accounts[i].accountID == account -> accountID){
            accounts[i] = *account;
        }
    }
    fp = fopen(fileName, "wb");
    fwrite(accounts, sizeof(accounts[0]), n, fp);
    fclose(fp);
}

void createPassword(bankAccount *account){
    /*
    Creates a password for a new bank account
    param: new bank account pointer
    return: None
    */
    bool upper, lower, num, length;
    int i = 0, len = 0;
    char c;
    char *password;
    password = myMalloc(PWRDSIZE);
    do{
        printf("Create new account password, passwords must:\n\t1.Have at least one number\
        \n\t2.Have at least one upper case and lower case letter\n\t3.Be at least 8 characters\n\t4.Be max 20 characters\n");
        printf("Enter password: ");
        while(i < PWRDSIZE && (c = getchar()) != '\n'){
            password[i++] = c;
            len++;
        }
        password[i++] = '\0';
        if(strcmp(password, account -> password) == 0){
            printf("New password cannot be old password.\n");
            i = 0;
            continue;
        }
        strcpy(account -> password, password);
        if(strlen(account -> password) >= MINPWD){
            length = true;
        }
        for(i = 0; i < len; i++){
            if(islower(account -> password[i])){
                lower = true;
            }
            else if(isupper(account -> password[i])){
                upper = true;
            }
            else if(isdigit(account -> password[i])){
                num = true;
            }
        }
    }while(upper == false || lower == false || num == false);
}
void createAccount(bankAccount *account){

    int i = 0, accounts;
    char c;
    FILE *fp;

    account = myMalloc(sizeof(bankAccount));
    printf("Enter name for account (max %d characters): ", MAXNAME);

    while(i < MAXNAME && (c = getchar()) != '\n'){
        account -> name[i++] = c;
    }
    account -> name[i] = '\0';

    createPassword(account);
    
    printf("Enter checkings amount: ");
    scanf("%f", &account -> checkings);
    printf("Enter savings amount: ");
    scanf("%f", &account -> savings);

    fp = openFile(FILENAME, "ab");
    accounts = getNumAccounts(FILENAME);
    account->accountID = accounts + idNumbers;
    fwrite(account, sizeof(bankAccount), 1, fp);
    fclose(fp);
}

bankAccount verfiyPassword(void){
    char enteredPwd[PWRDSIZE + 1];
    int i = 0, numAccounts, attempts = 0;
    char c;
    FILE *fp;
    bankAccount *accounts, account;
    
    while(attempts < 3){
        printf("Enter account password (attempt %d of 3): ", attempts);
        while(i < PWRDSIZE && (c = getchar()) != '\n'){
            enteredPwd[i++] = c;
        }
        enteredPwd[i] = '\0';

        fp = openFile(FILENAME, "rb");

        numAccounts = getNumAccounts(FILENAME);

        accounts = getAccounts(FILENAME);
        fread(accounts, sizeof(bankAccount), numAccounts, fp);
        
        for(i = 0; i < numAccounts; i++){
            if(strcmp(enteredPwd, accounts[i].password) == 0){
                if(strlen(accounts[i].password) == strlen(enteredPwd)){
                    account = accounts[i];
                    free(accounts);
                    fclose(fp);
                    return account;
                }
            }
        }
        attempts++;
        i = 0;
    }
    free(accounts);
    fclose(fp);
    printf("Exceeded attempts, exiting simulator\n");
    exit(EXIT_FAILURE);
}

void printAccounts(void){

    bankAccount *accounts;
    int numAccounts, i;

    numAccounts = getNumAccounts(FILENAME);

    if(numAccounts == 0){
        printf("No accounts in records!\n");
        return;
    }
    accounts = getAccounts(FILENAME);
    for(i = 0; i < numAccounts; i++){
        printf("|Account owner: %s|Account ID: %d|Checkings ammount: %.2f|Savings ammount: %.2f|\n", accounts[i].name\
        ,accounts[i].accountID, accounts[i].checkings, accounts[i].savings);
    }
    free(accounts);
}

void printAccountInfo(bankAccount account){
    printf("*** Account info ***\n");
    printf("Owner of account: %s\n", account.name);
    printf("Account ID: %d\n", account.accountID);
    printf("Amount in checkings: %.2f\n", account.checkings);
    printf("Amount in savings: %.2f\n", account.savings);
}

void addFunds(bankAccount *account){
    float add;
    int option;
    
    printf("Select which account to add funds to:\n"); 
    printf("\t1.Checkings\n\t2.Savings\n");
    printf("Select option (1 or 2): ");
    scanf("%d", &option);
    if(option == 1){
        printf("Enter amount to enter in checkings account: ");
        scanf("%f", &add);
        account -> checkings += add;
        printf("Adding %.2f to checkings...\n", add); sleep(3);
        printf("Successfully added %.2f to checkings\n", add);
    }
    else{
        printf("Enter amount to enter in savings account: ");
        scanf("%f", &add);
        account -> savings += add;
        printf("Adding %.2f to savings...\n", add); sleep(3);
        printf("Successfully added %.2f to savings\n", add);
    }
    saveData(FILENAME, account);
    
}

void changePassword(bankAccount account){
    
    createPassword(&account);
    saveData(FILENAME, &account);
}

void deleteAccount(bankAccount account){

    FILE *fp;
    bankAccount *accounts;
    int n, pos, i;

    n = getNumAccounts(FILENAME);
    accounts = getAccounts(FILENAME);
    pos = account.accountID - 1000;

    for(i = pos; i < n; i++){
        accounts[i] = accounts[i + 1];
    }
   
    fp = openFile(FILENAME, "wb");
    fwrite(accounts, sizeof(accounts[0]), n - 1, fp);
    fclose(fp);
    free(accounts);
    
}

void updateAccountOwner(bankAccount *account){
    int i = 0;
    char c;

    printf("Enter new account owner(max %d characters): ", MAXNAME);

    while(i < MAXNAME && (c = getchar()) != '\n'){
        account -> name[i++] = c;
    }
    account -> name[i] = '\0';
    saveData(FILENAME, account);
}
