/**
 * @file lea_core.c
 * @brief Core implementation of the LEA encryption algorithm.
 *
 * Implements the core functions for the LEA algorithm including key scheduling,
 * encryption and decryption routines.
 */
#include <stdio.h>
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
void leaEncKeySchedule(const u32* key, u32* roundKeys, const int keySize) {
    u32 T[4];

    // Load the byte key into T
    // for (int i = 0; i < 4; i++) {
    //     T[i] = (key[i * 4] << 24) | (key[i * 4 + 1] << 16) | (key[i * 4 + 2] << 8) | key[i * 4 + 3];
    // }
    T[0] = key[0];
    T[1] = key[1];
    T[2] = key[2];
    T[3] = key[3];

    int rounds = (keySize == 128) ? 24 : (keySize == 192) ? 28 : 32;

    // Generate round keys
    for (int i = 0; i < rounds; i++) {
        T[0] = ROTL32(T[0] + ROTL32(delta[i % 4], i), 1);
        T[1] = ROTL32(T[1] + ROTL32(delta[i % 4], i + 1), 3);
        T[2] = ROTL32(T[2] + ROTL32(delta[i % 4], i + 2), 6);
        T[3] = ROTL32(T[3] + ROTL32(delta[i % 4], i + 3), 11);

        roundKeys[i * 6] = T[0];
        roundKeys[i * 6 + 1] = T[1];
        roundKeys[i * 6 + 2] = T[2];
        roundKeys[i * 6 + 3] = T[0];
        roundKeys[i * 6 + 4] = T[3];
        roundKeys[i * 6 + 5] = T[1];
    }
    
    // u32 T[4];

    // // Copy the original key into T
    // // for (i = 0; i < keySize/32; i++) {
    // //     T[i] = key[i];
    // // }
    // T[0] = key[0];
    // T[1] = key[1];
    // T[2] = key[2];
    // T[3] = key[3];

    // // Number of rounds depends on key size
    // int rounds = (keySize == 128) ? 24 : (keySize == 192) ? 28 : 32;

    // // Generate round keys
    // for (int i = 0; i < rounds; i++) {
    //     T[0] = ROTL32(T[0] + ROTL32(delta[i % 4], i + 0),  1);
    //     T[1] = ROTL32(T[1] + ROTL32(delta[i % 4], i + 1),  3);
    //     T[2] = ROTL32(T[2] + ROTL32(delta[i % 4], i + 2),  6);
    //     T[3] = ROTL32(T[3] + ROTL32(delta[i % 4], i + 3), 11);

    //     roundKeys[6 * i + 0] = T[1];
    //     roundKeys[6 * i + 1] = T[3];
    //     roundKeys[6 * i + 2] = T[1];
    //     roundKeys[6 * i + 3] = T[2];
    //     roundKeys[6 * i + 4] = T[1];
    //     roundKeys[6 * i + 5] = T[0];
    // }
    
    // for (int i = 0; i < 24; i++) {
    //     T[0] = ROTL32(T[0] + ROTL32(delta[i % 4], i+0),  1);
    //     T[0] = ROTL32(T[0] + ROTL32(delta[i % 4], i+1),  3);
    //     T[0] = ROTL32(T[0] + ROTL32(delta[i % 4], i+2),  6);
    //     T[0] = ROTL32(T[0] + ROTL32(delta[i % 4], i+3), 11);

    //     roundKeys[0] = T[0];
    //     roundKeys[1] = T[1];
    //     roundKeys[2] = T[2];
    //     roundKeys[3] = T[1];
    //     roundKeys[4] = T[3];
    //     roundKeys[5] = T[1];
    // }
    
    // u32 T[4] = { 0x00, }; // 128-bit
    // int i = 0; int j = 0;
    
    // // T: 128-bit
    // // key: 128-bit
    // memcpy(T, key, 4 * sizeof(u32));
    
    // for (i = 0; i < 4; i++)
    //     printf("%08x\n", T[i]);

    // for (i = 0; i < 24; i++) {
    //     printf("ROTL32(delta[%d], %d) = %08x\n", i%4, i+1, ROTL32(delta[i % 4], i+0));
    //     printf("%08x + ROTL32(delta[%d], %d) = %08x\n", T[3],i%4, i+1, T[3]+ROTL32(delta[i % 4], i+0));
    //     printf("%08x + ROTL32(delta[%d], %d) = %08x\n", T[0],i%4, i+1, T[0]+ROTL32(delta[i % 4], i+0));
    //     T[0] = ROTL32(T[0] + ROTL32(delta[i % 4], i+0),  1);
    //     T[1] = ROTL32(T[1] + ROTL32(delta[i % 4], i+1),  3);
    //     T[2] = ROTL32(T[2] + ROTL32(delta[i % 4], i+2),  6);
    //     T[3] = ROTL32(T[3] + ROTL32(delta[i % 4], i+3), 11);
    //     printf("T[0]: %08x\n", T[0]);
    //     printf("T[1]: %08x\n", T[1]);
    //     printf("T[2]: %08x\n", T[2]);
    //     printf("T[3]: %08x\n", T[3]);
    //     // roundKeys[j++] = T[0];
    //     // roundKeys[j++] = T[1];
    //     // roundKeys[j++] = T[2];
    //     // roundKeys[j++] = T[1];
    //     // roundKeys[j++] = T[3];
    //     // roundKeys[j++] = T[1];
    // }
}

// void leaDecSchedule(const u8* key, u32* roundKeys) {

// }
// void leaEncrypt(const u8* input, const u32* roundKeys, u8* output) {

// }
// void leaDecrypt(const u8* input, u32* output) {

// }

// Other functions...