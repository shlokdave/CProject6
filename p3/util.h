/**
  @file util.h
  @author Shlok Dave (ssdave)
  This file acts as the header file for the util component. In this header file,
  there are three functions that are declared, that are directly used in the
  util.c file. As the header file, only the function declarations are in this file,
  as the function definition and implementation belongs in the util.c file.
*/

#include <stdio.h>
#include <stdbool.h>

/**
  This function checks for overflow in an add of two unsigned low values. The
  function returns true if the two numbers are able to be added without an overflow
  occurring. This specific function is the declaration in the header file.
  @param a the first unsigned long integer that is checked for addition.
  @param b the second unsigned long integer that is checked for addition.
 */
bool checkAdd(unsigned long a, unsigned long b);

/**
  This function checks for overflow in a subtraction of two unsigned long values.
  It returns true if b is able to be subtracted from a without an overflow
  occurring. This specific function is the declaration that is part of the header
  file.
  @param a the first unsigned long integer that is checked for subtraction.
  @param b the second unsigned long integer that is chcked for subtraction.
 */
bool checkSub(unsigned long a, unsigned long b);

/**
  This function checks for overlow in a mutliplcity of two unsigned long values.
  The function returns true if the values of a and b can be multiplied without
  an overflow occurring. This specific function is the declaration part of the
  header file.
  @param a the first unsigned long integer that is chcked for multiplication.
  @param b the second unsigned long integer that is chcekd for mulitplication.
 */
bool checkMul(unsigned long a, unsigned long b);