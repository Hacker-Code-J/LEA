/**
 * @file lea_modes.h
 * @brief Header file for defining the modes of operation in the LEA encryption algorithm
 * 
 */

#include <stdio.h>

#include "lea.h"

#ifndef _LEA_MODES_H
#define _LEA_MODES_H

void PKCS7_BYTE_PAD_32bit(u32* block, size_t block_len_bytes, size_t input_len_bytes);

void CBC_Encrypt_LEA(u32* restrict ct, 
                     const u32* restrict pt, const size_t dataLength,
                     const u32* restrict key, const size_t keyLength,
                     const u32* restrict iv);

void CBC_Decrypt_LEA(u32* restrict pt, 
                     const u32* restrict ct, const size_t dataLength,
                     const u32* restrict key, const size_t keyLength,
                     const u32* restrict iv);

void CBC_lea128_test(void);
void CBC_lea192_test(void);
void CBC_lea256_test(void);

void CTR_Encrypt_LEA(u32* restrict ct, 
                     const u32* restrict pt, const size_t dataLength,
                     const u32* restrict key, const size_t keyLength,
                     const u32* restrict ctr);

void CTR_Decrypt_LEA(u32* restrict pt, 
                     const u32* restrict ct, const size_t dataLength,
                     const u32* restrict key, const size_t keyLength,
                     const u32* restrict ctr);

void CTR_lea128_test(void);
void CTR_lea192_test(void);
void CTR_lea256_test(void);



// void xorBlocks(u32* dst, const u32* src1, const u32* src2);

// void ECB_Encrypt_LEA(u32* ciphertext, const u32* plaintext, size_t plaintext_len, const u32* key);
// void ECB_Decrypt_LEA(u32* plaintext, const u32* ciphertext, size_t ciphertext_len, const u32* key);

// void CBC_Encrypt_LEA(u32* ciphertext, const u32* plaintext, size_t length, 
//                 const u32* key, const u32* iv);
// void CBC_Decrypt_LEA(u32* plaintext, const u32* ciphertext, size_t length, 
//                 const u32* key, const u32* iv);

// void CBC_Encrypt_LEA(u32* ciphertext, const u32* plaintext, size_t plaintext_len, const u32* key, const u32* iv);
// void CBC_Decrypt_LEA(u32* plaintext, const u32* ciphertext, size_t ciphertext_len, const u32* key, const u32* iv);

// void xorBlocks(const u32* src1, const u32* src2, u32* dst, size_t blockSize);

// void ecbEncrypt(const u32* plaintext, const u32* roundKeys, u32* ciphertext, size_t blockSize);
// void ecbDecrypt(const u32* ciphertext, const u32* roundKeys, u32* plaintext, size_t blockSize);

// void cbcEncrypt(const u32* plaintext, const u32* roundKeys, u32* ciphertext, size_t blockSize, const u32* iv);
// void cbcDecrypt(const u32* ciphertext, const u32* roundKeys, u32* plaintext, size_t blockSize, const u32* iv);

// void cfbEncrypt(const u32* plaintext, const u32* roundKeys, u32* ciphertext, size_t blockSize, u32* iv);
// void cfbDecrypt(const u32* ciphertext, const u32* roundKeys, u32* plaintext, size_t blockSize, u32* iv);

// void ofbCrypt(const u32* input, const u32* roundKeys, u32* output, size_t blockSize, u32* iv);

// void ctrCrypt(const u32* input, const u32* roundKeys, u32* output, size_t blockSize, u32* counter);

#endif /* _LEA_MODES_H */