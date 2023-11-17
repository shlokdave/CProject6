/**
    @file DES.h
    @author Shlok Dave (ssdave)
    Header for the DES Implementation. This component will be the main
    source of all of the DES algorithms take place for encryption and
    decryption. As the header file, it provides all the function declarations
    and the variables that are used in the source file. The component has a total
    of nine functions as part of the DES algorithm.
*/

#include <string.h>
#include <stdio.h>
#include "DESMagic.h"

/** Number of bits in a byte. */
#define BYTE_SIZE 8

/** Round a number of bits up to the nearest number of bytes needed
    to store that many bits. */
#define ROUND_TO_BYTES(bits) (((bits) + BYTE_SIZE - 1) / BYTE_SIZE)

/** Number of bytes in a DES block. */
#define BLOCK_BYTES ROUND_TO_BYTES(BLOCK_BITS)

/** Number of bytes in the left or right halves of a block (L and R). */
#define BLOCK_HALF_BYTES ROUND_TO_BYTES(BLOCK_HALF_BITS)

/** Number of bytes to store the left-side and right-side values (C
    and D) used to create the subkeys. */
#define SUBKEY_HALF_BYTES ROUND_TO_BYTES(SUBKEY_HALF_BITS)

/** Number of bytes to store a whole subkey (K_1 .. K_16). */
#define SUBKEY_BYTES ROUND_TO_BYTES(SUBKEY_BITS)

/** Type used to represent a block to encrypt or decrypt with DES. */
typedef struct
{
  /** Sequence of bytes in the block. */
  byte data[BLOCK_BYTES];

  /** Number of bytes currently in the data array (e.g., the last block in a file could
      be shorter. */
  int len;
} DESBlock;

/**
  This function checks the given text key to make sure that the key doesn't exceed
  the limit. This specific function copies the characters from the text key array into
  the key array. It copies the characters until the specific length of the DES
  block. This function contains the declaration of the function.
  @param key array of bytes that contain the size of BLOCK_BYTES. This specific key
  array would contain the contents of the text key array.
  @param textKey pointer as the null-terminated character array. All of the data in this
  array would be copied into the key array.
 */
void prepareKey(byte key[BLOCK_BYTES], char const *textKey);

/**
  This function is responsible for the one-based value of the bit at the specific
  index. This occurs in the given array of bytes. It calculates the exact byte and bit
  position in the array based on the index given. The getBit function makes the whole bit
  manipulation process easier with the DES algorithm. This function contains the declaration
  of the function.
  @param data the array of the byte that is being extracted.
  @param idx the index of the bit that is being extracted.
  @return the bit value at the specific index that is given. It can be either 0 or 1.
 */
int getBit(byte const data[], int idx);

/**
  This function is responsible for clearing or setting the bit at an index (idx) of the
  data array. It first determines the exact byte and bit position, then setting or clearing
  the bit at that specific position. The index is 1-based. This function contains the declaration
  of the function.
  @param data the array of the byte that is being set or cleared.
  @param idx the index of the bit that is being changed.
  @param val the value that the bit will be set at. 0 means that that it will be cleared, while
  1 means that it will be set.
 */
void putBit(byte data[], int idx, int val);

/**
  This function performs the permute operation, while copying n bits from the given input
  array to the output. If n isn't a multiple of 8, then the function should set the remaining
  bits in the last byte to zero. In all, this function helps with the main permutation operation
  that is part of the DES algorithm. This function contains the declaration of the function.
  @param output the array of the bytes that the bits will be permuted to.
  @param input the array of the bytes that are being permuted from.
  @param perm the array of integers that will determine the permutation order.
  @param n integer representation of the number of bits that will be permuted.
 */
void permute(byte output[], byte const input[], int const perm[], int n);

/**
  This function is responsible for computing 16 subkeys based on the input key and stores
  each one in an element of the given K array. The resulting subkeys are stored and the element
  zero of the subkey isn't used. In all, the function generates subkeys for a block cipher with
  a designated key. This function contains the declaration of the function.
  @param K the array of subkeys that will be stored in a different array.
  @param key the key that is used for the main encryption.
 */
void generateSubkeys(byte K[ROUND_COUNT][SUBKEY_BYTES], byte const key[BLOCK_BYTES]);

/**
  This function is responsible for returning the rest of an S-Box calculation. The index
  values range from zero to seven and the result is an element of the S-Box table. The index
  is counted from zero for B1 and one for B2. The result is a four bit value which is stored
  in the high-order four bits. This function contains the declaration of the function.
  @param output the array of the bytes to store the output of the S-box calculation.
  @param input the 48-bit array that is designated as the input array.
  @param idx the index that the S-Box calculation will use and this number ranges from
  zero to seven.
 */
void sBox(byte output[1], byte const input[SUBKEY_BYTES], int idx);

/**
  This function is responsible for computing the f function based on the 32-bit value R
  and the given 48-bit subkey, S. After these calculations, the result is stored in the
  result array. The function primarily focuses on expanding the data, mixing it with the
  sub key, and performing permutations at the end. This function contains the declaration
  of the function.
  @param result the array of bytes that the final output is stored in.
  @param R the right half of the block of data. The size should be the same as BLOCK_HALF_BYTES.
  @param K subkey that is used for the expansion and mixing. The size should be the
  same as SUBKEY_BYTES.
 */
void fFunction(byte result[BLOCK_HALF_BYTES], byte const R[BLOCK_HALF_BYTES], byte const K[SUBKEY_BYTES]);

/**
  This function performs the encrypt operation on the byte array in block, using the subkeys
  in the K array. First, a new block is created by performing the initial permutation. Second,
  the block is split into two halves. Finally, the function processes each half and the left
  and right halves are swapped. After all of this, the final encrypted result is stored back
  in the given block. This function contains the declaration of the function.
  @param block pointer to the block structure that contains the data that needs to be
  encrypted. After the process, the encrypted data is also stored here.
  @param K 2D array that contains the subkeys for each part of the encryption process.
 */
void encryptBlock(DESBlock *block, byte const K[ROUND_COUNT][SUBKEY_BYTES]);

/**
  This function performs the decrypt operation on the byte array in block, using the
  subkeys in the K array. After the entire process, the decrypted result is stored back
  in the given block. For simplicity, this function uses the encryptBlock to reverse the
  keys. This function contains the declaration of the function.
  @param block pointer to the block structure that contains the data that needs to be
  decrypted. After the process, the decrypted data is also stored here.
  @param K 2D array that contains the subkeys for each part of the decryption process.
 */
void decryptBlock(DESBlock *block, byte const K[ROUND_COUNT][SUBKEY_BYTES]);