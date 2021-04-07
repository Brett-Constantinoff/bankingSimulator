#include <stdlib.h>
#include <stdio.h>

#define PWRDSIZE 25
#define MINPWD 8
#define MAXNAME 50
#define FILENAME "bankAccounts"


typedef struct{
    int accountID;
    float checkings;
    float savings;
    char password[PWRDSIZE + 1];
    char name[MAXNAME + 1];
}bankAccount;

void *myMalloc(size_t nbytes);

FILE *openFile(char *fileName, char *mode);

void createAccount(bankAccount *account);

void createPassword(bankAccount *account);

bankAccount verfiyPassword(void);

void printAccounts(void);

void printAccountInfo(bankAccount account);

void addFunds(bankAccount *account);

void changePassword(bankAccount account);

void deleteAccount(bankAccount account);

void updateAccountOwner(bankAccount *account);

bankAccount *getAccounts(char *fileName);

int getNumAccounts(char *fileName);

void saveData(char *fileName, bankAccount *account);
