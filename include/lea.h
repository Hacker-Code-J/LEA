/**
 * @file lea.h
 * @brief Header file for LEA (Lightweight Encryption Algorithm) implementation.
 * 
 * This file contains the prototypes and definitions required for the LEA
 * encryption and decryption functionalities, as well as key scheduling.
 */
#include "lea_config.h"

#ifndef _LEA_H
#define _LEA_H

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
void lea192_test(void);
void lea256_test(void);

#endif /* _LEA_H */