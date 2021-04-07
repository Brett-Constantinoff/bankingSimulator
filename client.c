#include "bankManager.h"
#include <stdio.h>
#include <unistd.h>

int main(void){
    int option;
    bankAccount account;

    printf("*** Welcome to Bank Simulator 2021 ***\n");
    for(;;){
        printf("Please select an option to get started:\n");
        printf("\t1.Create new account\n\t2.Login to existing account\n\t3.View accounts\n\t4.Save and exit simulator\n");
        printf("Select option (1-4): ");
        scanf("%d", &option);
        getchar();
        switch(option){
            case 1:
                createAccount(&account);
                printf("Creating account...\n"); sleep(3);
                printf("Account created successfully!\n");
                break;
            case 2:
                //login to account
                //display options for 1.printing info 2. changing password
                //3. adding funds 4. tranferring funds 5.deleting acount
                account = verfiyPassword();
                for(;;){
                    printf("Please select an option:\n");
                    printf("\t1.Print account info\n\t2.Add funds\n\t3.Transfer funds\
                    \n\t4.Change Password\n\t5.Update account owner\n\t6.Delete account\n\t7.Return to main menu\n");
                    printf("Select option (1-7): ");
                    scanf("%d", &option);
                    if (option == 7){
                        break; //returns to main menu
                    }
                    else if(option == 6){
                        printf("Are you sure you want to delete your account? (1.Yes or 2.No): ");
                        scanf("%d", &option);
                        if(option == 1){
                            printf("Deleting account...\n"); sleep(3);
                            printf("Transfering funds to charity...\n"); sleep(3);
                            deleteAccount(account);
                            printf("Successfully deleted account!\n");
                            break;
                        }
                        continue;
                    }
                    switch(option){
                        case 1: printAccountInfo(account);break; //print info
                        case 2://add funds
                           addFunds(&account);
                           break;
                        case 3: //transfer funds
                            printf("Select which account to transfer funds to:\n"); 
                            printf("\t1.Checkings\n\tSavings\n");
                            printf("Select option (1 or 2): ");
                            scanf("%d", &option);
                            switch(option){
                                case 1: break;
                                case 2: break;
                            }
                            break;
                        case 4: 
                            getchar(); 
                            changePassword(account); 
                            printf("Changing password...\n"); sleep(3);
                            printf("Password successfully changed!\n");
                            break; //change password
                        case 5: 
                            getchar();
                            updateAccountOwner(&account); 
                            printf("Changing account owner...\n"); sleep(3);
                            printf("Account owner successfully changed!\n");
                            break;
                    }
                }
                break;
            case 3: printAccounts(); break;
            case 4:
                printf("Saving session...\n");
                sleep(3);
                printf("Thank you for using Bank Simulator 2021!\n"); return 0;
        }
    }
}