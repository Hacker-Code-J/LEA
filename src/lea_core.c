/**
 * @file lea_core.c
 * @brief Core implementation of the LEA encryption algorithm.
 *
 * Implements the core functions for the LEA algorithm including key scheduling,
 * encryption and decryption routines.
 */

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
void leaEncKeySchedule(u32* roundKeys, const u32* key, const int LEA_VERSION) {
    if (LEA_VERSION == 192) {
        const int Nr = 28;
        u32 T[6];

        T[0] = REVERSE_BYTE_ORDER(key[0]);
        T[1] = REVERSE_BYTE_ORDER(key[1]);
        T[2] = REVERSE_BYTE_ORDER(key[2]);
        T[3] = REVERSE_BYTE_ORDER(key[3]);
        T[4] = REVERSE_BYTE_ORDER(key[4]);
        T[5] = REVERSE_BYTE_ORDER(key[5]);

        for (int i = 0; i < Nr; i++) {
            T[0] = ROTL32(T[0] + ROTL32(delta[i % 6], i + 0),  1);
            T[1] = ROTL32(T[1] + ROTL32(delta[i % 6], i + 1),  3);
            T[2] = ROTL32(T[2] + ROTL32(delta[i % 6], i + 2),  6);
            T[3] = ROTL32(T[3] + ROTL32(delta[i % 6], i + 3), 11);
            T[4] = ROTL32(T[4] + ROTL32(delta[i % 6], i + 4), 13);
            T[5] = ROTL32(T[5] + ROTL32(delta[i % 6], i + 5), 17);

            roundKeys[i * 6 + 0] = T[0];
            roundKeys[i * 6 + 1] = T[1];
            roundKeys[i * 6 + 2] = T[2];
            roundKeys[i * 6 + 3] = T[3];
            roundKeys[i * 6 + 4] = T[4];
            roundKeys[i * 6 + 5] = T[5];
        }
    } else if (LEA_VERSION == 256) {
        const int Nr = 32;
        u32 T[8];

        // Initialize T array from key
        // memcpy(T, key, sizeof(u32) * 8);
        T[0] = REVERSE_BYTE_ORDER(key[0]);
        T[1] = REVERSE_BYTE_ORDER(key[1]);
        T[2] = REVERSE_BYTE_ORDER(key[2]);
        T[3] = REVERSE_BYTE_ORDER(key[3]);
        T[4] = REVERSE_BYTE_ORDER(key[4]);
        T[5] = REVERSE_BYTE_ORDER(key[5]);
        T[6] = REVERSE_BYTE_ORDER(key[6]);
        T[7] = REVERSE_BYTE_ORDER(key[7]);

        for (int i = 0; i < Nr; i++) {
            T[(i * 6 + 0) % 8] =
                ROTL32(T[(i * 6 + 0) % 8] + ROTL32(delta[i % 8], i + 0),  1);
            T[(i * 6 + 1) % 8] = 
                ROTL32(T[(i * 6 + 1) % 8] + ROTL32(delta[i % 8], i + 1),  3);
            T[(i * 6 + 2) % 8] = 
                ROTL32(T[(i * 6 + 2) % 8] + ROTL32(delta[i % 8], i + 2),  6);
            T[(i * 6 + 3) % 8] = 
                ROTL32(T[(i * 6 + 3) % 8] + ROTL32(delta[i % 8], i + 3), 11);
            T[(i * 6 + 4) % 8] = 
                ROTL32(T[(i * 6 + 4) % 8] + ROTL32(delta[i % 8], i + 4), 13);
            T[(i * 6 + 5) % 8] = 
                ROTL32(T[(i * 6 + 5) % 8] + ROTL32(delta[i % 8], i + 5), 17);

            roundKeys[i * 6 + 0] = T[(i * 6 + 0) % 8];
            roundKeys[i * 6 + 1] = T[(i * 6 + 1) % 8];
            roundKeys[i * 6 + 2] = T[(i * 6 + 2) % 8];
            roundKeys[i * 6 + 3] = T[(i * 6 + 3) % 8];
            roundKeys[i * 6 + 4] = T[(i * 6 + 4) % 8];
            roundKeys[i * 6 + 5] = T[(i * 6 + 5) % 8];
        }
    } else {
        const int Nr = 24;
        u32 T[4];

        // Load the byte key into T
        // for (int i = 0; i < 4; i++) {
        //     T[i] = (key[i * 4] << 24) | (key[i * 4 + 1] << 16) | (key[i * 4 + 2] << 8) | key[i * 4 + 3];
        // }

        // Load the word key into T
        T[0] = REVERSE_BYTE_ORDER(key[0]);
        T[1] = REVERSE_BYTE_ORDER(key[1]);
        T[2] = REVERSE_BYTE_ORDER(key[2]);
        T[3] = REVERSE_BYTE_ORDER(key[3]);

        // int rounds = (keySize == 128) ? 24 : (keySize == 192) ? 28 : 32;

        // Generate round keys
        for (int i = 0; i < Nr; i++) {
            T[0] = ROTL32(T[0] + ROTL32(delta[i % 4], i + 0),  1);
            T[1] = ROTL32(T[1] + ROTL32(delta[i % 4], i + 1),  3);
            T[2] = ROTL32(T[2] + ROTL32(delta[i % 4], i + 2),  6);
            T[3] = ROTL32(T[3] + ROTL32(delta[i % 4], i + 3), 11);

            roundKeys[i * 6 + 0] = T[0];
            roundKeys[i * 6 + 1] = T[1];
            roundKeys[i * 6 + 2] = T[2];
            roundKeys[i * 6 + 3] = T[1];
            roundKeys[i * 6 + 4] = T[3];
            roundKeys[i * 6 + 5] = T[1];
        }
    }
}

