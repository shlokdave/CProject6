/**
  @file account.h
  @author Shlok Dave (ssdave)
  This file acts as the header file for the account component. It is responsible
  for reading and writing the account file. It is also responsible for reading and maintaining
  the list of account names. The component contains four main functions with two
  constants that are both defined. As the header file, these functions are only
  declared and not implemented.
 */

#include <stdbool.h>
#include <stdio.h>

/** Maximum length of an account name. */
#define NAME_LIMIT 30

/** Limit on the length of an account file. */
#define AFILE_LIMIT 30

/**
  This function is responsible for reading an amount of currency
  from the given stream, storing it in an unsigned value. This
  unsigned value is pointed by val and it is returned true if the
  input contains an amount of currency in a valid format for an
  account transaction file and can be represented as cents. This
  is the declaration of the function, which is part of the header file.
  @param fp pointer to the file for the currency value to be read.
  @param val pointer to an unsigned long variable for the read value.
  @return true if input contains an amount of currency in a valid format
  for an account to be represented as cents in an unsigned long.
*/
bool readCurrency(FILE *fp, unsigned long *val);

/**
  This function returns a pointer to the balance value for an account with
  the given name. If there is no account, it returns NULL. This specific function
  can be used by the trasnaction to get the balance value for an account and
  adjust it based on the current transaction. This specific function is the declaration
  of the function, which is part of the header file.
  @param name character array which contains the name of the account.
  @return a pointer to the account balance, if account is not found, returns NULL.
*/
unsigned long *lookupAccount(char const name[NAME_LIMIT + 1]);

/**
  This function loads all the accounts from the file withthe given name. It detects
  errors in the filename or any of the file contents. This specific function is the
  declaration of the function, which is part of the header file.
  @param fname character array that is represented from a file and is able to be stored.
*/
void loadAccounts(char fname[AFILE_LIMIT + 1]);

/**
  This fuction is responsible for writing out the updated balances of all accounts. It
  specifically writes to the next version of the given account. It is responsible for saving
  the accounts. This specific function is the declaration of the function, which is
  part of the header file.
  @param fname character array that is represented from a file to save the accounts.
*/
void saveAccounts(char fname[AFILE_LIMIT + 1]);