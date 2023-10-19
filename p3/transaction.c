/**
  @file transaction.c
  @author Shlok Dave (ssdave)
  This file acts as the source file for the transaction component. The
  file contains the definition of the one function that was declared
  in the header file. This component in general is responsible for
  reading and processing all the transactions in the transaction file.
  As the source file, the implementations are provided.
 */

#include "transaction.h"
#include "account.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include "limits.h"

/**
  This function is responsible for acting as a helper function to help with the
  process of subtracting. It helps subtract an amount from the given balance. The
  original balance is returned if the subtraction cannot be peformed for any reason.
  A reason could include that the subtraction would lead to a negative result.
  @param balance the balance of the current account that is going to be subtracted from.
  @param amount the amount that is needed to be subtracted from the balance.
 */
static unsigned long helperForSubtraction(unsigned long accountBalance, unsigned long subAmount)
{
  if (checkSub(accountBalance, subAmount))
  {
    return accountBalance - subAmount;
  }
  return accountBalance;
}

/**
  This function is responsible for acting as a helper function to help with the
  process of adding. It helps add an amount from the given balance. The
  original balance is returned if the addition cannot be peformed for any reason.
  A reason could include that the subtraction would lead to an overflow.
  @param balance the balance of the current account that is going to be added to.
  @param amount the amount that is needed to be added to the balance.
 */
static unsigned long helperForAddition(unsigned long balance, unsigned long amount)
{
  if (checkAdd(balance, amount))
  {
    return balance + amount;
  }
  return balance;
}

/**
  This function is responsible for acting as a helper function to help convert
  the price as a double into separate dollar and cent values. This function helps
  with the conversion and the overall transaction process. Using this helper method,
  allows for the processTransaction to be able to just call this when needed.
  @param price the price that the function is converting from.
  @param dollars a pointer to the dollar component from the price, displays the dollars.
  @param cents a pointer to the cents component from the price, displays the cents.
 */
static void helperForConversion(double conversionPrice, unsigned long long *conversionDollars, unsigned long long *conversionCents)
{
  *conversionDollars = (unsigned long long)conversionPrice;
  *conversionCents = (unsigned long long)((conversionPrice - *conversionDollars) * 100 + 0.5);
}

/**
  This function is responsible for reading and performing all the transactions
  that are necessary in the transaction file with the given name. It also
  uses the account component to update the account balances and detect any
  errors that the transaction file processes. This is the definition of the function,
  which includes the full implementation of the function.
  @param fname the name of the file that the transaction is being processed in. This
  file would contain all the necessary transactions.
*/
void processTransactions(char fname[AFILE_LIMIT + 1])
{
  // Open the file to process transactions
  FILE *filePointer = fopen(fname, "r");

  // Checks to see if the file can be opened or not
  if (!filePointer)
  {
    fprintf(stderr, "Can't open account file: %s\n", fname);
    exit(EXIT_FAILURE);
  }

  char storeName[NAME_LIMIT + 1];
  char storeOperand[5];
  int storeQuantity;
  double storePrice;

  // Reads the file until the end of file, it gets the name, operand, quantity, and price for transaction
  while (fscanf(filePointer, "%30s %4s %d %lf", storeName, storeOperand, &storeQuantity, &storePrice) != EOF)
  {
    unsigned long long dollars, cents;
    helperForConversion(storePrice, &dollars, &cents);

    // Checks for any overflow when converting the transaction
    if (!checkMul(dollars * 100 + cents, storeQuantity))
    {
      fprintf(stderr, "Transaction overflow error\n");
      exit(1);
    }
    unsigned long long storeTotal = (dollars * 100 + cents) * storeQuantity;
    unsigned long long *storeBalance = (unsigned long long *)lookupAccount(storeName);

    // Checks if the balance is valid or not
    if (!storeBalance)
    {
      fprintf(stderr, "Invalid transaction file\n");
      exit(1);
    }

    // Checks if the balance is longer than the unsigned long max
    if (*storeBalance > ULONG_MAX)
    {
      fprintf(stderr, "Invalid account file\n");
      exit(1);
    }

    // Transaction is processed for "buy" operand
    if (strcmp(storeOperand, "buy") == 0)
    {
      if (*storeBalance < storeTotal)
      {
        fprintf(stderr, "Account overflow\n");
        exit(1);
      }
      // Subtracts specific transaction amount
      *storeBalance = helperForSubtraction(*storeBalance, storeTotal);
    }

    // Transaction is processed for "sell" operand
    else if (strcmp(storeOperand, "sell") == 0)
    {
      if (*storeBalance == 0)
      {
        fprintf(stderr, "Account overflow\n");
        exit(1);
      }
      if (!checkAdd(*storeBalance, storeTotal))
      {
        fprintf(stderr, "Account overflow\n");
        exit(1);
      }
      // Adds specific transaction amount
      *storeBalance = helperForAddition(*storeBalance, storeTotal);
    }
  }
  fclose(filePointer);
}