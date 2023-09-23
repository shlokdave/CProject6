/**
  @file ssn.h
  @author Shlok Dave (ssdave)
  This is the header file for the ssn.c file. This header file is
  responsible for calling the necessary functions from the ssn.c
  file. The component provides funcntions for reading the social
  security number and converting it to the proper standard
  format.
 */

#include "util.h"
#include <stdbool.h>

/** String indicating no SSN */
#define SSN_NA "N/A"

/**
  Calls the function that reads the content of the SSN field from standard
  input and stores it in the given string. The function exits with an exit
  status of 103 if the social security number is missing, or if the social
  security number is too long.
  @param ssn the ssn that is being read in and checked to see if it fits the format.
*/
void read_ssn(char ssn[FIELD_MAX + 1]);

/**
  Calls the function that converts the string of the social security number to
  the standard format. The function exits with an exit status of 103 if the string
  of the ssn is not in an acceptable format for a necessary ssn.
  @param ssn the ssn that is being fixed to standard format.
*/
void fix_ssn(char ssn[FIELD_MAX + 1]);