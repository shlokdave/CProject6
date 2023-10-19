/**
  @file trader.c
  @author Shlok Dave (ssdave)
  This file contains the main function. The function is responsible
  for parsing the command-line arguments and using other components to
  be able to effectively read the account file. It is also responsible for
  processing all the transactions and writing it to the final account file
  at the end of the entire process.
 */

#include <stdlib.h>
#include <stdio.h>
#include "account.h"
#include "transaction.h"

/** Index of the account file command-line argument. */
#define ACCOUNT_ARG 1

/** Index of the transacton file command-line argument. */
#define TRANSACT_ARG 2

/**
  This is the main part of the program and it is where the program
  is run from. This function contains the actions to call the functions
  from other components. It is responsbile for printing the correct transactions
  after the processes are complete within each of the other functions. In all, the
  main function acts as the overall "brain" of the entire program.
*/
int main(int argc, char *argv[])
{
  if (argc < 3 || argc != 3)
  {
    fprintf(stderr, "usage: trader <account-file> <transaction-file>\n");
    exit(1);
  }

  loadAccounts(argv[ACCOUNT_ARG]);
  processTransactions(argv[TRANSACT_ARG]);
  saveAccounts(argv[ACCOUNT_ARG]);

  return 0;
}