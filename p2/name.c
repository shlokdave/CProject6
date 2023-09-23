/**
   @file name.c
   @author Shlok Dave (ssdave)
   This file acts as the name component for providing functions for reading
   the name field and converting it to the standard format. Unlike the header file,
   this file not only contains the functions, it also includes the necessary source
   code for the functions. There is one helper function that is added to help capitalize
   the names for the input.
 */

#include "name.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** Exit status for a missing or badly formatted name. */
#define NAME_ERROR 101

/**
  Helper method that is designed to capitalize the first word that is
  read in the file. This function helps check that the first letter is
  an actual character. After it capitalizes the first character, the
  function uses a while loop to convert the rest of the strings to
  lowercase. Thus, only capitalizing the first character in the string.
  @param word the pointer to the string that represents the input as a name.
*/
void capital(char *word)
{
  // Checks to see if the word has anything.
  if (*word)
  {
    // Converted to uppercase.
    *word = toupper(*word);
    word++;
    while (*word)
    {
      *word = tolower(*word);
      word++;
    }
  }
}

/**
   Calls the function that reads the contents of a name field from the standard
   input. After it reads the input, the function stores it in a given string. The
   funciton returns false if the end of the file is reached and there is no name
   field. The function exits with an exit status of 101 if the name is missing
   or too long.
   @param name char array for where the name of the string will be stored.
   @return true if the name from the string was able to be read or is present.
*/
bool read_name(char name[FIELD_MAX + 1])
{
  // A buffer is developed to help check for big inputs.
  char checkSize[3 * FIELD_MAX + 3];

  // This is where the input is read.
  if (fgets(checkSize, sizeof(checkSize), stdin) == NULL)
  {
    return false;
  }

  char *symbol = strtok(checkSize, ":");
  if (symbol == NULL)
  {
    return false;
  }

  strncpy(name, symbol, FIELD_MAX);
  name[FIELD_MAX] = '\0';

  // This checks for string limits.
  size_t sizeOfName = strlen(name);
  if (sizeOfName > 0 && name[sizeOfName - 1] == '\n')
  {
    name[sizeOfName - 1] = '\0';
  }

  // This is more error checking.
  if (sizeOfName == FIELD_MAX && name[FIELD_MAX - 1] != '\0')
  {
    exit(NAME_ERROR);
  }

  return true;
}

/**
  Calls the function that converts the string in name to the standard format. It
  exits with an exit status of 101 if the string isn't able to be read as a
  name. In all, the function just converts the string to its proper format.
  @param name the array that contains the name needing to be fixed.
*/
void fix_name(char name[FIELD_MAX + 1])
{
  // These are buffers that are created to store different parts of the name.
  char f[FIELD_MAX + 1] = {0};
  char l[FIELD_MAX + 1] = {0};
  char b[2 * FIELD_MAX + 3] = {0};

  char *checkForComma = strchr(name, ',');

  // Checks for a comma in the name.
  if (checkForComma)
  {
    *checkForComma = '\0';
    strcpy(l, name);
    strcpy(f, checkForComma + 2);
  }
  else
  {
    // Name is split using a delimeter.
    char *space = strchr(name, ' ');
    if (!space)
    {
      // This is Error checking.
      exit(NAME_ERROR);
    }
    *space = '\0';
    strcpy(f, name);
    strcpy(l, space + 1);
  }

  // This checks to see if the name has any symbols not valid.
  for (char *charError = l; *charError; charError++)
  {
    if (!isalpha(*charError) && *charError != ' ' && *charError != '-' && *charError != '\'')
    {
      exit(NAME_ERROR);
    }
  }
  for (char *charError = f; *charError; charError++)
  {
    if (!isalpha(*charError) && *charError != ' ' && *charError != '-' && *charError != '\'')
    {
      exit(NAME_ERROR);
    }
  }

  // This capitalizes the first and last name to the correct format.
  capital(f);
  capital(l);

  if (strlen(f) + strlen(l) + 2 > FIELD_MAX)
  {
    exit(NAME_ERROR);
  }

  // This is the last part to format the name.
  snprintf(b, sizeof(b), "%s, %s", l, f);
  strncpy(name, b, FIELD_MAX);
  name[FIELD_MAX] = '\0';
}