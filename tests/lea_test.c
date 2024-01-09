/**
 * @file lea_test.c
 * @brief Test suite for the LEA encryption algorithm.
 *
 * Implements various tests to ensure the correct functionality of the LEA
 * encryption algorithm. This includes unit tests for individual components
 * and integration tests.
 */

#include "lea.h"

/**
 * @file main.c
 * @brief Main entry point for the LEA encryption algorithm demonstration.
 *
 * This file contains the main function to demonstrate the usage of the LEA
 * encryption algorithm. It may include example usage or benchmarking.
 */

#include <stdio.h>

#include "lea.h"

int main(void) {
    u32 key[4];
    const char* keyString = "0f1e2d3c4b5a69788796a5b4c3d2e1f0";
    stringToWordArray(keyString, key);
    
    // Print the plaintext
    printf("Key: \n");
    printLittleEndian(key, 4);

    u32 enc_roundkey[144]; // 192 * 24 = 4608 = 32 * 144
    leaEncKeySchedule(key, enc_roundkey, LEA_VERSION);
    printf("\nRoundKey: \n");
    for (int i = 0, j = 0;
         i < 144;
         i++) {
        if((i % 6) == 0) printf("\nRound[%02d] | ", j++);
        printf("%08x:", enc_roundkey[i]);
    }
    printf("\n");

    u32 plain[4];
    u32 cipher[4];

    const char* plainString = "101112131415161718191a1b1c1d1e1f";
    stringToWordArray(plainString, plain);

    leaEncrypt(plain, enc_roundkey, cipher);

    printf("\nCipherText: \n");
    printLittleEndian(cipher, 4);
}

