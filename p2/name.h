/**
  @file name.h
  @author Shlok Dave (ssdave)
  This file acts as the header file for the name.c file. This file contains
  the declarations for the functions and variables that are used in the 
  main name.c file. These declarations are directly accessed into the other
  file. 
 */

#include "util.h"
#include <stdbool.h>

/**
   Calls the function that reads the contents of a name field from the standard
   input. After it reads the input, the function stores it in a given string. The
   funciton returns false if the end of the file is reached and there is no name
   field. The function exits with an exit status of 101 if the name is missing 
   or too long.
   @param name char array for where the name of the string will be stored.
   @return true if the name from the string was able to be read or is present.
*/
bool read_name( char name[ FIELD_MAX + 1 ] )

/**
  Calls the function that converts the string in name to the standard format. It
  exits with an exit status of 101 if the string isn't able to be read as a
  name. In all, the function just converts the string to its proper format.
  @param name the array that contains the name needing to be fixed.
*/
void fix_name( char name[ FIELD_MAX + 1 ] )