void leaDecKeySchedule(u32* roundKeys, const u32* key, const int LEA_VERSION) {
    if (LEA_VERSION == 192) {
        const int Nr = 28;
        u32 T[6];

        T[0] = REVERSE_BYTE_ORDER(key[0]);
        T[1] = REVERSE_BYTE_ORDER(key[1]);
        T[2] = REVERSE_BYTE_ORDER(key[2]);
        T[3] = REVERSE_BYTE_ORDER(key[3]);
        T[4] = REVERSE_BYTE_ORDER(key[4]);
        T[5] = REVERSE_BYTE_ORDER(key[5]);

        for (int i = 0; i < Nr; i++) {
            T[0] = ROTL32(T[0] + ROTL32(delta[i % 6], i + 0),  1);
            T[1] = ROTL32(T[1] + ROTL32(delta[i % 6], i + 1),  3);
            T[2] = ROTL32(T[2] + ROTL32(delta[i % 6], i + 2),  6);
            T[3] = ROTL32(T[3] + ROTL32(delta[i % 6], i + 3), 11);
            T[4] = ROTL32(T[4] + ROTL32(delta[i % 6], i + 4), 13);
            T[5] = ROTL32(T[5] + ROTL32(delta[i % 6], i + 5), 17);

            roundKeys[(Nr - 1 - i) * 6 + 0] = T[0];
            roundKeys[(Nr - 1 - i) * 6 + 1] = T[1];
            roundKeys[(Nr - 1 - i) * 6 + 2] = T[2];
            roundKeys[(Nr - 1 - i) * 6 + 3] = T[3];
            roundKeys[(Nr - 1 - i) * 6 + 4] = T[4];
            roundKeys[(Nr - 1 - i) * 6 + 5] = T[5];
        }
    } else if (LEA_VERSION == 256) {    
        const int Nr = 32;
        u32 T[8];

        // Initialize T array from key
        // memcpy(T, key, sizeof(u32) * 8);
        T[0] = REVERSE_BYTE_ORDER(key[0]);
        T[1] = REVERSE_BYTE_ORDER(key[1]);
        T[2] = REVERSE_BYTE_ORDER(key[2]);
        T[3] = REVERSE_BYTE_ORDER(key[3]);
        T[4] = REVERSE_BYTE_ORDER(key[4]);
        T[5] = REVERSE_BYTE_ORDER(key[5]);
        T[6] = REVERSE_BYTE_ORDER(key[6]);
        T[7] = REVERSE_BYTE_ORDER(key[7]);

        for (int i = 0; i < Nr; i++) {
            T[(i * 6 + 0) % 8] =
                ROTL32(T[(i * 6 + 0) % 8] + ROTL32(delta[i % 8], i + 0),  1);
            T[(i * 6 + 1) % 8] = 
                ROTL32(T[(i * 6 + 1) % 8] + ROTL32(delta[i % 8], i + 1),  3);
            T[(i * 6 + 2) % 8] = 
                ROTL32(T[(i * 6 + 2) % 8] + ROTL32(delta[i % 8], i + 2),  6);
            T[(i * 6 + 3) % 8] = 
                ROTL32(T[(i * 6 + 3) % 8] + ROTL32(delta[i % 8], i + 3), 11);
            T[(i * 6 + 4) % 8] = 
                ROTL32(T[(i * 6 + 4) % 8] + ROTL32(delta[i % 8], i + 4), 13);
            T[(i * 6 + 5) % 8] = 
                ROTL32(T[(i * 6 + 5) % 8] + ROTL32(delta[i % 8], i + 5), 17);

            roundKeys[(Nr - 1 - i) * 6 + 0] = T[(i * 6 + 0) % 8];
            roundKeys[(Nr - 1 - i) * 6 + 1] = T[(i * 6 + 1) % 8];
            roundKeys[(Nr - 1 - i) * 6 + 2] = T[(i * 6 + 2) % 8];
            roundKeys[(Nr - 1 - i) * 6 + 3] = T[(i * 6 + 3) % 8];
            roundKeys[(Nr - 1 - i) * 6 + 4] = T[(i * 6 + 4) % 8];
            roundKeys[(Nr - 1 - i) * 6 + 5] = T[(i * 6 + 5) % 8];
        }
    } else {
        const int Nr = 24;
        u32 T[4];

        // Load the word key into T
        T[0] = REVERSE_BYTE_ORDER(key[0]);
        T[1] = REVERSE_BYTE_ORDER(key[1]);
        T[2] = REVERSE_BYTE_ORDER(key[2]);
        T[3] = REVERSE_BYTE_ORDER(key[3]);

        // Generate round keys
        for (int i = 0; i < Nr; i++) {
            T[0] = ROTL32(T[0] + ROTL32(delta[i % 4], i + 0), 1);
            T[1] = ROTL32(T[1] + ROTL32(delta[i % 4], i + 1), 3);
            T[2] = ROTL32(T[2] + ROTL32(delta[i % 4], i + 2), 6);
            T[3] = ROTL32(T[3] + ROTL32(delta[i % 4], i + 3), 11);

            roundKeys[(Nr - 1 - i) * 6 + 0] = T[0];
            roundKeys[(Nr - 1 - i) * 6 + 1] = T[1];
            roundKeys[(Nr - 1 - i) * 6 + 2] = T[2];
            roundKeys[(Nr - 1 - i) * 6 + 3] = T[1];
            roundKeys[(Nr - 1 - i) * 6 + 4] = T[3];
            roundKeys[(Nr - 1 - i) * 6 + 5] = T[1];
        }
    }
}

