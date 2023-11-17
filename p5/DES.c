/**
    @file DES.c
    @author Shlok Dave (ssdave)
    Implementation of the DES algorithm. This component will be the main
    source of all of the DES algorithms take place for encryption and
    decryption. As the source file, it provides all the function definitions and
    their respective implementations. The function implements a total of nine
    functions with one static helper function.
*/

#include "DES.h"
#include <stdio.h>

/** SBOX value divided by 2*/
#define HALF_SBOX 2

/** SBOX input value divided by 2*/
#define HALF_SBOX_INPUT 3

/**
  This function checks the given text key to make sure that the key doesn't exceed
  the limit. This specific function copies the characters from the text key array into
  the key array. It copies the characters until the specific length of the DES
  block. This function contains the actual implementation of the function.
  @param key array of bytes that contain the size of BLOCK_BYTES. This specific key
  array would contain the contents of the text key array.
  @param textKey pointer as the null-terminated character array. All of the data in this
  array would be copied into the key array.
 */
void prepareKey(byte key[BLOCK_BYTES], char const *textKey)
{
  int newIndex = 0;

  // Loop to ensure current character is not null and the index is not more than the size
  while (textKey[newIndex] != '\0' && newIndex < BLOCK_BYTES)
  {
    key[newIndex] = (byte)textKey[newIndex];
    newIndex++;
  }

  // Loop pads the remaining of the array with 0's
  while (newIndex < BLOCK_BYTES)
  {
    key[newIndex++] = 0;
  }
}

/**
  This function is responsible for the one-based value of the bit at the specific
  index. This occurs in the given array of bytes. It calculates the exact byte and bit
  position in the array based on the index given. The getBit function makes the whole bit
  manipulation process easier with the DES algorithm. This function contains the actual
  implementation of the function.
  @param data the array of the byte that is being extracted.
  @param idx the index of the bit that is being extracted.
  @return the bit value at the specific index that is given. It can be either 0 or 1.
 */
int getBit(byte const data[], int idx)
{
  // Directly compare the index of the byte with the position it is at
  int indexByte = (idx - 1) / BYTE_SIZE;
  int positionBit = (idx - 1) % BYTE_SIZE;

  // Get the bit using the single bitwise operation
  return (data[indexByte] >> ((BYTE_SIZE - 1) - positionBit)) & 1;
}

/**
  This function is responsible for clearing or setting the bit at an index (idx) of the
  data array. It first determines the exact byte and bit position, then setting or clearing
  the bit at that specific position. The index is 1-based. This function contains the actual
  implementation of the function.
  @param data the array of the byte that is being set or cleared.
  @param idx the index of the bit that is being changed.
  @param val the value that the bit will be set at. 0 means that that it will be cleared, while
  1 means that it will be set.
 */
void putBit(byte data[], int idx, int val)
{
  // Directly comparing the index of the byte with its position
  int indexByte = (idx - 1) / BYTE_SIZE;
  int positionBit = (idx - 1) % BYTE_SIZE;

  // Mask created for the position of the bit
  byte newMask = 1 << ((BYTE_SIZE - 1) - positionBit);

  // If statement to help check the value of val
  if (!val)
  {
    // Bit is set using the AND operator
    data[indexByte] &= ~newMask;
  }
  else
  {
    // Bit is set using the OR operator
    data[indexByte] |= newMask;
  }
}

/**
  This function performs the permute operation, while copying n bits from the given input
  array to the output. If n isn't a multiple of 8, then the function should set the remaining
  bits in the last byte to zero. In all, this function helps with the main permutation operation
  that is part of the DES algorithm. This function contains the actual implementation of the function.
  @param output the array of the bytes that the bits will be permuted to.
  @param input the array of the bytes that are being permuted from.
  @param perm the array of integers that will determine the permutation order.
  @param n integer representation of the number of bits that will be permuted.
 */
void permute(byte output[], byte const input[], int const perm[], int n)
{
  int calculateNumBytes = (n + (BYTE_SIZE - 1)) / BYTE_SIZE;

  // Initialize output array to zero
  for (int idx = 0; idx < calculateNumBytes; idx++)
  {
    output[idx] = 0;
  }

  // Permutation logic
  for (int idx = 0; idx < n; idx++)
  {
    int bitCopy = getBit(input, perm[idx]);
    putBit(output, idx + 1, bitCopy);
  }

  // Handle any leftover bits in the last byte if n is not a multiple of 8
  if (n % BYTE_SIZE != 0)
  {
    byte newMask = (1 << (BYTE_SIZE - n % BYTE_SIZE)) - 1;
    output[calculateNumBytes - 1] &= ~newMask;
  }
}

/**
  This function is responsible for computing 16 subkeys based on the input key and stores
  each one in an element of the given K array. The resulting subkeys are stored and the element
  zero of the subkey isn't used. In all, the function generates subkeys for a block cipher with
  a designated key. This function contains the actual implementation of the function.
  @param K the array of subkeys that will be stored in a different array.
  @param key the key that is used for the main encryption.
 */
