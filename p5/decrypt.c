/**
  @file decrypt.c
  @author Shlok Dave (ssdave)
  This file acts as the main component for the decrypt program. It
  contains the main function and uses other components to read the
  input file. The file also uses the main function to perform the decryption
  and write out the plaintext output. Specifically, the decryption process is through
  the DES algorithm.
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
  This function acts as the main program for the decryption process using DES. The program
  takes in three command-line arguments and decrypts the contents of the input file
  using the specific key. After this process, it writes the decrypted data to the
  output file. As the main function, this function is the starting point of the program
  execution. It controls program execution by directing calls to the other functions that
  are part of the program. The program continues to read, decrypt, and write blocks until
  it reaches the end of the file.
  @param argc number of command-line arguments represented as an integer.
  @param argv array of command-line arguments represented as a string.
  @return 0 if the program is correctly executed. The program has successfully completed the
  decryption with the return value of 0. The program returns 1 if there was an error at any point
  of the decryption process. These errors could include key length being too large or
  the file is not opened in the proper format.
 */
int main(int argc, char *argv[])
{
  // Checking if the correct number of arguments are being passed
  if (argc != EXPECTED_ARG_COUNT)
  {
    fprintf(stderr, "usage: decrypt <key> <input_file> <output_file>\n");
    return 1;
  }

  // Key length is checked if it is too long
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
    fclose(firstInFile);
    return 1;
  }

  // Output file is opened in binary mode to read
  FILE *firstOutFile = fopen(argv[THIRD_ARG], "wb");
  if (!firstOutFile)
  {
    perror(argv[THIRD_ARG]);
    fclose(firstOutFile);
    return 1;
  }

  // Initializing key and the subkeys
  byte initKey[BLOCK_BYTES];
  byte initSubKey[ROUND_COUNT][SUBKEY_BYTES];
  prepareKey(initKey, argv[1]);
  generateSubkeys(initSubKey, initKey);

  // Initializing block to help process all of the data
  DESBlock block;

  // Processing blocks until the end of the file
  while (1)
  {
    // Reads the block of data
    readBlock(firstInFile, &block);
    if (block.len == 0)
    {
      break;
    }

    // Decrypting the block using DES algorithm
    decryptBlock(&block, initSubKey);
    int value = 0;

    // For loop to help remove the padding
    for (int idx = block.len - 1; idx >= 0; idx--)
    {
      // Check to see the bytes
      if (block.data[idx] == 0x00)
      {
        value++;
      }
      else
      {
        break;
      }
    }
    block.len -= value;
    writeBlock(firstOutFile, &block);
  }

  // Closing all of the files
  fclose(firstInFile);
  fclose(firstOutFile);

  return 0;
}