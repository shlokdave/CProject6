/**
  @file encrypt.c
  @author Shlok Dave (ssdave)
  This file acts as the main component for the encrypt program. It
  contains the main function and uses other components to read the
  input file. The file also uses the main function to perform the encryption
  and write out the ciphertext output. Specifically, the process is through
  DES.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

/** Expected argument count for the main function */
#define EXPECTED_ARG_COUNT 4

/** Second command-line argument*/
#define SECOND_ARG 2

/** Third command-line argument*/
#define THIRD_ARG 3

/**
  This function acts as the main program for the encryption process using DES. The program
  takes in three command-line arguments and encrypts the contents of the input file
  using the provided key. After this process, it writes the encrypted data to the
  output file. As the main function, this function is the starting point of the program
  execution. It controls program execution by directing calls to the other functions that
  are part of the program.
  @param argc number of command-line arguments represented as an integer.
  @param argv array of command-line arguments represented as a string.
  @return 0 if the program is correctly executed. With a return value of 0, the program has
  successfully completed the encryption. The program returns 1 if there was an error at any
  point of the encryption process. These errors could include key length being too large or
  the file not being able to be opened correctly.
 */
int main(int argc, char *argv[])
{
    // Checking if the correct number of arguments are being passed
    if (argc != EXPECTED_ARG_COUNT)
    {
        fprintf(stderr, "Usage: encrypt <key> <input_file> <output_file>\n");
        return 1;
    }

    // Checking if the key is not too long
    if (BYTE_SIZE < strlen(argv[1]))
    {
        fprintf(stderr, "Key too long\n");
        return 1;
    }

    // Input file is opened in binary mode to read
    FILE *firstInFile = fopen(argv[SECOND_ARG], "rb");
    if (!firstInFile)
    {
        perror(argv[SECOND_ARG]);
        return 1;
    }

    // Output file is opened in binary mode to write
    FILE *firstOutFile = fopen(argv[THIRD_ARG], "wb");
    if (!firstOutFile)
    {
        perror(argv[THIRD_ARG]);
        fclose(firstInFile);
        return 1;
    }

    // Initializing key and the subkeys
    byte initKey[BLOCK_BYTES];
    byte initSubKey[ROUND_COUNT][SUBKEY_BYTES];
    prepareKey(initKey, argv[1]);
    generateSubkeys(initSubKey, initKey);

    // Initializing block to help process all of the data
    DESBlock newDESBlock;
    size_t readBit;

    // While loop used to read the file by each block
    while ((readBit = fread(newDESBlock.data, sizeof(byte), BLOCK_BYTES, firstInFile)))
    {
        memset(newDESBlock.data + readBit, 0, BLOCK_BYTES - readBit);
        newDESBlock.len = BLOCK_BYTES;
        encryptBlock(&newDESBlock, initSubKey);
        writeBlock(firstOutFile, &newDESBlock);
    }

    // Closing each of the files
    fclose(firstInFile);
    fclose(firstOutFile);

    return 0;
}
