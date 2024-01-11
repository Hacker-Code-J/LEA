/**
 * @file lea_utils.c
 * @brief Utility functions for the LEA encryption algorithm.
 *
 * Contains implementation of various utility functions such as byte manipulation
 * required by the LEA encryption and decryption processes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lea.h"

void keyToWordArray(const char* hexString, u32* wordArray) {
    size_t length = strlen(hexString);

    // Calculate the number of 32-bit words
    size_t numWords = length / 8;
    if (length % 8 != 0) {
        numWords++;  // Add one more word if there are extra characters
    }

    for (size_t i = 0; i < numWords; i++) {
        char temp[9] = {0};  // Temporary buffer to store 8 characters + null terminator
        size_t start = length > 8 ? length - 8 : 0;  // Calculate the start index for the current word

        strncpy(temp, &hexString[start], length - start);  // Copy the relevant part of the string
        sscanf(temp, "%8x", &wordArray[i]);  // Read the value into the word array

        if (length >= 8) {
            length -= 8;  // Move to the previous word
        }
    }
}

void stringToWordArray(const char* hexString, u32* wordArray) {
    size_t length = strlen(hexString);
    for (size_t i = 0; i < length; i += 8) {
        sscanf(&hexString[i], "%8x", &wordArray[i / 8]);
    }
}

void printBigEndian(u32* array, size_t size) {
    // printf("0x");
    for (size_t i = 0; i < size; i++) {
        u32 value = array[i];
        if (i == 4) printf("\n");
        for (int j = 3; j >= 0; j--) {
            printf("%02x:", (value >> (j * 8)) & 0xFF);
        }
    }
    printf("\n");
}

void printLittleEndian(u32* array, size_t size) {
    // printf("0x");
    for (size_t i = 0; i < size; i++) {
        u32 value = array[i];
        if (i == 4) printf("\n");
        for (int j = 0; j < 4; j++) {
            printf("%02x:", (value >> (j * 8)) & 0xFF);
        }
    }
    printf("\n");
}

void printEncRoundKeys(u32* enc_roundkey) {
    /*
     * RK[00] = 000 || 001 || 002 || 003 || 004 || 005
     * RK[01] = 006 || 007 || 008 || 009 || 010 || 011
     * RK[02] = 012 || 013 || 014 || 015 || 016 || 017
     * ...
     * RK[23] = 138 || 139 || 140 || 141 || 142 || 143
     * ...
     * RK[27] = 162 || 163 || 164 || 165 || 166 || 167
     * ...
     * RK[31] = 186 || 187 || 188 || 189 || 190 || 191
    */
    printf("\nEncryption RoundKey: \n");
    for (int i = 0, j = 0; i < TOTAL_RK; i++) {
        if((i % 6) == 0) printf("\nEnc_Round[%02d] | ", j++);
        printf("%08x:", enc_roundkey[i]);
    }
    printf("\n");
    
    // /*
    //  * RK[00] = 005 || 004 || 003 || 002 || 001 || 000
    //  * RK[01] = 011 || 010 || 009 || 008 || 007 || 006
    //  * RK[02] = 017 || 016 || 015 || 014 || 013 || 012
    //  * ...
    //  * RK[23] = 143 || 142 || 141 || 140 || 139 || 138
    //  * ...
    //  * RK[27] = 167 || 166 || 165 || 164 || 163 || 162
    //  * ...
    //  * RK[31] = 191 || 190 || 189 || 188 || 187 || 186
    // */
    // printf("\nEncryption RoundKey: \n");
    // int rows = Nr;
    // int elementsPerRow = 6;
    // int start = 5; // Starting number of the first row

    // for (int i = 0; i < rows; i++) {
    //     printf("Enc_Round[%02d] | ", i);
    //     int num = start;
    //     for (int j = 0; j < elementsPerRow; j++) {
    //         printf("%08x:", enc_roundkey[num--]);
    //     }
    //     start += elementsPerRow; // Set the starting number for the next row
    //     printf("\n");
    // }
}

