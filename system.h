//
// Created by Brett Constantinoff on 2021-04-18.
//
#ifndef BANKMANAGER_SYSTEM_H
#define BANKMANAGER_SYSTEM_H

#define MAXTRANS 50
#define NAMELEN 25
#define TIMELEN 35
#define PASSLEN 15
#define FILENAME  "clients.dat"
#define SLEEP 2

typedef struct{
    float tAmount;
    char time[TIMELEN]; //time each transaction occurred
    unsigned int account; //0 for checking, 1 for savings
    unsigned int type; //0 for sending, 1 for adding
}Transaction;

typedef struct{
    float checkings;
    float savings;
    Transaction accountTrans[MAXTRANS];
    unsigned int numTrans;
}Account;

typedef struct{
    unsigned long phoneNumber;
    char clientName[NAMELEN];
    char password[PASSLEN];
    char time[TIMELEN]; //time account is created
    Account accounts;
}Client;

/*
 Creates a new client and returns
 the struct with the new information in
 it
*/
void addClient(void);

/*
 * Prints out all current clients in the
 * system and their information
 */
void printClients(void);

/*
 * Logs into a current client account
 * based off phone number and password
 */
Client *login(void);

/*
 * Given a password, it will
 * return 0 or 1 if the given
 * password does not meet the requirements
 */
int verifyPassword(char *password);

/*
 * Opens a given file in a given mode
 */
void *openFile(char *fileName, char *mode);

/*
 * Reads input into the given string
 */
void readIntoString(char *string, int stringLen);

/*
 * Update clients name
 */
void updateClientName(char *name, char *fileName);

/*
 * Updates a clients phone number
 */
void updateClientPhone(unsigned long phone, char *fileName);

/*
 * Displays main interface menu
 */
void displayMainMenu(void);

/*
 * Displays menu for editing
 * client information
 */
void displaySubMenu1(void);

/*
 * Adds a user entered amount to an
 * account of their choice
 */
void addFunds(Client *c);

/*
 * Withdraws a user selected amount
 * of money from their account
 */
void withdrawFunds(Client *c);

/*
 * Displays transactions for current account
 */
void displayTransactions(Client *c);

/*
 * Gets a timestamp and stores it in the given
 * string
 */
void getTime(char *time, int timeLen);

/*
 * Transfers funds from the given client
 * to any other client in system
 */
void transferFunds(Client *c);

/*
 * Adds a transaction to a clients list
 * of transactions
 */
void addTransaction(Client *c, int type, int account, float amount);
#endif //BANKMANAGER_SYSTEM_H

