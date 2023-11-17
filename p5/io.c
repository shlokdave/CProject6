/**
  @file io.c
  @author Shlok Dave (ssdave)
  This file acts as the source file for the io component. As the
  header file, this file is responsible for containing the actual definition and
  implementations that are used in the other source files. This component is responsible
  for reading input and writing output files a block at a time and contains only
  two functions.
*/

#include "io.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
  This function is responsible for reading up to eight bytes from the given
  input file. While reading, it stores them in the data array of block and setting
  the len field to indicate the number of bytes that were read to. The function does
  not directly return a value but it operates directly on the pointers of the
  DESBlock. This function contains the actual implementation of the function.
  @param fp pointer to the file object to represent a file that needs to be read from.
  The file is opened depending on the mode.
  @param block pointer to the DESBlock where the data that is read will be stored.
 */
void readBlock(FILE *fp, DESBlock *block)
{
    // Check if the file pointer or block pointer is null
    if (fp == NULL || block == NULL)
    {
        fprintf(stderr, "The file pointer or block is null.\n");
        return;
    }

    // Reading the data from the file
    size_t readingUsingBytes = fread(block->data, sizeof(byte), BLOCK_BYTES, fp);

    // Checking number of bytes if they are less than BLOCK_BYTES
    if (readingUsingBytes < BLOCK_BYTES)
    {
        // EOF check
        if (feof(fp))
        {
            block->len = (int)readingUsingBytes;
            return;
        }
        else if (ferror(fp))
        {
            block->len = 0;
            return;
        }

        // Calculates the padding value that is stored
        byte calcPad = BLOCK_BYTES - (byte)readingUsingBytes;

        // Loop to pad the rest of the block within the new pad value
        for (size_t idx = readingUsingBytes; idx < BLOCK_BYTES; idx++)
        {
            block->data[idx] = calcPad;
        }
        block->len = BLOCK_BYTES;
    }
    else
    {
        // Block length set to the number of bytes read
        block->len = (int)readingUsingBytes;
    }
}

/**
  This function is responsible for writing the contents of the data array in block to
  the specific file. The len field of block will indicate the number of bytes that the
  block will contain. It does not return any value but it directly writes to the file that
  is referred to from the file pointer. This function contains the actual implementation
  of the function.
  @param fp pointer to the file that is represented as an open file. The pointer must be valid
  so that the file is opened correctly for writing.
  @param block pointer to the constant DESBlock, from which the data will be written to the
  specific file.
  */
void writeBlock(FILE *fp, DESBlock const *block)
{
    // Check if the file pointer or block pointer is null
    if (fp == NULL || block == NULL)
    {
        fprintf(stderr, "The file pointer or block is null.\n");
        return;
    }

    // Writing the data to the file
    size_t writingUsingBytes = fwrite(block->data, sizeof(byte), block->len, fp);

    // Checking number of bytes
    if (writingUsingBytes != block->len)
    {
        fprintf(stderr, "Error writing to the file.\n");
    }
}