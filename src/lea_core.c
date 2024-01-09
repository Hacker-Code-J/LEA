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
 * @param key Pointer to the src key.
 * @param roundKeys Pointer to the array where round keys will be stored.
*/
void leaEncKeySchedule(const u32* key, u32* roundKeys) {
    u32 T[4];

    // Load the byte key into T
    // for (int i = 0; i < 4; i++) {
    //     T[i] = (key[i * 4] << 24) | (key[i * 4 + 1] << 16) | (key[i * 4 + 2] << 8) | key[i * 4 + 3];
    // }

    // Load the word key into T
    T[0] = key[0];
    T[1] = key[1];
    T[2] = key[2];
    T[3] = key[3];

    // int rounds = (keySize == 128) ? 24 : (keySize == 192) ? 28 : 32;

    // Generate round keys
    for (int i = 0; i < Nr; i++) {
        T[0] = ROTL32(T[0] + ROTL32(delta[i % 4], i), 1);
        T[1] = ROTL32(T[1] + ROTL32(delta[i % 4], i + 1), 3);
        T[2] = ROTL32(T[2] + ROTL32(delta[i % 4], i + 2), 6);
        T[3] = ROTL32(T[3] + ROTL32(delta[i % 4], i + 3), 11);

        roundKeys[i * 6] = T[0];
        roundKeys[i * 6 + 1] = T[1];
        roundKeys[i * 6 + 2] = T[2];
        roundKeys[i * 6 + 3] = T[1];
        roundKeys[i * 6 + 4] = T[3];
        roundKeys[i * 6 + 5] = T[1];
    }
}

void leaDecSchedule(const u32* key, u32* roundKeys) {
    u32 T[4];

    // Load the word key into T
    T[0] = key[0];
    T[1] = key[1];
    T[2] = key[2];
    T[3] = key[3];

    // int rounds = (keySize == 128) ? 24 : (keySize == 192) ? 28 : 32;

    // Generate round keys
    for (int i = Nr-1; i <= 0; i--) {
        T[0] = ROTL32(T[0] + ROTL32(delta[i % 4], i), 1);
        T[1] = ROTL32(T[1] + ROTL32(delta[i % 4], i + 1), 3);
        T[2] = ROTL32(T[2] + ROTL32(delta[i % 4], i + 2), 6);
        T[3] = ROTL32(T[3] + ROTL32(delta[i % 4], i + 3), 11);

        roundKeys[i * 6] = T[0];
        roundKeys[i * 6 + 1] = T[1];
        roundKeys[i * 6 + 2] = T[2];
        roundKeys[i * 6 + 3] = T[1];
        roundKeys[i * 6 + 4] = T[3];
        roundKeys[i * 6 + 5] = T[1];
    } 
}

void leaEncrypt(const u32* src, const u32* roundKeys, u32* dst) {
    u32 t[4];

    t[0] = src[0];
    t[1] = src[1];
    t[2] = src[2];
    t[3] = src[3];

    printf("\nEncryption Test:\nt[%02d] = %08x:%08x:%08x:%08x\n",
                0, t[0],t[1],t[2],t[3]);

    for (int i = 0, j = 0; i < Nr; i++, j += 6) {
        u32 _t0 = t[0];
        t[0] = t[3];

        t[0] = ROTL32((t[0] ^ roundKeys[j + 0])
                    + (t[1] ^ roundKeys[j + 1]), 9);
        t[1] = ROTR32((t[1] ^ roundKeys[j + 2])
                    + (t[2] ^ roundKeys[j + 3]), 5);
        t[2] = ROTR32((t[2] ^ roundKeys[j + 4])
                    + (t[3] ^ roundKeys[j + 5]), 3);
        t[3] = _t0;

        printf("t[%02d] = %08x:%08x:%08x:%08x\n",
                i+1, t[0],t[1],t[2],t[3]);
    }

    dst[0] = t[0];
    dst[1] = t[1];
    dst[2] = t[2];
    dst[3] = t[3];
}


void leaDecrypt(const u32* src, const u32* roundKeys, u32* dst) {
    u32 t[4];

    t[0] = src[0];
    t[1] = src[1];
    t[2] = src[2];
    t[3] = src[3];

    printf("\nEncryption Test:\nt[%02d] = %08x:%08x:%08x:%08x\n",
                0, t[0],t[1],t[2],t[3]);

    for (int i = 0, j = 0; i < Nr; i++, j += 6) {
        u32 _t0 = t[0];

        t[0] = t[3];
        t[1] = (ROTR32(_t0, 9) - (_t0 ^ roundKeys[j + 0]))
            ^ roundKeys[j +1];
        t[2] = (ROTL32(_t0, 9) - (_t0 ^ roundKeys[j + 0]))
            ^ roundKeys[j +1];
        t[3] = (ROTL32(_t0, 9) - (_t0 ^ roundKeys[j + 0]))
            ^ roundKeys[j +1];

        t[0] = ROTL32((t[0] ^ roundKeys[j + 0])
                    + (t[1] ^ roundKeys[j + 1]), 9);
        t[1] = ROTR32((t[1] ^ roundKeys[j + 2])
                    + (t[2] ^ roundKeys[j + 3]), 5);
        t[2] = ROTR32((t[2] ^ roundKeys[j + 4])
                    + (t[3] ^ roundKeys[j + 5]), 3);
        t[3] = _t0;

        printf("t[%02d] = %08x:%08x:%08x:%08x\n",
                i+1, t[0],t[1],t[2],t[3]);
    }

    dst[0] = t[0];
    dst[1] = t[1];
    dst[2] = t[2];
    dst[3] = t[3];    
}

// Other functions...