void leaEncrypt(u32* dst, const u32* src, const u32* key, const int LEA_VERSION) {
    int Nr, TOTAL_RK;
    switch (LEA_VERSION) {
        case 192:
            Nr = 28;
            TOTAL_RK = 168;
            break;
        case 256:
            Nr = 32;
            TOTAL_RK = 192;
            break;
        default:  // Assuming default is for LEA_VERSION == 128 or any other value
            Nr = 24;
            TOTAL_RK = 144;
            break;
    }

    u32 roundKeys[TOTAL_RK];
    leaEncKeySchedule(roundKeys, key, LEA_VERSION);

    u32 t[4];

    t[0] = REVERSE_BYTE_ORDER(src[0]);
    t[1] = REVERSE_BYTE_ORDER(src[1]);
    t[2] = REVERSE_BYTE_ORDER(src[2]);
    t[3] = REVERSE_BYTE_ORDER(src[3]);

    // printf("\nEncryption Test:\nt[%02d] = %08x:%08x:%08x:%08x\n",
    //             0, t[0], t[1], t[2], t[3]);

    for (int i = 0, j = 0;
         i < Nr;
         i++, j += 6) {
        u32 _t0 = t[0];

        t[0] = ROTL32((t[0] ^ roundKeys[j + 0])
                    + (t[1] ^ roundKeys[j + 1]), 9);
        t[1] = ROTR32((t[1] ^ roundKeys[j + 2])
                    + (t[2] ^ roundKeys[j + 3]), 5);
        t[2] = ROTR32((t[2] ^ roundKeys[j + 4])
                    + (t[3] ^ roundKeys[j + 5]), 3);
        t[3] = _t0;

        // printf("t[%02d] = %08x:%08x:%08x:%08x\n",
        //         i+1, t[0],t[1],t[2],t[3]);
    }

    dst[0] = REVERSE_BYTE_ORDER(t[0]);
    dst[1] = REVERSE_BYTE_ORDER(t[1]);
    dst[2] = REVERSE_BYTE_ORDER(t[2]);
    dst[3] = REVERSE_BYTE_ORDER(t[3]);
}

