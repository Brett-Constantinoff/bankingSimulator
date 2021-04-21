//
// Created by Brett Constantinoff on 2021-04-18.
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "system.h"

void addClient(void){

    Client newClient;
    FILE *fp;

    printf("Enter name for account: ");
    readIntoString(newClient.clientName, NAMELEN);

    printf("Enter phone number for account: "); //verify phone number
    scanf("%lu", &newClient.phoneNumber);
    getchar();//eats '\n'

    do {
        printf("Password must\n-Be at least 10 characters long\n-Contain at least 1 digit\n-Have one upper and lower case letter\nEnter password for account: ");
        readIntoString(newClient.password, PASSLEN);
    }while(verifyPassword(newClient.password) != 0);

    printf("Enter initial amount in checkings: ");
    scanf("%f", &newClient.accounts.checkings);
    while(newClient.accounts.checkings < 0){
        printf("Error: Invalid amount of %.2f entered for account: checkings\n", newClient.accounts.checkings);
        printf("Enter initial amount in checkings: ");
        scanf(" %f", &newClient.accounts.checkings);
    }
    printf("Enter initial amount in savings: ");
    scanf("%f", &newClient.accounts.savings);
    while(newClient.accounts.savings < 0){
        printf("Error: Invalid amount of %.2f entered for account: savings\n", newClient.accounts.savings);
        printf("Enter initial amount in savings: ");
        scanf(" %f", &newClient.accounts.savings);
    }
    getchar();

    getTime(newClient.time, TIMELEN);
    newClient.accounts.numTrans = 0;

    fp = (FILE *) openFile(FILENAME, "ab");
    fwrite(&newClient, sizeof(Client), 1, fp);
    fclose(fp);

    printf("Adding new client...\n");
    sleep(SLEEP);
    printf("Successfully added %s as a new client!\n", newClient.clientName);
}

int verifyPassword(char *password){

    int i;
    bool upper, lower, number, length;

    upper = lower = number = length = false;
    if(strlen(password) >= 10){
        length = true;
    }
    for(i = 0; i < strlen(password); i++){
        if(isdigit(password[i]) == 0){
            number = true;
        }
        if(islower(password[i]) == 0){
            lower = true;
        }
        if(isupper(password[i]) == 0){
            upper = true;
        }
    }
    if(upper == true && lower == true && number == true && length == true){
        return 0;
    }
    return 1;
}

