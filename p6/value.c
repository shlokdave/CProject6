/**
    @file value.c
    @author Shlok Dave (ssdave)
    Implementation for the value component, with support for integer
    and (eventually) string values.
  */

#include "value.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

//////////////////////////////////////////////////////////
// Integer implementation.

// print method for Integer.
static void printInteger(Value const *v)
{
  // Print the integer inside this value.
  printf("%d", v->ival);
}

// move method for Integer.
static void moveInteger(Value const *src, Value *dest)
{
  // We just need to copy the value.
  dest->ival = src->ival;

  dest->print = src->print;
  dest->move = src->move;
  dest->equals = src->equals;
  dest->hash = src->hash;
  dest->empty = src->empty;
}

// equals method for Integer.
static bool equalsInteger(Value const *v, Value const *other)
{
  // Make sure the other object is also an Integer.
  // (i.e., it uses the same print funtion)
  if (other->print != printInteger)
    return false;

  return v->ival == other->ival;
}

// hash method for Integer.
static unsigned int hashInteger(Value const *v)
{
  // This will overflow negative values to large positive ones.
  return v->ival;
}

// Free memory used inside this integer Value.
static void emptyInteger(Value *v)
{
  // An int vaue doesn't need any additional memory.
}

int parseInteger(Value *v, char const *str)
{
  // Try to parse an integer from str.
  int val, len;
  if (sscanf(str, "%d%n", &val, &len) != 1)
    return 0;

  // Fill in all the fields of v for an integer type of value.
  v->print = printInteger;
  v->move = moveInteger;
  v->equals = equalsInteger;
  v->hash = hashInteger;
  v->empty = emptyInteger;
  v->ival = val;

  // Return how much of str we parsed.
  return len;
}

// Print method for String.
static void printString(Value const *v)
{
  // Print the string inside this value.
  printf("\"%s\"", (char *)v->vptr);
}

// Move method for String.
static void moveString(Value const *src, Value *dest)
{
  // String values are just copied.
  dest->vptr = src->vptr;

  // Copy function pointers.
  dest->print = src->print;
  dest->move = src->move;
  dest->equals = src->equals;
  dest->hash = src->hash;
  dest->empty = src->empty;
}

// Equals method for String.
static bool equalsString(Value const *v, Value const *other)
{
  // Null pointer check.
  if (v->vptr == NULL || other->vptr == NULL)
  {
    // If both are null they are equal.
    return v->vptr == other->vptr;
  }

  // Strings are compared with each other.
  return strcmp((char *)v->vptr, (char *)other->vptr) == 0;
}

// Hash method for String.
static unsigned int hashString(Value const *v)
{
  unsigned int hash = 0;

  // Check for value pointer null.
  if (v && v->vptr)
  {
    const char *newString = (char *)v->vptr;

    // Iterates through each character of string.
    while (*newString)
    {
      // Casting unsigned char for proper handling.
      hash += (unsigned char)(*newString++);
      hash += hash << 10;
      hash ^= hash >> 6;
    }

    // Final mixing of hash for even distribution.
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
  }
  return hash;
}

// Empty method for String.
static void emptyString(Value *v)
{
  // Check if v pointer is null.
  if (v != NULL && v->vptr != NULL)
  {
    // Free the memory of the vptr member.
    free(v->vptr);
    v->vptr = NULL;
  }
}

/**
    Function that parses a quoted string from the input string. It initializes a Value structure
    to hold the parsed string. The function scans the input for a string that is in double
    quotes. This function ensures that the parsed string is stored within the Value structure and
    is properly null-terminated.
    @param v pointer to the value that the instance will hold the parsed string.
    @param str string from which to parse the string value.
    @return number of characters processed from the input string, or zero if unsuccessful.
*/
int parseString(Value *v, char const *str)
{

  // Position in the string.
  const char *posString = str;

  // Check for any whitespace.
  while (isspace((unsigned char)*posString))
  {
    posString++;
  }

  // Parsing the String here.
  char newBuff[BUFFER_SIZE];
  int count = 0;
  if (sscanf(posString, "\"%1023[^\"]\"%n", newBuff, &count) != 1)
  {
    return 0;
  }

  // Copy the new string for memory allocation.
  char *copyNewString = malloc(strlen(newBuff) + 1);
  strcpy(copyNewString, newBuff);
  v->vptr = copyNewString;

  // Value struct copying process.
  v->vptr = copyNewString;
  v->print = printString;
  v->move = moveString;
  v->equals = equalsString;
  v->hash = hashString;
  v->empty = emptyString;

  // Returns the characters processed.
  return posString - str + count;
}