void generateSubkeys(byte K[ROUND_COUNT][SUBKEY_BYTES], byte const key[BLOCK_BYTES])
{
  // Initial separation
  byte Left[SUBKEY_HALF_BYTES] = {0};
  byte Right[SUBKEY_HALF_BYTES] = {0};

  // Perform intital permutations
  permute(Left, key, leftSubkeyPerm, SUBKEY_HALF_BITS);
  permute(Right, key, rightSubkeyPerm, SUBKEY_HALF_BITS);

  // Iterate through each round
  for (int idx = 1; idx <= ROUND_COUNT; idx++)
  {
    int encryptShift = subkeyShiftSchedule[idx];

    // Start shifting process
    for (int idx1 = 0; idx1 < (SBOX_OUTPUT_BITS / 2); idx1++)
    {
      byte *newStoreBlock = (idx1 == 0) ? Right : Left;
      int tHalf = (SUBKEY_HALF_BITS + (BYTE_SIZE - 1)) / BYTE_SIZE;
      byte tByte[SUBKEY_HALF_BYTES] = {0};
      memcpy(tByte, newStoreBlock, tHalf);

      // Perform circular shift
      for (int idx2 = 0; idx2 < encryptShift; idx2++)
      {
        for (int idx3 = SUBKEY_HALF_BITS; idx3 > 0; idx3--)
        {
          int getRequiredBit = getBit(newStoreBlock, idx3);
          if (idx3 == 1)
          {
            putBit(tByte, SUBKEY_HALF_BITS, getRequiredBit);
          }
          else
          {
            putBit(tByte, idx3 - 1, getRequiredBit);
          }
        }
        memcpy(newStoreBlock, tByte, tHalf);
      }
    }

    // Swap left and right blocks
    byte newSwapR[SUBKEY_HALF_BYTES] = {0};
    byte newSwapL[SUBKEY_HALF_BYTES] = {0};
    memcpy(newSwapR, Right, SBOX_ROWS);
    memcpy(newSwapL, Left, SBOX_ROWS);
    byte mainK[SUBKEY_BYTES] = {0};

    // Combine the blocks
    for (int idx = 1; idx <= SUBKEY_HALF_BITS; idx++)
    {
      putBit(mainK, idx, getBit(newSwapL, idx));
      putBit(mainK, idx + SUBKEY_HALF_BITS, getBit(newSwapR, idx));
    }

    // Perform final permutation and store the key
    byte newKey[SUBKEY_BYTES] = {0};
    permute(newKey, mainK, subkeyPerm, SUBKEY_BITS);
    memcpy(K[idx], newKey, (SUBKEY_BITS / BYTE_SIZE));
  }
}

/**
  This function is responsible for returning the rest of an S-Box calculation. The index
  values range from zero to seven and the result is an element of the S-Box table. The index
  is counted from zero for B1 and one for B2. The result is a four bit value which is stored
  in the high-order four bits. This function contains the actual implementation of the function.
  @param output the array of the bytes to store the output of the S-box calculation.
  @param input the 48-bit array that is designated as the input array.
  @param idx the index that the S-Box calculation will use and this number ranges from
  zero to seven.
 */
void sBox(byte output[1], byte const input[SUBKEY_BYTES], int idx)
{
  // Initial declarations for calculating the starting position.
  int positionBit = idx * SBOX_INPUT_BITS;
  int bitsStored = 0;

  // Extracting the bits and perform calculations
  for (int idx = 0; idx < SBOX_INPUT_BITS; idx++)
  {
    int bitNum = (input[positionBit / SBOX_COUNT] >> ((BYTE_SIZE - 1) - (positionBit % SBOX_COUNT))) & 1;
    bitsStored = (bitsStored << 1) | bitNum;
    positionBit++;
  }

  // Perform calculations to figure out the row and column for table
  int sBoxRow = (bitsStored & BLOCK_HALF_BITS) >> SBOX_ROWS | (bitsStored & 1);
  int sBoxColumn = (bitsStored >> 1) & (SBOX_COLS - 1);

  // Extract the output from the table
  byte getOutput = sBoxTable[idx][sBoxRow][sBoxColumn];

  // Final output stored
  output[0] = 0;
  for (int idx = 0; idx < SBOX_OUTPUT_BITS; idx++)
  {
    putBit(output, idx + 1, (getOutput >> (SBOX_OUTPUT_BITS - 1 - idx)) & 1);
  }
}

/**
  This function is responsible for computing the f function based on the 32-bit value R
  and the given 48-bit subkey, S. After these calculations, the result is stored in the
  result array. The function primarily focuses on expanding the data, mixing it with the
  sub key, and performing permutations at the end. This function contains the actual
  implementation of the function.
  @param result the array of bytes that the final output is stored in.
  @param R the right half of the block of data. The size should be the same as BLOCK_HALF_BYTES.
  @param K subkey that is used for the expansion and mixing. The size should be the
  same as SUBKEY_BYTES.
 */
