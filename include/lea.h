/**
 * @file lea.h
 * @brief Header file for LEA (Lightweight Encryption Algorithm) implementation.
 * 
 * This file contains the prototypes and definitions required for the LEA
 * encryption and decryption functionalities, as well as key scheduling.
 */

#ifndef _LEA_H
#define _LEA_H

#include <stdint.h>

#define LEA_VERSION 128
#define Nr 24

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

extern const u32 delta[8];

#define ROTL32(val, shift) (((val) << (shift)) | ((val) >> (32 - (shift))))
#define ROTR32(val, shift) (((val) >> (shift)) | ((val) << (32 - (shift))))

/* LEA_CORE */

/**
 * @brief Function to perform key scheduling in LEA.
 * 
 * @param key The encryption key.
 * @param roundKeys The array to store the generated round keys.
 */
void leaEncKeySchedule(const u32* key, u32* roundKeys);
void leaDecSchedule(const u8* key, u32* roundKeys);
void leaEncrypt(const u8* input, const u32* roundKeys, u8* output);
void leaDecrypt(const u8* input, u32* output);

/* LEA_UTILS */

void RANDOM_KEY_GENERATION(u32* key);
void stringToWordArray(const char* hexString, u32* wordArray);
char* string_to_hex(const char* input);

#endif // _LEA_H