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
    printf("%08x\n", 0xc3d2e1f0 + 0xc3efe9db);
    const char* inputString = "0f1e2d3c4b5a69788796a5b4c3d2e1f0";

    u32 key[4];
    stringToWordArray(inputString, key);
    
    // Print the plaintext
    printf("Key: ");
    for (int i = 0; i < sizeof(key)/sizeof(u32); i++) {
        // if (!(i % 2)) printf(":");
        printf("%08x:", key[i]);
    }
    printf("\n");

    u32 roundkey[144]; // 192 * 24 = 4608 = 32 * 144
    leaEncKeySchedule(key, roundkey, LEA_VERSION);
    printf("\nRoundKey: \n");
    for (int i = 0, j = 0;
         i < 144;
         i++) {
        if((i % 6) == 0) printf("\nRound[%02d] | ", j++);
        printf("%08x:", roundkey[i]);
    }
    printf("\n");
}
