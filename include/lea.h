/**
 * @file lea.h
 * @brief Header file for LEA (Lightweight Encryption Algorithm) implementation.
 * 
 * This file contains the prototypes and definitions required for the LEA
 * encryption and decryption functionalities, as well as key scheduling.
 */

#include <stdint.h>
#include <stddef.h>

#ifndef _LEA_H
#define _LEA_H

#define LEA_VERSION 128

#define BLOCK_SIZE 16 // 16-byte (128-bit) block

#if LEA_VERSION == 192
#define KEY_SIZE 6
#define Nr 28
#define TOTAL_RK 168
#elif LEA_VERSION == 256
#define KEY_SIZE 8
#define Nr 32
#define TOTAL_RK 192
#else
#define KEY_SIZE 4
#define Nr 24
#define TOTAL_RK 144
#endif

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

extern const u32 delta[8];

#define ROTL32(val, shift) (((val) << (shift)) | ((val) >> (32 - (shift))))
#define ROTR32(val, shift) (((val) >> (shift)) | ((val) << (32 - (shift))))
#define REVERSE_BYTE_ORDER(word) (((word & 0x000000FF) << 0x18) | \
                                  ((word & 0x0000FF00) << 0x08) | \
                                  ((word & 0x00FF0000) >> 0x08) | \
                                  ((word & 0xFF000000) >> 0x18))

/* LEA_CORE */

/**
 * @brief Function to perform encryption key scheduling in LEA.
 * 
 * @param key The encryption key.
 * @param roundKeys The array to store the generated round keys.
 */
void leaEncKeySchedule(u32* roundKeys, const u32* key);

/**
 * @brief Function to perform decryption key scheduling in LEA.
 * 
 * @param key The decryption key.
 * @param roundKeys The array to store the generated round keys.
 */
void leaDecKeySchedule(u32* roundKeys, const u32* key);

void leaEncrypt(u32* dst, const u32* src, const u32* roundKeys);
void leaDecrypt(u32* dst, const u32* src, const u32* roundKeys);

/* LEA_UTILS */

void stringToWordArray(u32* wordArray, const char* hexString);

void printBigEndian(u32* array, size_t size); // User Array
void printLittleEndian(u32* array, size_t size); // Real Virtual Memory

void printEncRoundKeys(u32* enc_roundkey);
void printDecRoundKeys(u32* dec_roundkey);

double measure_time(void (*func)(u32*, const u32*, const u32*), u32* dst, const u32* src, const u32* key);

// void keyToWordArray(u32* wordArray, const char* hexString);
// void RANDOM_KEY_GENERATION(u32* key);
// void stringToByteArray(const char* hexString, u8* byteArray);

/* LEA_TESTS */
void lea128_test();
void lea192_test();
void lea256_test();

/* LEA_MODE_TESTS */
void lea128_ECB_test();
void lea192_ECB_test();
void lea256_ECB_test();

void lea128_CBC_test();
void lea192_CBC_test();
void lea256_CBC_test();

void lea128_CTR_test();
void lea192_CTR_test();
void lea256_CTR_test();

#endif // _LEA_H