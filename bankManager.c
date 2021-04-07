#include "bankManager.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

int idNumbers = 1000;

/**********************************************
 myMalloc: requests that nbytes of storage be allocated,
 if allocation fails the program will take appropriate
 action. A pointer to the first byte in the block of 
 allocated memory is returned for usage.
**********************************************/
void *myMalloc(size_t nbytes){
   
    void *ptr;
    ptr = malloc(nbytes);
    if(ptr == NULL){
        printf("Failed allocating memory!\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

/**********************************************
 openFile: opens a file in a a given mode, if the 
 file cannot be oppened, the program will take appropriate
 action. The file pointer is returned for usage
**********************************************/
FILE *openFile(char *fileName, char *mode){
    
    FILE *fp;
    fp = fopen(fileName, mode);
    if(fp == NULL){
        printf("Cannot open file %s!\n", fileName);
        exit(EXIT_FAILURE);
    }
    return fp;
}

/**********************************************
 getAccounts: gets the current list of accounts 
 from filename as an array and returns a pointer
 to the first element in the array
**********************************************/
bankAccount *getAccounts(char *fileName){
    
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

/**********************************************
 getNumAccounts: gets the current number of 
 accounts in the array stored in filename and 
 returns it
**********************************************/
int getNumAccounts(char *fileName){
    
    int n;
    FILE *fp;

    fp = openFile(fileName, "rb");
    fseek(fp, 0l, SEEK_END);
    n = ftell(fp) / sizeof(bankAccount);
    fclose(fp);

    return n;
}


/**********************************************
 saveData: saves the current account information 
 into a file. The array of accounts will be read from
 the file, the account in the array is updated and the
 array is read back to the same file.
**********************************************/
void saveData(char *fileName, bankAccount *account){

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


/**********************************************
 createPassword: creates a password for a bank account.
 Error checking is used to make sure new passwords are not
 the same as the old password, at least one number, lower case
 and upper case character are present. Also ensures passwords 
 are longer than 8 characters.
**********************************************/
void createPassword(bankAccount *account){
    
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


/**********************************************
 createAccount: creates a bank account with a new
 user, password and initial monetary ammounts in 
 checkings and savings. The account is then saved 
 to a file.
**********************************************/
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

/**********************************************
 verifyPassword: checks to make sure that a user
 entered password matches the password saved for 
 their account. If the user enters an incorrect 
 password 3 times, the simulator will quit.
**********************************************/
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

/**********************************************
 printAccounts: displays the owner, account ID
 and ammount in checkings and savings for each account
 currently saved in the file
**********************************************/
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

/**********************************************
 printAccountInfo: displays current account information
 to a user currently logged into their account
**********************************************/
void printAccountInfo(bankAccount account){
    printf("*** Account info ***\n");
    printf("Owner of account: %s\n", account.name);
    printf("Account ID: %d\n", account.accountID);
    printf("Amount in checkings: %.2f\n", account.checkings);
    printf("Amount in savings: %.2f\n", account.savings);
}

/**********************************************
 addFunds: adds a user entered ammount of funds
 to either checkings or savings. The account is 
 then saved.
**********************************************/
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

/**********************************************
 changePassword: allows the user to change their
 account password. After changing their password
 the account is then saved.
**********************************************/
void changePassword(bankAccount account){
    
    createPassword(&account);
    saveData(FILENAME, &account);
}

/**********************************************
 deleteAccount: deletes a users account, the 
 array of accounts is read from the file and then
 all but the current account is coppied back into
 the array, the array is then saved back to the file.
**********************************************/
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

/**********************************************
 updateAccountOwner: allows for an owner of a
 bank account to update the owner of the account.
 After changing the owner, the account is the 
 saved.
**********************************************/
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
