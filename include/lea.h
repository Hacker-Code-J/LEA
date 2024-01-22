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

#define LEA_V 128

#define LEA128 128
#define LEA192 192
#define LEA256 256

#define BLOCK_SIZE 16 // 16-byte (128-bit) block
// #define BLOCK_SIZE 4 // Assuming a block size of 4 u32 values (128 bits)

// #if LEA_V == 192
// #define KEY_SIZE 6
// #define Nr 28
// #define TOTAL_RK 168
// #elif LEA_V == 256
// #define KEY_SIZE 8
// #define Nr 32
// #define TOTAL_RK 192
// #else
// #define KEY_SIZE 4
// #define Nr 24
// #define TOTAL_RK 144
// #endif

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

extern const u32 delta[8];

static inline u32 ROTL32(u32 val, u32 shift) {
    return (val << shift) | (val >> (32 - shift));
}

static inline u32 ROTR32(u32 val, u32 shift) {
    return (val >> shift) | (val << (32 - shift));
}

static inline u32 REVERSE_BYTE_ORDER(u32 word) {
    return ((word & 0x000000FF) << 0x18) |
           ((word & 0x0000FF00) << 0x08)  |
           ((word & 0x00FF0000) >> 0x08)  |
           ((word & 0xFF000000) >> 0x18);
}

/**
 * Reads the current value of the processor's time-stamp counter.
 * 
 * The time-stamp counter is a 64-bit register present on all x86 processors
 * since the Pentium. It counts the number of cycles since reset. The `rdtsc`
 * instruction is used to read this value.
 * 
 * Note that the counter's resolution and the relation between its value and
 * actual time may vary between processor models and power/thermal conditions.
 * 
 * @return The current value of the time-stamp counter.
 */
static inline u64 rdtsc() {
    u32 lo, hi;
    
    // Inline assembly syntax for GCC. This tells the compiler to emit the rdtsc instruction,
    // which stores the result in two 32-bit registers: the lower 32 bits in 'eax' (mapped to 'lo' in C),
    // and the higher 32 bits in 'edx' (mapped to 'hi' in C).
    __asm__ __volatile__ (
      "rdtsc" : "=a" (lo), "=d" (hi)
    );
    
    // Combine the high and low 32-bit values into a single 64-bit integer.
    // The high value is shifted left by 32 bits and then combined with the low value.
    return ((u64)hi << 32) | lo;
}

/* LEA_CORE */

/**
 * @brief Function to perform encryption key scheduling in LEA.
 * 
 * @param key The encryption key.
 * @param roundKeys The array to store the generated round keys.
 */
void leaEncKeySchedule(u32* roundKeys, const u32* key, const int LEA_VERSION);

/**
 * @brief Function to perform decryption key scheduling in LEA.
 * 
 * @param key The decryption key.
 * @param roundKeys The array to store the generated round keys.
 */
void leaDecKeySchedule(u32* roundKeys, const u32* key, const int LEA_VERSION);

void leaEncrypt(u32* dst, const u32* src, const u32* key, const int LEA_VERSION);
void leaDecrypt(u32* dst, const u32* src, const u32* key, const int LEA_VERSION);

/* LEA_UTILS */

void stringToWordArray(u32* wordArray, const char* hexString);

void printBigEndian(u32* array, size_t size); // User Array
void printLittleEndian(u32* array, size_t size); // Real Virtual Memory

void printEncRoundKeys(u32* enc_roundkey, const int LEA_VERSION);
void printDecRoundKeys(u32* dec_roundkey, const int LEA_VERSION);

double measure_time(void (*func)(u32*, const u32*, const u32*), u32* dst, const u32* src, const u32* key);

// void keyToWordArray(u32* wordArray, const char* hexString);
// void RANDOM_KEY_GENERATION(u32* key);
// void stringToByteArray(const char* hexString, u8* byteArray);

/* LEA_TESTS */
void lea128_test(void);
// void lea192_test();
// void lea256_test();

/* LEA_MODE_TESTS */

void lea128_CBC_test();
void lea192_CBC_test();
void lea256_CBC_test();

void lea128_CTR_test();
void lea192_CTR_test();
void lea256_CTR_test();

#endif /* _LEA_H */