void leaDecrypt(u32* dst, const u32* src, const u32* key, const int LEA_VERSION) {
    int Nr, TOTAL_RK;
    switch (LEA_VERSION) {
        case 192:
            Nr = 28;
            TOTAL_RK = 168;
            break;
        case 256:
            Nr = 32;
            TOTAL_RK = 192;
            break;
        default:  // Assuming default is for LEA_VERSION == 128 or any other value
            Nr = 24;
            TOTAL_RK = 144;
            break;
    }

    u32 roundKeys[TOTAL_RK];
    leaDecKeySchedule(roundKeys, key, LEA_VERSION);

    u32 t[4];

    t[0] = REVERSE_BYTE_ORDER(src[0]);
    t[1] = REVERSE_BYTE_ORDER(src[1]);
    t[2] = REVERSE_BYTE_ORDER(src[2]);
    t[3] = REVERSE_BYTE_ORDER(src[3]);

    // printf("\nDecryption Test:\nt[%02d] = %08x:%08x:%08x:%08x\n",
    //             0, t[0], t[1], t[2], t[3]);

    for (int i = 0, j = 0;
         i < Nr;
         i++, j += 6) {
        
        u32 _t0 = t[0];
        u32 _t1 = t[1];
        u32 _t2 = t[2];

        t[0] = t[3];
        t[1] = (ROTR32(_t0, 9) - (t[0] ^ roundKeys[j + 0]))
            ^ roundKeys[j + 1];
        t[2] = (ROTL32(_t1, 5) - (t[1] ^ roundKeys[j + 2]))
            ^ roundKeys[j + 3];
        t[3] = (ROTL32(_t2, 3) - (t[2] ^ roundKeys[j + 4]))
            ^ roundKeys[j + 5];

        // printf("t[%02d] = %08x:%08x:%08x:%08x\n",
        //         i + 1, t[0], t[1], t[2], t[3]);
    }

    dst[0] = REVERSE_BYTE_ORDER(t[0]);
    dst[1] = REVERSE_BYTE_ORDER(t[1]);
    dst[2] = REVERSE_BYTE_ORDER(t[2]);
    dst[3] = REVERSE_BYTE_ORDER(t[3]);    
}

// Other functions...