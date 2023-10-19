/**
  @file account.c
  @author Shlok Dave (ssdave)
  This file acts as the source file for the account component. It is responsible
  for reading and writing the account file. It is also responsible for reading and maintaining
  the list of account names. The component contains four main functions with two
  constants that are both defined. As the source file, the file contains the functions
  and their implementations.
 */

#include "account.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"

#include <limits.h>

/** Number of accounts supported by the program. */
#define ACCOUNT_LIMIT 100000

/** Global variable declared to keep an array of strings. */
char accountNames[ACCOUNT_LIMIT][NAME_LIMIT + 1];

/** Global variable declared to keep an array of unsigned long integers. */
unsigned long accountBalances[ACCOUNT_LIMIT];

/** Counter to know how many numerical accounts are entered. */
int numOfAccounts = 0;

/**
  This function is responsible for reading an amount of currency
  from the given stream, storing it in an unsigned value. This
  unsigned value is pointed by val and it is returned true if the
  input contains an amount of currency in a valid format for an
  account transaction file and can be represented as cents. This
  is the defintiion of the function, which includes the implementation.
  @param fp pointer to the file for the currency value to be read.
  @param val pointer to an unsigned long variable for the read value.
  @return true if input contains an amount of currency in a valid format
  for an account to be represented as cents in an unsigned long.
*/
bool readCurrencyFromString(char *str, unsigned long *val)
{
  char initChar;
  unsigned long initValue = 0;
  bool checkNextDec = false;
  unsigned multDec = 100;
  int index = 0;

  // Go through each character until end
  while ((initChar = str[index++]) != '\0')
  {
    // Confirm that a decimal is present
    if (initChar == '.')
    {
      if (checkNextDec)
        return false;

      checkNextDec = true;
      continue;
    }

    // Return false if the character is not a digit
    if (!isdigit(initChar))
      return false;

    unsigned d = initChar - '0';

    // Checks for any overflow
    if (!checkNextDec && initValue > (ULONG_MAX - d) / 10)
      return false;
    initValue = initValue * 10 + d;

    // Reduces the multiplier if there is a decimal present
    if (checkNextDec)
    {
      multDec /= 10;
    }
  }

  // This check is for no decimal point in the string
  if (!checkNextDec)
  {
    // Another overflow check with the multiplication
    if (!checkMul(initValue, 100))
      return false;
    initValue *= 100;
  }

  // Format invalid if the decimal point is seen but mulitplier = 1
  else if (multDec != 1)
  {
    return false;
  }

  initValue /= multDec;

  *val = initValue;
  return true;
}

/**
  This function returns a pointer to the balance value for an account with
  the given name. If there is no account, it returns NULL. This specific function
  can be used by the trasnaction to get the balance value for an account and
  adjust it based on the current transaction. This specific function is the definition
  of the function, which includes the implementation.
  @param name character array which contains the name of the account.
  @return a pointer to the account balance, if account is not found, returns NULL.
*/
unsigned long *lookupAccount(char const name[NAME_LIMIT + 1])
{
  // Loop initalized to go through all the accounts
  for (int idx = 0; idx < ACCOUNT_LIMIT; idx++)
  {
    // Compares the list with the name
    if (strcmp(accountNames[idx], name) == 0)
    {
      return &accountBalances[idx];
    }
  }
  return NULL;
}

