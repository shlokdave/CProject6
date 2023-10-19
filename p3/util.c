/**
  @file util.c
  @author Shlok Dave (ssdave)
  This file acts as the source file for the util component. Unlike the header file,
  this source file contains the actual definition and implmentation of the three main
  functions. This file contains the functions that are necessary to help detect
  overflow in the unsigned long type.
*/

#include "util.h"
#include <limits.h>

/**
  This function checks for overflow in an add of two unsigned low values. The
  function returns true if the two numbers are able to be added without an overflow
  occurring. This specific function is the definition and includes the actual
  implementation.
  @param a the first unsigned long integer that is checked for addition.
  @param b the second unsigned long integer that is checked for addition.
 */
bool checkAdd(unsigned long a, unsigned long b)
{
  return ULONG_MAX - a >= b;
}

/**
  This function checks for overflow in a subtraction of two unsigned long values.
  It returns true if b is able to be subtracted from a without an overflow
  occurring. This specific function is the definition that contains the actual
  implementation part of the source file.
  @param a the first unsigned long integer that is checked for subtraction.
  @param b the second unsigned long integer that is chcked for subtraction.
 */
bool checkSub(unsigned long a, unsigned long b)
{
  return a >= b;
}

/**
  This function checks for overlow in a mutliplcity of two unsigned long values.
  The function returns true if the values of a and b can be multiplied without
  an overflow occurring. This specific function contains the definition and
  the implementation.
  @param a the first unsigned long integer that is chcked for multiplication.
  @param b the second unsigned long integer that is chcekd for mulitplication.
 */
bool checkMul(unsigned long a, unsigned long b)
{
  return (a == 0) || (b == 0) || (a <= ULONG_MAX / b);
}
