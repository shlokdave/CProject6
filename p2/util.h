/**
  @file util.h
  @author Shlok Dave (ssdave)
  This file is considered a header file for the util.c file. This file
  contains the declarations of variables and functions that are utilized
  in the other source files. With this header file, these declarations are
  directly used in the util.c file.
 */

#include <stdbool.h>
#include <ctype.h>

/** Maximum length of any field in the input. */
#define FIELD_MAX 30

/**
  Calls the function is responsible for taking a string and a starting index in
  that string. The function then searches through the string and is able to find
  the index of the null terminator at the end of the string. The function is also
  responsible for finding sequences of digits.
  @param str the input that the string is checked on.
  @param start the index that of the starting point to skip digits.
  @return the index of the first non-digit.
*/
int skip_digits(char str[], int start);

/**
  Calls the function that is very similar to skip_digits, however it skips the difference
  of the letters instead of the digits. This especially helps when parsing parts
  of the string of the name.
  @param str the input string to check.
  @param start the starting index of the starting point to skip letters.
  @return the index of the first non-character.
*/
int skip_letters(char str[], int start);

/**
  Calls the function responsible for copying a sequence of characters from the
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
int copy_substring(char dest[], int dstart, char src[], int sstart, int send);
