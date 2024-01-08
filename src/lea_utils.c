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

void stringToWordArray(const char* hexString, u32* wordArray) {
    size_t length = strlen(hexString);
    for (size_t i = 0; i < length; i += 8) {
        sscanf(&hexString[i], "%8x", &wordArray[i / 8]);
    }
}

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
