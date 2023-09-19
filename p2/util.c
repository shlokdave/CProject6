/**
  @file util.c
  @author Shlok Dave (ssdave)
  This file is responsible to represent the util component of the personal info 
  formatter. This component handles the functions of processing and manipulating
  strings. This component is also used by all the other components that are part
  of the program. Unlike the header file, this file contains the actual source code
  that describes the functionality of the program.
  */

#include "util.h"
#include <ctype.h>

/**
  This function is responsible for taking a string and a starting index in
  that string. The function then searches through the string and is able to find
  the index of the null terminator at the end of the string. The function is also
  responsible for finding sequences of digits.
  @param str the input that the string is checked on.
  @param start the index that of the starting point to skip digits.
  @reutrn the index of the first non-digit.
*/
int skip_digits(char str[], int start )
{
  while (isdigit(str[start])) {
      start++;
    }
    return start;

}

/**
  This function is very similar to skip_digits, however it skips the difference
  of the letters instead of the digits. This especially helps when parsing parts
  of the string of the name. 
  @param str the input string to check.
  @param start the starting index of the starting point to skip letters.
  @return the index of the first non-character.
*/
int skip_letters( char str[], int start )
{
   while (isalpha(str[start])) {
      start++;
    }
    return start;
}

/**
  This function is responsible for copying a sequence of characters from the 
  src string to the dest string. The function copies all the characters that 
  are from the sstart index in src to the send index. The characters are 
  copied into the dest string. 
  @param dest this string acts as the final destination string.
  @param dstart the starting index for the copying.
  @param src the source string for the substring.
  @param sstart this is the starting point of the substring.
  @param send this is the ending point of the substring.
  @return the index of the last string.
*/
int copy_substring( char dest[], int dstart, char src[], int sstart, int send )
{
  int index = dstart;
  for (int newIndex = sstart; send > newIndex; newIndex++, index++ ) {
    dest[index] = src[newIndex];
  }
  //Null terminator
  dest[index] = '\0';
  return index;
}