/**
   @file ssn.c
   @author Shlok Dave (ssdave)
   This file acts as the source file for the header file of
   ssn. This specific compontent provides the functions for reading
   the social security number and converting it into the proper
   standard format. Unlike the header file, this file contains not only
   the function declarations, but has the source code for the functions.
 */

#include "ssn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** Exit status for a missing or badly formatOfSsn ssn. */
#define SSN_ERROR 103

/** Number of digits in the first group in the SSN. */
#define START_DIGITS 3

/** Number of digits in the middle group in the SSN. */
#define MIDDLE_DIGITS 2

/** Number of digits in the last group in the SSN. */
#define END_DIGITS 4

/**
  Helper method that checks the social security number if it
  contains a digit.
  @param check the character that is being checked from the ssn.
  @return 1 if the character contains a digit in the ssn, otherwise
  it will return 0.
*/
int checkForDigit(char check)
{
  return check >= '0' && check <= '9';
}

/**
  Calls the function that reads the content of the SSN field from standard
  input and stores it in the given string. The function exits with an exit
  status of 103 if the social security number is missing, or if the social
  security number is too long.
  @param ssn the ssn that is being read in and checked to see if it fits the format.
*/
void read_ssn(char ssn[FIELD_MAX + 1])
{
  // SSN is read.
  if (!fgets(ssn, FIELD_MAX + 1, stdin))
  {
    exit(SSN_ERROR);
  }

  // Deletes the leading line.
  size_t sizeOfSsn = strlen(ssn);
  if (sizeOfSsn > 0 && ssn[sizeOfSsn - 1] == '\n')
  {
    ssn[--sizeOfSsn] = '\0';
  }

  // Trims the whitespace.
  char *beginningofSsn = ssn;
  while (*beginningofSsn && isspace(*beginningofSsn))
  {
    beginningofSsn++;
    sizeOfSsn--;
  }

  while (sizeOfSsn > 0 && isspace(ssn[sizeOfSsn - 1]))
  {
    ssn[sizeOfSsn - 1] = '\0';
    sizeOfSsn--;
  }

  // String is shifted with changes.
  if (beginningofSsn != ssn)
  {
    for (size_t i = 0; i <= sizeOfSsn; i++)
    {
      ssn[i] = beginningofSsn[i];
    }
  }

  // Error checking.
  if (sizeOfSsn == 0 || sizeOfSsn > (START_DIGITS + MIDDLE_DIGITS + END_DIGITS + 2))
  {
    exit(SSN_ERROR);
  }
}

/**
  Calls the function that converts the string of the social security number to
  the standard format. The function exits with an exit status of 103 if the string
  of the ssn is not in an acceptable format for a necessary ssn.
  @param ssn the ssn that is being fixed to standard format.
*/
void fix_ssn(char ssn[FIELD_MAX + 1])
{
  // Error Checking.
  if (!ssn)
  {
    exit(SSN_ERROR);
  }

  size_t sizeOfSsn = strlen(ssn);

  // No SSN check.
  if (strcmp(ssn, "N/A") == 0)
  {
    strcpy(ssn, SSN_NA);
    return;
  }

  // Checks for hyphens.
  if (sizeOfSsn == (START_DIGITS + MIDDLE_DIGITS + END_DIGITS))
  {
    for (int i = 0; i < sizeOfSsn; i++)
    {
      if (!checkForDigit(ssn[i]))
      {
        exit(SSN_ERROR);
      }
    }

    // Reformats the SSN.
    char formatOfSsn[START_DIGITS + 1 + MIDDLE_DIGITS + 1 + END_DIGITS + 1];
    snprintf(formatOfSsn, sizeof(formatOfSsn), "%.*s-%.*s-%.*s",
             START_DIGITS, ssn,
             MIDDLE_DIGITS, ssn + START_DIGITS,
             END_DIGITS, ssn + START_DIGITS + MIDDLE_DIGITS);
    strncpy(ssn, formatOfSsn, FIELD_MAX);
    ssn[FIELD_MAX] = '\0';
  }

  else if (sizeOfSsn == (START_DIGITS + MIDDLE_DIGITS + END_DIGITS + 2))
  {
    bool hasDashes = ssn[START_DIGITS] == '-' && ssn[START_DIGITS + 1 + MIDDLE_DIGITS] == '-';
    if (!hasDashes)
    {
      exit(SSN_ERROR);
    }

    // Error Checking.
    for (int i = 0; i < START_DIGITS; i++)
    {
      if (!checkForDigit(ssn[i]))
      {
        exit(SSN_ERROR);
      }
    }

    // Error Checking.
    for (int i = START_DIGITS + 1; i < START_DIGITS + 1 + MIDDLE_DIGITS; i++)
    {
      if (!checkForDigit(ssn[i]))
      {
        exit(SSN_ERROR);
      }
    }

    // Error Checking.
    for (int i = START_DIGITS + 1 + MIDDLE_DIGITS + 1; i < sizeOfSsn; i++)
    {
      if (!checkForDigit(ssn[i]))
      {
        exit(SSN_ERROR);
      }
    }
  }
  else
  {
    exit(SSN_ERROR);
  }
}