Client *login(void){

    unsigned long phone;
    char password[PASSLEN];
    FILE *fp;
    Client *c;

    c = malloc(sizeof(Client));
    if(c == NULL){
        fprintf(stderr, "Error allocating memory in %s line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    printf("Enter client phone number: ");
    scanf("%lu", &phone);
    getchar();

    fp = openFile(FILENAME, "rb+");
    while((fread(c, sizeof(Client), 1, fp)) != 0){
        if(c->phoneNumber == phone){
            printf("Enter account password: ");
            readIntoString(password, PASSLEN);
            if(strcmp(password, c->password) != 0){
                c = NULL;
                return c;
            }
            else{
                printf("Login successful.\n");
                fclose(fp);
                return c;
            }
        }
    }
    fclose(fp);
    c = NULL;
    return c;
}
void printClients(void){

    FILE *fp;
    Client c;

    fp = (FILE *) openFile(FILENAME, "rb");
    printf("Client Name              |Phone Number |Amount in Checkings |Amount in Savings |Date Created\n"); // finish formatting
    while((fread(&c, sizeof(Client), 1, fp)) != 0){
        printf("%-25s|%-12lu |%-20.2f|%-18.2f|%-14s", c.clientName, c.phoneNumber, c.accounts.checkings, c.accounts.savings,c.time);
    }
    fclose(fp);
}

void *openFile(char *fileName, char *mode){

    FILE *fp;
    fp = fopen(fileName, mode);
    if(fp == NULL){
        fprintf(stderr, "Error opening file in %s line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    return (void *)fp;
}

void getTime(char *timeStr, int timeLen){

    time_t rawTime;
    struct tm *timeInfo;

    time(&rawTime);
    timeInfo = localtime(&rawTime);
    strncpy(timeStr, asctime(timeInfo), timeLen);

}

void readIntoString(char *string, int strLen){

    int i = 0;
    char c;

    while(i < strLen && (c = getchar()) != '\n'){
        string[i++] = c;
    }
    string[i] = '\0';
}

void updateClientName(char *name, char *fileName){

    FILE *fp;
    Client temp;
    char updatedName[NAMELEN];

    fp = fopen(fileName, "rb+");
    while((fread(&temp, sizeof(Client), 1, fp)) != 0){
        if(strcmp(temp.clientName, name) == 0){
            printf("Enter updated client name: ");
            readIntoString(updatedName, NAMELEN);

            strcpy(temp.clientName, updatedName);
            fseek(fp, -sizeof(Client), SEEK_CUR);

            fwrite(&temp, sizeof(Client), 1, fp);
            fclose(fp);
            printf("Successfully updated client name.\n");
            return;
        }
    }
}

void updateClientPhone(unsigned long phone, char *fileName){

    FILE *fp;
    Client temp;

    fp = fopen(fileName, "rb+");
    while((fread(&temp, sizeof(Client), 1, fp)) != 0){
        if(temp.phoneNumber == phone){
            printf("Enter updated client phone number: ");
            scanf("%lu", &temp.phoneNumber);
            fseek(fp, -sizeof(Client), SEEK_CUR);
            fwrite(&temp, sizeof(Client), 1, fp);
            fclose(fp);
            printf("Successfully updated client phone number.\n");
            return;
        }
    }
}

void addFunds(Client *c){
    int opt;
    float funds;
    FILE *fp;
    Client temp;

    printf("Select which account to add funds to: ");
    printf("\n[1] - Checkings");
    printf("\n[2] - Savings");
    printf("\nEnter option: ");
    scanf("%d", &opt);
    printf("Enter funds to add: ");
    scanf("%f", &funds);

    if(opt == 1){
        c->accounts.checkings += funds;
        printf("Successfully added %.2f funds to account: checkings\n", funds);
    }
    else{
        c->accounts.savings += funds;
        printf("Successfully added %.2f funds to account: savings\n", funds);
    }
    addTransaction(c, 1, opt, funds);

    fp = (FILE *) openFile(FILENAME, "rb+");
    while((fread(&temp, sizeof(Client), 1, fp)) != 0){
        if(temp.phoneNumber == c->phoneNumber){
            temp = *c;
            fseek(fp, -sizeof(Client), SEEK_CUR);
            fwrite(&temp, sizeof(Client), 1, fp);
            fclose(fp);
        }
    }
}

void withdrawFunds(Client *c){
    int opt;
    float funds;
    FILE *fp;
    Client temp;

    printf("Select which account to withdraw funds from: ");
    printf("\n[1] - Checkings");
    printf("\n[2] - Savings");
    printf("\nEnter option: ");
    scanf("%d", &opt);
    printf("Enter funds to withdraw: ");
    scanf("%f", &funds);

    if(opt == 1){
        if(c->accounts.checkings - funds < 0){
            printf("Error withdrawing %.2f funds from account: checkings\n", funds);
            return;
        }
        c->accounts.checkings -= funds;
        printf("Successfully withdrew %.2f funds to account: checkings\n", funds);
    }
    else{
        if(c->accounts.checkings - funds < 0){
            printf("Error withdrawing %.2f funds from account: checkings\n", funds);
            return;
        }
        c->accounts.savings -= funds;
        printf("Successfully withdrew %.2f funds to account: savings\n", funds);
    }
    addTransaction(c, 0, opt, funds);

    fp = (FILE *) openFile(FILENAME, "rb+");
    while((fread(&temp, sizeof(Client), 1, fp)) != 0){
        if(temp.phoneNumber == c->phoneNumber){
            temp = *c;
            fseek(fp, -sizeof(Client), SEEK_CUR);
            fwrite(&temp, sizeof(Client), 1, fp);
            fclose(fp);
        }
    }
}

void displayTransactions(Client *c){

    int i;

    printf("|Transaction Amount|Account of Transaction|Type of Transaction|Time of Transactions\n");
    for(i = 0; i < c->accounts.numTrans; i++) {
        if (c->accounts.accountTrans[i].account == 1) {
            if (c->accounts.accountTrans[i].type == 1) {
                printf("|%-18.2f|Checkings             |Added funds        |%s", c->accounts.accountTrans[i].tAmount,
                       c->accounts.accountTrans[i].time);
            } else {
                printf("|%-18.2f|Checkings             |Withdrew funds     |%s", c->accounts.accountTrans[i].tAmount,
                       c->accounts.accountTrans[i].time);
            }
        } else {
            if (c->accounts.accountTrans[i].type == 1) {
                printf("|%-18.2f|Savings               |Added funds        |%s", c->accounts.accountTrans[i].tAmount,
                       c->accounts.accountTrans[i].time);
            } else {
                printf("|%-18.2f|Savings               |Withdrew funds     |%s", c->accounts.accountTrans[i].tAmount,
                       c->accounts.accountTrans[i].time);
            }
        }
    }
}

void transferFunds(Client *c){

    unsigned long phone;
    float transfer;
    int account;
    FILE *fp;
    Client *temp;

    temp = malloc(sizeof(Client));
    if(temp == NULL){
        fprintf(stderr, "Error allocating memory in %s line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    printClients();
    printf("Enter phone number of client who will receive funds: ");
    scanf("%lu", &phone);
    printf("Enter account of transfer(1 for checkings 0 for savings): ");
    scanf(" %d", &account);
    printf("Enter amount of transfer: ");
    scanf( "%f", &transfer);

    if(account == 1){
        if(c->accounts.checkings - transfer < 0){
            printf("Error transfering %.2f funds from account: checkings\n", transfer);
            return;
        }
        c->accounts.checkings -= transfer;
    }
    else{
        if(c->accounts.savings - transfer < 0){
            printf("Error transfering %.2f funds from account: savings\n", transfer);
            return;
        }
        c->accounts.savings -= transfer;
    }
    addTransaction(c, 0, account, transfer);
    fp = (FILE *)openFile(FILENAME, "rb+");
    while((fread(temp, sizeof(Client), 1, fp)) != 0){
        if(phone == temp->phoneNumber){
            if(account == 1){
                temp->accounts.checkings += transfer;
            }
            else{
                temp->accounts.savings += transfer;
            }
            addTransaction(temp, 0, account, transfer);
            fseek(fp, -sizeof(Client), SEEK_CUR);
            fwrite(temp, sizeof(Client), 1, fp);
        }
        else if(temp->phoneNumber == c->phoneNumber){
            temp = c;
            fseek(fp, -sizeof(Client), SEEK_CUR);
            fwrite(temp, sizeof(Client), 1, fp);
        }
    }
    fclose(fp);
}

void addTransaction(Client *c, int type, int account, float amount){

    Transaction newTrans;
    getTime(newTrans.time, TIMELEN);
    newTrans.type = type;
    newTrans.account = account;
    newTrans.tAmount = amount;

    c->accounts.accountTrans[c->accounts.numTrans] = newTrans;
    c->accounts.numTrans += 1;

    if(c->accounts.numTrans >= 50){
        c->accounts.numTrans = 0;
    }

}
void displayMainMenu(void){

    printf("Select option:");
    printf("\n[1] - Create new client");
    printf("\n[2] - Login to existing client account");
    printf("\n[3] - Display Current Clients");
    printf("\n[4] - Save and Quit");
    printf("\nEnter option: ");
}

void displaySubMenu1(void){

    printf("Select option:");
    printf("\n[1] - Update client name");
    printf("\n[2] - Update client phone number");
    printf("\n[3] - Add funds");
    printf("\n[4] - Withdraw funds");
    printf("\n[5] - Display account transactions");
    printf("\n[6] - Transfer funds");
    printf("\n[7] - Return to main menu");
    printf("\nEnter option: ");
}
