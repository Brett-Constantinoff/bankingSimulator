# bankingSimulator
This application is a basic simulation of an online banking system. The bankManager module will allow users to create a bank account and set up the accounts initial information. This includes account owner, account password, and initial funds of the account. Editing account information, printing out current accounts, printing account information, adding funds to the account, transferring funds from one account to the other and deleting an account are also included in the module's functionality. Note that account passwords are not protected in any way beyond simple password verifications. Also, in order to view created accounts, the file "bankAccounts" must be present within current directory as the client executable is in. 

In order to run the simulator, simply open your terminal and navigate to your directory holding the files and type "make client". This will produce an executable called "client", then type "./client" and enjoy creating a bank account with as much money as you can dream of!
