/**
   @file date.c
   @author Shlok Dave (ssdave)
   This file acts as the component for providing the necessary
   functions to read the birthdate field and converting it to the
   necessary standard format. Unlike the header file, this file
   contains the actual code that the main program uses in producing
   the correct output.
 */

#include "date.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** Exit status for a missing or badly formatted date. */
#define DATE_ERROR 102

/** Number of digits in the short (2 digit) y. */
#define SHORT_YEAR 2

/** Number of digits in the full (4 digit) y. */
#define FULL_YEAR 4

/** Number of digits in the m. */
#define MONTH_DIGITS 2

/** Number of digits in the d. */
#define DAY_DIGITS 2

/**
  Helper method that is designed for helping to check the string if
  there are any digits in it.
  @param checkedString the string that the input is checking on.
  @param s the size of the string that is being checked.
  @return 1 if the string has only digits in it.
*/
static int checkStringForDigits(const char *checkedString, int s)
{
  // Loops through to check for a digit.
  for (int i = 0; i < s; i++)
  {
    if (!isdigit(checkedString[i]))
    {
      // Returns 0 with no digits.
      return 0;
    }
  }

  // Returns 1 with all characters being digits.
  return 1;
}

/**
  This is the function that reads the content of a date field from the standard
  input. The function exits with an exit status of 101 if the date is missing
  or too long. In all, the primary function of the function is to be able
  to read the date.
  @param date the date that is stored in the array from the input.
*/
void read_date(char date[FIELD_MAX + 1])
{
  // Date is being read from the standard input.
  if (fgets(date, FIELD_MAX + 1, stdin) == NULL || strlen(date) == 0)
  {
    exit(DATE_ERROR);
  }

  size_t sizeOfString = strlen(date);
  if (sizeOfString > 0 && date[sizeOfString - 1] == '\n')
  {
    date[sizeOfString - 1] = '\0';
  }

  // This is to check if the date is too long.
  if (strlen(date) >= FIELD_MAX && date[FIELD_MAX - 1] != '\0')
  {
    exit(DATE_ERROR);
  }
}
/**
  Function that converts the string in date to the standard format. The
  function exits with an exit status of 102 if the given string is not in a format
  that is acceptable. The function converts the string to be able to be read in its
  proper format.
  @param date the date that is being converted to the standard format.
*/
void fix_date(char date[FIELD_MAX + 1])
{
  // These variables are declared to store the components of a birthdate.
  char d[DAY_DIGITS + 1] = {0};
  char m[MONTH_DIGITS + 1] = {0};
  char y[FULL_YEAR + 1] = {0};
  char *splitOne = strpbrk(date, "/-");

  if (!splitOne)
  {
    exit(DATE_ERROR);
  }

  char *splitSecond = strpbrk(splitOne + 1, "/-");
  if (!splitSecond)
  {

    exit(DATE_ERROR);
  }

  // This helps extract the day, month, and the year based off of the input.
  if ((splitOne - date) == MONTH_DIGITS && (splitSecond - splitOne - 1) == DAY_DIGITS)
  {
    strncpy(m, date, MONTH_DIGITS);
    strncpy(d, splitOne + 1, DAY_DIGITS);
    strcpy(y, splitSecond + 1);
  }

  // This is another situation with one digit of a month or day.
  else if ((splitOne - date) == DAY_DIGITS && (splitSecond - splitOne - 1) == MONTH_DIGITS)
  {
    strncpy(d, date, DAY_DIGITS);
    strncpy(m, splitOne + 1, MONTH_DIGITS);
    strcpy(y, splitSecond + 1);
  }

  else if ((splitOne - date) == FULL_YEAR && (splitSecond - splitOne - 1) == MONTH_DIGITS)
  {
    strncpy(y, date, FULL_YEAR);
    strncpy(m, splitOne + 1, MONTH_DIGITS);
    strncpy(d, splitSecond + 1, DAY_DIGITS);
  }
  else
  {
    // Standard error thrown if incorrect date format is handled.
    exit(DATE_ERROR);
  }

  if (strlen(m) == 1)
  {
    char newMon[4];
    snprintf(newMon, sizeof(newMon), "0%s", m);
    strcpy(m, newMon);
  }

  if (strlen(d) == 1)
  {
    char newD[4];
    snprintf(newD, sizeof(newD), "0%s", d);
    strcpy(d, newD);
  }

  char newY[FULL_YEAR + 1 + 1] = {0};

  // Handles the conversion of two digit years.
  if (strlen(y) == SHORT_YEAR)
  {
    if (atoi(y) >= 24)
    {
      snprintf(newY, FULL_YEAR + 1, "19%.2s", y);
    }
    else
    {
      snprintf(newY, FULL_YEAR + 1, "20%.2s", y);
    }
    strcpy(y, newY);
  }
  else if (strlen(y) != FULL_YEAR)
  {
    exit(DATE_ERROR);
  }

  // This checks to see if the day, month, and year are all numbers.
  if (!checkStringForDigits(d, DAY_DIGITS) ||
      !checkStringForDigits(m, MONTH_DIGITS) ||
      !checkStringForDigits(y, FULL_YEAR))
  {
    exit(DATE_ERROR);
  }

  // Finalizes the correct date format.
  snprintf(date, FIELD_MAX + 1, "%s-%s-%s", y, m, d);
}
