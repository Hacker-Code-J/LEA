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

// Constants and type definitions...

/**
 * @brief Function to perform key scheduling in LEA.
 * 
 * @param key The encryption key.
 * @param roundKeys The array to store the generated round keys.
 */
void LEA_KeySchedule(const uint8_t *key, uint32_t *roundKeys);

// Other function prototypes...

#endif // _LEA_H