/**
 * @file lea.h
 * @brief Header file for LEA (Lightweight Encryption Algorithm) implementation.
 * 
 * This file contains the prototypes and definitions required for the LEA
 * encryption and decryption functionalities, as well as key scheduling.
 */

#include <stdint.h>

#ifndef _LEA_H
#define _LEA_H

#define LEA_VERSION 128

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
void leaEncKeySchedule(const u32* key, u32* roundKeys);

/**
 * @brief Function to perform decryption key scheduling in LEA.
 * 
 * @param key The decryption key.
 * @param roundKeys The array to store the generated round keys.
 */
void leaDecKeySchedule(const u32* key, u32* roundKeys);

void leaEncrypt(const u32* src, const u32* roundKeys, u32* dst);
void leaDecrypt(const u32* src, const u32* roundKeys, u32* dst);

/* LEA_UTILS */

void stringToWordArray(const char* hexString, u32* wordArray);

void printBigEndian(u32* array, size_t size);
void printLittleEndian(u32* array, size_t size);

void printEncRoundKeys(u32* enc_roundkey);
void printEncRoundKeys(u32* dec_roundkey);

double measure_time(void (*func)(const u32*, const u32*, u32*), const u32* src, const u32* key, u32* dst);

// void RANDOM_KEY_GENERATION(u32* key);
// void stringToByteArray(const char* hexString, u8* byteArray);

#endif // _LEA_H