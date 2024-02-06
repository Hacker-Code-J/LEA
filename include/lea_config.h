/**
 * @file lea_config.h
 * @brief Header file containing configuration settings and options for the Lightweight Encryption Algorithm (LEA) implementation.
 * 
 */

#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>

#ifndef _LEA_CONFIG_H
#define _LEA_CONFIG_H

typedef int8_t i8;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

#define LEA128 128
#define LEA192 192
#define LEA256 256

#define BLOCK_SIZE 16 // 16-byte (128-bit) block

// #define LEA_V 128
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

#endif /* _LEA_CONFIG_H */