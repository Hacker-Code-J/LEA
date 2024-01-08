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
    const char* inputString = "0f1e2d3c4b5a69788796a5b4c3d2e1f0";
    u32 key[4];
    stringToWordArray(inputString, key);
    // Print the plaintext
    printf("Key: ");
    for (int i = 0; i < 4; i++) {
        if (!(i % 2)) printf(":");
        printf("%08x", key[i]);
    }
    printf("\n");

    u32 roundkey[24];
    leaEncKeySchedule(key, roundkey);
    printf("RoundKey: \n");
    for (int i = 0; i < 24; i++) {
        printf("%08x:", roundkey[i]);
        if(!(i % 6) && i != 0) printf("\n");
    }
    printf("\n");
}