/**
  This function loads all the accounts from the file withthe given name. It detects
  errors in the filename or any of the file contents. This specific function is the
  definition of the function, which includes the implementation.
  @param fname character array that is represented from a file and is able to be stored.
*/
void loadAccounts(char fname[AFILE_LIMIT + 1])
{
  // Buffer defined
  char storeAccount[AFILE_LIMIT + 1];
  int newVersion;

  // File name is parsed to help get word and version of the file
  if (sscanf(fname, "%30[^-]-%d.txt", storeAccount, &newVersion) != 2)
  {
    fprintf(stderr, "Invalid account file name: %s\n", fname);
    exit(EXIT_FAILURE);
  }

  char fileArrayTemp[AFILE_LIMIT + 1];
  int updatedVersion = newVersion + 1;
  int newSize = snprintf(fileArrayTemp, sizeof(fileArrayTemp), "%s-%d.txt", storeAccount, updatedVersion);

  // Checks for the new file name with the limit constraint
  if (newSize >= AFILE_LIMIT)
  {
    fprintf(stderr, "Invalid account file name: %s\n", fname);
    exit(EXIT_FAILURE);
  }

  // Opens up the file to load the accounts
  FILE *newFilePointer = fopen(fname, "r");
  if (!newFilePointer)
  {
    fprintf(stderr, "Can't open account file: %s\n", fname);
    exit(EXIT_FAILURE);
  }

  // Accounts are set to 0
  numOfAccounts = 0;

  char tempAccountBalance[50];
  char tempAccountName[NAME_LIMIT + 1];

  // Loop to help read the account name and the balance
  while (fscanf(newFilePointer, "%30s %49s", tempAccountName, tempAccountBalance) != EOF &&
         numOfAccounts < ACCOUNT_LIMIT)
  {
    // Simple check to see if the account name is invalid
    if (strlen(tempAccountName) > NAME_LIMIT)
    {
      fprintf(stderr, "Account what name is too long in file: %s\n", fname);
      fclose(newFilePointer);
      exit(EXIT_FAILURE);
    }

    // Checks if any whitespace occurs in the name
    if (strcspn(tempAccountName, " \t\n\r\f\v") != strlen(tempAccountName))
    {
      fprintf(stderr, "Invalid account file\n");
      fclose(newFilePointer);
      exit(1);
    }

    unsigned long newUnsignedLongValue;

    // The balance is converted from a string to an unsigned long
    if (!readCurrencyFromString(tempAccountBalance, &newUnsignedLongValue))
    {
      fprintf(stderr, "Invalid account file\n");
      fclose(newFilePointer);
      exit(1);
    }

    // Store all of the account infomration into the arrays defined
    strcpy(accountNames[numOfAccounts], tempAccountName);
    accountBalances[numOfAccounts] = newUnsignedLongValue;
    numOfAccounts++;
  }

  fclose(newFilePointer);
}

/**
  This fuction is responsible for writing out the updated balances of all accounts. It
  specifically writes to the next version of the given account. This specific function
  is the declaration of the function, which is part of the header file.
  @param fname character array that is represented from a file to save the accounts.
*/
void saveAccounts(char fname[AFILE_LIMIT + 1])
{
  // Buffer defined
  char storeAccount[AFILE_LIMIT + 1];
  int version;

  // Gets the account name and the version number from the file
  if (sscanf(fname, "%30[^-]-%d.txt", storeAccount, &version) != 2 || strlen(fname) > AFILE_LIMIT)
  {
    fprintf(stderr, "Invalid account file name: %s\n", fname);
    exit(EXIT_FAILURE);
  }

  version++;
  char newFname[AFILE_LIMIT + 1];
  int bytesWritten = snprintf(newFname, sizeof(newFname), "%s-%d.txt", storeAccount, version);

  // Check for the filename, if it exceeds the buffer size
  if (bytesWritten >= sizeof(newFname) || bytesWritten < 0)
  {
    fprintf(stderr, "Stepped version of account file name is too long: %s-%d.txt\n", storeAccount, version);
    exit(EXIT_FAILURE);
  }

  // Opens the file to be able to save it
  FILE *file = fopen(newFname, "w");
  if (!file)
  {
    perror("Unable to open file for writing");
    exit(EXIT_FAILURE);
  }

  const int MAX_ALLOWED_NAME_WIDTH = 25;

  // Determine the length of the account name that is the longest
  int maxNameLength = 0;
  for (int idx = 0; idx < numOfAccounts; idx++)
  {
    int len = strlen(accountNames[idx]);
    if (len > maxNameLength)
    {
      maxNameLength = len;
    }
  }

  // Saves and writes the account name to the new file, while handling two specific cases
  for (int idx = 0; idx < numOfAccounts; idx++)
  {
    unsigned long dollars = accountBalances[idx] / 100;
    unsigned long cents = accountBalances[idx] % 100;

    // Writes the information of the account to the file
    if (strlen(accountNames[idx]) > MAX_ALLOWED_NAME_WIDTH)
    {
      fprintf(file, "%s %lu.%02lu\n", accountNames[idx], dollars, cents);
    }
    else
    {
      fprintf(file, "%-36s %12lu.%02lu\n", accountNames[idx], dollars, cents);
    }
  }
  fclose(file);
}