void fFunction(byte result[BLOCK_HALF_BYTES], byte const R[BLOCK_HALF_BYTES], byte const K[SUBKEY_BYTES])
{
  // Expand using the expansion table
  byte newBit[SUBKEY_BYTES];
  permute(newBit, R, expandedRSelector, SUBKEY_BITS);

  // Mix the keys
  byte mixKeys[SUBKEY_BYTES];
  for (int idx = 0; idx < SUBKEY_BYTES; idx++)
  {
    mixKeys[idx] = newBit[idx] ^ K[idx];
  }

  // Perform substitution utilizing the S-boxes
  byte subBox[SBOX_ROWS] = {0, 0, 0, 0};
  for (int i = 0; i < BYTE_SIZE; i++)
  {
    byte newOutput[1];
    sBox(newOutput, mixKeys, i);
    subBox[i / (SBOX_ROWS / HALF_SBOX)] |= (i % (SBOX_ROWS / HALF_SBOX) == 0) ? newOutput[0] : (newOutput[0] >> SBOX_ROWS);
  }

  // Perform final permutation with the substituted result
  permute(result, subBox, fFunctionPerm, BLOCK_HALF_BITS);
}

/**
  This function is responsible for acting as a helper function to the encryptBlock function.
  As the function is static, it can be only used in this class. The function takes two arrays,
  an array, and the size. The function XORs each of the bytes in the array of the left array.
  After this, it swaps the left array to get the right array.
  @param Left the byte array representing the left side of the block of data.
  @param Right the byte array representing the right side of the block of data.
  @param newOutput the byte array that contains the output from the calculations.
  @param arraySize the size of the left, right, and newOutput arrays. This number will help
  recognize the number of bytes that will need to be processed in the function.
 */
static void helperEncryptBlock(byte Left[], byte Right[], const byte newOutput[], int arraySize)
{
  // Loop to go through each of the bytes in the arrays
  for (int idx = 0; idx < arraySize; idx++)
  {
    // Stores the current byte from left, then swaps it to get the right
    byte newSwap = Left[idx];
    Left[idx] = Right[idx];
    Right[idx] = newSwap ^ newOutput[idx];
  }
}

/**
  This function performs the encrypt operation on the byte array in block, using the subkeys
  in the K array. First, a new block is created by performing the initial permutation. Second,
  the block is split into two halves. Finally, the function processes each half and the left
  and right halves are swapped. After all of this, the final encrypted result is stored back
  in the given block. This function contains the actual implementation of the function.
  @param block pointer to the block structure that contains the data that needs to be
  encrypted. After the process, the encrypted data is also stored here.
  @param K 2D array that contains the subkeys for each part of the encryption process.
 */
void encryptBlock(DESBlock *block, byte const K[ROUND_COUNT][SUBKEY_BYTES])
{
  // Create a new block and perform the initial permutation
  byte newBlock[BLOCK_BYTES] = {0};
  permute(newBlock, block->data, leftInitialPerm, BLOCK_HALF_BITS);
  permute(newBlock + BLOCK_HALF_BYTES, block->data, rightInitialPerm, BLOCK_HALF_BITS);

  // Split the block of data into left and right halves
  byte Left[BLOCK_HALF_BYTES] = {0};
  byte Right[BLOCK_HALF_BYTES] = {0};
  memcpy(Left, newBlock, BLOCK_HALF_BYTES);
  memcpy(Right, newBlock + BLOCK_HALF_BYTES, BLOCK_HALF_BYTES);

  // Loop to process each piece of the data through all the necessary rounds
  for (int idx = 1; idx < ROUND_COUNT; idx++)
  {
    byte newOutput[BLOCK_HALF_BYTES] = {0};
    fFunction(newOutput, Right, K[idx]);

    helperEncryptBlock(Left, Right, newOutput, BLOCK_HALF_BYTES);
  }

  // Swap the left and right halves and perform final permutation
  memcpy(newBlock, Right, BLOCK_HALF_BYTES);
  memcpy(newBlock + BLOCK_HALF_BYTES, Left, BLOCK_HALF_BYTES);
  permute(block->data, newBlock, finalPerm, BLOCK_BITS);
}

/**
  This function performs the decrypt operation on the byte array in block, using the
  subkeys in the K array. After the entire process, the decrypted result is stored back
  in the given block. For simplicity, this function uses the encryptBlock to reverse the
  keys. This function contains the actual implementation of the function.
  @param block pointer to the block structure that contains the data that needs to be
  decrypted. After the process, the decrypted data is also stored here.
  @param K 2D array that contains the subkeys for each part of the decryption process.
 */
void decryptBlock(DESBlock *block, byte const K[ROUND_COUNT][SUBKEY_BYTES])
{
  // Reverse order of the keys stored for decryption process
  byte reversedSubkeys[ROUND_COUNT][SUBKEY_BYTES];

  // Loop to help reverse the order of the keys
  for (int round = 1; round <= ROUND_COUNT; round++)
  {
    memcpy(reversedSubkeys[round - 1], K[ROUND_COUNT - round + 1], SUBKEY_BYTES);
  }

  // Call the encryptBlock method with the reversed keys to simplify the process
  encryptBlock(block, reversedSubkeys);
}