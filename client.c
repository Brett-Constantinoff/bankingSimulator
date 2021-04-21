#include "system.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

    int mainOpt, subOpt;
    Client *c;

    for(;;){
        displayMainMenu();
        scanf("%d", &mainOpt);
        getchar();
        switch(mainOpt){
            case 1:addClient(); break;
            case 2:
                c = login();
                if(c == NULL) {printf("Invalid credentials.\n"); break;}
                for(;;) {
                    displaySubMenu1();
                    scanf("%d", &subOpt);
                    getchar();
                    if(subOpt == 7){break;}
                    switch (subOpt) {
                        case 1: updateClientName(c->clientName, FILENAME); break;
                        case 2: updateClientPhone(c->phoneNumber, FILENAME); break;
                        case 3: addFunds(c); break;
                        case 4: withdrawFunds(c); break;
                        case 5: displayTransactions(c); break;
                        case 6: transferFunds(c); break;
                    }
                }
                break;
            case 3: printClients(); break;
            case 4: free(c); return 0;
        }
    }
}