void printDecRoundKeys(u32* dec_roundkey) {
    printf("\nDecryption RoundKey: \n");
    for (int i = 0, j = 0; i < TOTAL_RK; i++) {
        if((i % 6) == 0) printf("\nDec_Round[%02d] | ", j++);
        printf("%08x:", dec_roundkey[i]);
    }
    printf("\n");
}

double measure_time(void (*func)(const u32*, const u32*, u32*), const u32* src, const u32* key, u32* dst) {
    // srand((u32)time(NULL));
    // clock_t start, end;
    // double cpu_time_used;

    // start = clock();
    // func(input, key, output);
    // end = clock();

    // cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    // return cpu_time_used;

    struct timespec start, end;
    double cpu_time_used;
    const int num_runs = 10000; // Number of runs for averaging

    // Warm-up run (optional, but often a good idea)
    func(src, key, dst);

    clock_gettime(1, &start);  // Start timing
    // clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < num_runs; i++) {
        func(src, key, dst);
    }
    clock_gettime(1, &end); // End timing
    // clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate elapsed time in seconds
    cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    
    return cpu_time_used / num_runs; // Average time per run
}

// void stringToByteArray(const char* hexString, u8* byteArray) {
//     size_t length = strlen(hexString);
//     for (size_t i = 0; i < length; i += 2) {
//         sscanf(&hexString[i], "%2hhx", &byteArray[i / 2]);
//     }
// }

// void stringToWordArray(const char* hexString, u32* wordArray) {
//     size_t length = strlen(hexString);
//     for (size_t i = 0; i < length; i += 8) {
//         sscanf(&hexString[i], "%8x", &wordArray[i / 8]);
//     }
// }

// void stringToWordArray(const char* hexString, u32* wordArray) {
//     size_t length = strlen(hexString);

//     for (size_t i = 0; i < length; i += 8) {
//         u32 temp;
//         sscanf(&hexString[i], "%8x", &temp);

//         // Reorder the bytes
//         wordArray[i / 8] = ((temp & 0x000000FF) << 0x18) |
//                            ((temp & 0x0000FF00) << 0x08) |
//                            ((temp & 0x00FF0000) >> 0x08) |
//                            ((temp & 0xFF000000) >> 0x18);
//     }
// }

// Converts a word array back to a hexadecimal string.
// void wordArrayToString(const u32* wordArray, char* hexString, size_t wordCount) {
//     for (size_t i = 0; i < wordCount; ++i) {
//         // Reverse the byte order
//         u32 temp = wordArray[i];
//         u32 reversed = ((temp & 0x000000FF) << 0x18) |
//                        ((temp & 0x0000FF00) << 0x08) |
//                        ((temp & 0x00FF0000) >> 0x08) |
//                        ((temp & 0xFF000000) >> 0x18);

//         // Convert each word back to a hexadecimal string
//         sprintf(&hexString[i * 8], "%08x", reversed);
//     }
// }

// // Example usage
// int main() {
//     const char* originalString = "0f1e2d3c4b5a69788796a5b4c3d2e1f0";
//     u32 wordArray[8]; // Assuming the original string is always 32 characters (8 words)
//     stringToWordArray(originalString, wordArray);

//     char hexString[33]; // 32 characters + null terminator
//     wordArrayToString(wordArray, hexString, 8);
//     hexString[32] = '\0'; // Null-terminate the string

//     printf("Reconstructed String: %s\n", hexString);
//     return 0;
// }

// void RANDOM_KEY_GENERATION(u32* key) {
//     srand((u32)time(NULL));
    
//     // Initialize pointer to the start of the key array
//     u8* p = key;

//     // Set the counter to 16 bytes
//     int cnt = 0;

//     // Loop until all 16 bytes are filled
//     while (cnt < 4) {
//         *p = rand() & 0xffffffff; // Assign a random byte (0 to 255) to the current location pointed by p
//         p++;                // Move to the next byte
//         cnt++;              // Decrement the byte count
//     }
// }
