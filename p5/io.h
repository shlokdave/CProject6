/**
  @file io.h
  @author Shlok Dave (ssdave)
  This file acts as the header file for the io component. As the
  header file, this file is responsible for containing the declarations
  of variables and functions that are used in the other source files.
  This component is responsible for reading input and writing output files
  a block at a time and contains only two functions.
*/

#include <stdio.h>
#include "DES.h"

/**
  This function is responsible for reading up to eight bytes from the given
  input file. While reading, it stores them in the data array of block and setting
  the len field to indicate the number of bytes that were read to. The function does
  not directly return a value but it operates directly on the pointers of the
  DESBlock. This function is the declaration part of the header file.
  @param fp pointer to the file object to represent a file that needs to be read from.
  The file is opened depending on the mode.
  @param block pointer to the DESBlock where the data that is read will be stored.
 */
void readBlock(FILE *fp, DESBlock *block);

/**
  This function is responsible for writing the contents of the data array in block to
  the specific file. The len field of block will indicate the number of bytes that the
  block will contain. It does not return any value but it directly writes to the file that
  is referred to from the file pointer. This function is the declaration part of the header
  file.
  @param fp pointer to the file that is represented as an open file. The pointer must be valid
  so that the file is opened correctly for writing.
  @param block pointer to the constant DESBlock, from which the data will be written to the
  specific file.
  */
void writeBlock(FILE *fp, DESBlock const *block);