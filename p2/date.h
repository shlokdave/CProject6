/**
  @file date.h
  @author Shlok Dave (ssdave)
  This file acts as the header file for the date.c file. This file
  contains the declarations of variables and functions that are utilized
  in the other source files. With this header file, these declarations are
  directly used in the date.c file. It contains the read_date() method and the
  fix_date() method.
 */

#include "util.h"
#include <stdbool.h>

/**
  Calls the function that reads the content of a date field from the standard
  input. The function exits with an exit status of 101 if the date is missing
  or too long. In all, the primary function of the function is to be able
  to read the date.
  @param date the date that is stored in the array from the input.
*/
void read_date(char date[FIELD_MAX + 1]);

/**
  Calls the function that converts the string in date to the standard format. The
  function exits with an exit status of 102 if the given string is not in a format
  that is acceptable. The function converts the string to be able to be read in its
  proper format.
  @param date the date that is being converted to the standard format.
*/
void fix_date(char date[FIELD_MAX + 1]);