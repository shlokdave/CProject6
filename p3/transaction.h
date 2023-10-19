/**
  @file transaction.h
  @author Shlok Dave (ssdave)
  This file acts as the header file for the transaction.c file. The
  file contains the declaration of the one function that is used in
  the transaction.c file. This component in general is responsible for
  reading and processing all the transactions in the transaction file.
  As the header file, only the declaration of the function is present.
 */
#include "account.h"
/**
  This function is responsible for reading and performing all the transactions
  that are necessary in the transaction file with the given name. It also
  uses the account component to update the account balances and detect any
  errors that the transaction file processes. This is only the declaration of
  the function.
  @param fname the name of the file that the transaction is being processed in. This
  file would contain all the necessary transactions.
*/
void processTransactions(char fname[AFILE_LIMIT + 1]);