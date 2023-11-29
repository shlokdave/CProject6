/**
  @file input.c
  @author Shlok Dave (ssdave)
  This file acts as the source file for the input component. As the
  source file of the input component, it contains the function definition
  of one specific function. This specific component helps read the
  input from the park files and it will also be used to read commands that
  are inputted by the user.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

/** Initial capacity that helps allocate for buffer. The value represents the starting size. */
#define CAPACITY 50

/** 100 is represented as the size of the temporary buffer that is used for reading the file. */
#define BUFFER 100

/** Represents the growth factor that is used for increasing the capacity of the buffer. */
#define CAPACITY_ADD 2

/**
  This is the only function that is part of the input component. The main
  responsibility of this function is to read a single input from the given input
  stream. After reading the input, it returns it as a string inside a block of
  dynamically allocated memory. This function also reads commands from the user
  and park descriptions from the park file. If there is no input, it returns
  null. This method contains the actual definition and implementation of the function.
  @param fp pointer to the input stream, which can be stdin or a file.
  @return pointer to the line that is read stored in the allocated array. It returns
  null if the EOF is reached without any of the characters being read.
*/
char *readLine(FILE *fp)
{
    char newBuffArrary[BUFFER];
    char *newLineChar = (char *)malloc(CAPACITY * sizeof(char));
    int initalized_capacity = CAPACITY;
    int lengthOfLine = 0;

    if (!newLineChar)
    {
        perror("Did not pass the memory allocation.");
        exit(1);
    }

    while (fgets(newBuffArrary, BUFFER, fp))
    {
        int newBuffArraryLen = strlen(newBuffArrary);

        if (newBuffArraryLen > 0 && newBuffArrary[newBuffArraryLen - 1] == '\n')
        {
            newBuffArrary[--newBuffArraryLen] = '\0';
        }

        if (initalized_capacity <= lengthOfLine + newBuffArraryLen)
        {
            while (initalized_capacity <= lengthOfLine + newBuffArraryLen)
            {
                initalized_capacity *= CAPACITY_ADD;
            }
            char *newLineChar2 = (char *)realloc(newLineChar, initalized_capacity * sizeof(char));

            if (!newLineChar2)
            {
                free(newLineChar);
                perror("Did not pass the memory allocation.");
                exit(1);
            }
            newLineChar = newLineChar2;
        }
        strcpy(newLineChar + lengthOfLine, newBuffArrary);
        lengthOfLine += newBuffArraryLen;

        if (newBuffArraryLen < BUFFER - 1 || newBuffArrary[BUFFER - 2] == '\n')
        {
            break;
        }
    }

    if (feof(fp) && lengthOfLine == 0)
    {
        free(newLineChar);
        return NULL;
    }

    return newLineChar;
}