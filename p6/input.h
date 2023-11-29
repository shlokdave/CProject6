/**
  @file input.h
  @author Shlok Dave (ssdave)
  This file acts as the header file for the input component. As the
  header file, this file is responsible for containing the declarations
  of variables and functions that are used in the other source files.
  This specific component helps read the input from the park files and
  it will also be used to read commands that are inputted by the user.
*/

#include <stdio.h>

/**
  This is the only function that is part of the input component. The main
  responsibility of this function is to read a single input from the given input
  stream. After reading the input, it returns it as a string inside a block of
  dynamically allocated memory. This function also reads commands from the user
  and park descriptions from the park file. If there is no input, it returns
  null. This method contains only the declaration of the function as it is part
  of the header file.
  @param fp pointer to the input stream, which can be stdin or a file.
  @return pointer to the line that is read stored in the allocated array. It returns
  null if the EOF is reached without any of the characters being read.
*/
char *readLine(FILE *fp);