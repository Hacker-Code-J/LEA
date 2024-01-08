/**
 * @file lea_core.c
 * @brief Core implementation of the LEA encryption algorithm.
 *
 * Implements the core functions for the LEA algorithm including key scheduling,
 * encryption and decryption routines.
 */
#include <string.h>
#include "lea.h"

const u32 delta[8] = {
    0xc3efe9dbU, 0x44626b02U, 0x79e27c8aU, 0x78df30ecU,
    0x715ea49eU, 0xc785da0aU, 0xe04ef22aU, 0xe5c40957U
};

/**
 * @brief Implements the key scheduling algorithm of LEA.
 * 
 * Detailed description of the key scheduling algorithm...
 * 
 * @param key Pointer to the input key.
 * @param roundKeys Pointer to the array where round keys will be stored.
 */
void leaEncKeySchedule(const u32* key, u32* roundKeys) {
    u32 T[4] = { 0x00, }; // 128-bit

    // T: 128-bit
    // key: 128-bit
    memcpy(T, key, 4);
    
    u32 idx;
    for (idx = 0; idx < 24; idx++) {
        T[0] = ROTL32(T[0] + ROTL32(delta[idx % 4], idx+0),  1);
        T[1] = ROTL32(T[1] + ROTL32(delta[idx % 4], idx+1),  3);
        T[2] = ROTL32(T[2] + ROTL32(delta[idx % 4], idx+2),  6);
        T[3] = ROTL32(T[3] + ROTL32(delta[idx % 4], idx+3), 11);
        u32 idx2 = 0;
        *(roundKeys + idx2 + 0) = *(T+0);
        *(roundKeys + idx2 + 1) = *(T+1);
        *(roundKeys + idx2 + 2) = *(T+2);
        *(roundKeys + idx2 + 3) = *(T+1);
        *(roundKeys + idx2 + 4) = *(T+3);
        *(roundKeys + idx2 + 5) = *(T+1);
        idx2 += 6;
    }
}

// void leaDecSchedule(const u8* key, u32* roundKeys) {

// }
// void leaEncrypt(const u8* input, const u32* roundKeys, u8* output) {

// }
// void leaDecrypt(const u8* input, u32* output) {

// }

// Other functions...