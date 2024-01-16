#include <stdlib.h>
#include <string.h>

#include "lea_modes.h"

void PKCS7_BYTE_PAD_32bit(u32* block, size_t block_len_bytes, size_t input_len_bytes) {
    // Calculate the number of 32-bit words (u32) in the block and input lengths
    size_t block_len = block_len_bytes / sizeof(u32);
    size_t input_len = input_len_bytes / sizeof(u32);

    if (block_len < input_len) {
        fprintf(stderr, "Block length must be greater than input length.\n");
        return;
    }

    // Calculate the padding value based on byte lengths, not word lengths
    u8 padding_value = (u8)(block_len_bytes - input_len_bytes);

    // Convert the block pointer to a byte pointer for padding
    u8* byte_block = (u8*)block;

    // Start padding at the end of the used portion of the block
    for (size_t i = input_len_bytes; i < block_len_bytes; ++i) {
        byte_block[i] = padding_value;
    }
}

// Helper function for XORing blocks
void xorBlocks(u32* dst, const u32* src1, const u32* src2) {
    for (int i = 0; i < BLOCK_SIZE/4; i++) {
        dst[i] = src1[i] ^ src2[i];
    }
}

// Assuming block size is fixed (e.g., 128 bits for LEA)
void CBC_Encrypt_LEA(u32* ciphertext, const u32* plaintext, size_t length, 
                const u32* key, const u32* iv) {
    u32 roundKeys[TOTAL_RK]; // Size as per LEA spec
    leaEncKeySchedule(roundKeys, key);

    u32 previousBlock[BLOCK_SIZE/4];
    u32 srcBlock[BLOCK_SIZE/4]; // Temporary block for processing

    memcpy(previousBlock, iv, BLOCK_SIZE);

    for (size_t i = 0; i < length; i += BLOCK_SIZE) {
        xorBlocks(srcBlock, &plaintext[i], previousBlock);
        leaEncrypt(&ciphertext[i], srcBlock, roundKeys);
        memcpy(previousBlock, &ciphertext[i], BLOCK_SIZE);
    }
}

void CBC_Decrypt_LEA(u32* plaintext, const u32* ciphertext, size_t length, 
                const u32* key, const u32* iv) {
    u32 roundKeys[TOTAL_RK]; // Size as per LEA spec
    leaDecKeySchedule(roundKeys, key);

    u32 previousBlock[BLOCK_SIZE/4];
    u32 dstBlock[BLOCK_SIZE/4]; // Temporary block for processing

    memcpy(previousBlock, iv, BLOCK_SIZE);

    for (size_t i = 0; i < length; i += BLOCK_SIZE) {
        leaDecrypt(dstBlock, &ciphertext[i], roundKeys);
        xorBlocks(&plaintext[i], dstBlock, previousBlock);
        memcpy(previousBlock, &ciphertext[i], BLOCK_SIZE);
    }
}

// void xorBlocks(const u32* src1, const u32* src2, u32* dst, size_t blockSize) {
//     for (size_t i = 0; i < blockSize / sizeof(u32); i++) {
//         dst[i] = src1[i] ^ src2[i];
//     }
// }

// void ecbEncrypt(const u32* plaintext, const u32* roundKeys, u32* ciphertext, size_t blockSize) {
//     for (size_t offset = 0; offset < blockSize; offset += sizeof(u32)) {
//         leaEncrypt(&ciphertext[offset / sizeof(u32)], &plaintext[offset / sizeof(u32)], roundKeys);
//     }
// }

// // ECB Encryption
// void ECB_Encrypt_LEA(u32* ciphertext, const u32* plaintext, size_t plaintext_len, const u32* key) {
//     u32 roundKeys[TOTAL_RK];  // Define the size according to LEA specification
//     leaEncKeySchedule(roundKeys, key);

//     // Encrypt each block
//     for (size_t i = 0; i < plaintext_len * sizeof(u32); i += sizeof(u32)) {
//         leaEncrypt(&ciphertext[i], &plaintext[i], roundKeys);
//     }
// }

// // ECB Decryption
// void ECB_Decrypt_LEA(u32* plaintext, const u32* ciphertext, size_t ciphertext_len, const u32* key) {
//     u32 roundKeys[TOTAL_RK];  // Define the size according to LEA specification
//     leaDecKeySchedule(roundKeys, key);

//     // Decrypt each block
//     for (size_t i = 0; i < ciphertext_len * sizeof(u32); i += sizeof(u32)) {
//         leaDecrypt(&plaintext[i], &ciphertext[i], roundKeys);
//     }
// }

// void xorBlocks(u32* dst, const u32* src1, const u32* src2, size_t blockSize) {
//     for (size_t i = 0; i < blockSize / sizeof(u32); i++) {
//         dst[i] = src1[i] ^ src2[i];
//     }
// }

// void CBC_Encrypt_LEA(u32* ciphertext, const u32* plaintext, size_t plaintext_len, const u32* key, const u32* iv) {
//     u32 roundKeys[TOTAL_RK];  // Define the size according to LEA specification
//     leaEncKeySchedule(roundKeys, key);

//     u32 prev_block[16 / sizeof(u32)];
//     memcpy(prev_block, iv, 16);  // Initialize with IV

//     for (size_t i = 0; i < plaintext_len; i += 16) {
//         // XOR plaintext with previous ciphertext (or IV for the first block)
//         xorBlocks(prev_block, plaintext, prev_block, 16);
        
//         // for (size_t j = 0; j < 16 / sizeof(u32); ++j) {
//         //     prev_block[j] ^= plaintext[i + j];
//         // }

//         // Encrypt the XORed data
//         leaEncrypt(&ciphertext[i], prev_block, roundKeys);

//         // Update prev_block to current ciphertext
//         memcpy(prev_block, &ciphertext[i], 16);
//     }
// }
// void CBC_Decrypt_LEA(u32* plaintext, const u32* ciphertext, size_t ciphertext_len, const u32* key, const u32* iv);

// // CBC Encryption
// void cbcEncrypt(u32* ciphertext, const u32* plaintext, size_t plaintext_len, const u32* key, const u32* iv) {
//     u32 roundKeys[...];  // Define the size according to LEA specification
//     leaEncKeySchedule(roundKeys, key);

//     u32 prev_block[BLOCK_SIZE / sizeof(u32)];
//     memcpy(prev_block, iv, BLOCK_SIZE);  // Initialize with IV

//     for (size_t i = 0; i < plaintext_len; i += BLOCK_SIZE) {
//         // XOR plaintext with previous ciphertext (or IV for the first block)
//         for (size_t j = 0; j < BLOCK_SIZE / sizeof(u32); ++j) {
//             prev_block[j] ^= plaintext[i + j];
//         }

//         // Encrypt the XORed data
//         leaEncrypt(&ciphertext[i], prev_block, roundKeys);

//         // Update prev_block to current ciphertext
//         memcpy(prev_block, &ciphertext[i], BLOCK_SIZE);
//     }
// }

// // CBC Decryption
// void cbcDecrypt(u32* plaintext, const u32* ciphertext, size_t ciphertext_len, const u32* key, const u32* iv) {
//     u32 roundKeys[...];  // Define the size according to LEA specification
//     leaDecKeySchedule(roundKeys, key);

//     u32 prev_block[BLOCK_SIZE / sizeof(u32)];
//     memcpy(prev_block, iv, BLOCK_SIZE);  // Initialize with IV

//     for (size_t i = 0; i < ciphertext_len; i += BLOCK_SIZE) {
//         u32 decrypted_block[BLOCK_SIZE / sizeof(u32)];

//         // Decrypt the block
//         leaDecrypt(decrypted_block, &ciphertext[i], roundKeys);

//         // XOR decrypted data with previous ciphertext (or IV for the first block)
//         for (size_t j = 0; j < BLOCK_SIZE / sizeof(u32); ++j) {
//             plaintext[i + j] = decrypted_block[j] ^ prev_block[j];
//         }

//         // Update prev_block to current ciphertext
//         memcpy(prev_block, &ciphertext[i], BLOCK_SIZE);
//     }
// }

// // OFB Mode (same for both Encryption and Decryption)
// void ofbOperate(u32* output, const u32* input, size_t length, const u32* key, const u32* iv) {
//     u32 roundKeys[...];  // Define according to LEA specification
//     leaEncKeySchedule(roundKeys, key);

//     u32 iv_block[BLOCK_SIZE / sizeof(u32)];
//     memcpy(iv_block, iv, BLOCK_SIZE);

//     for (size_t i = 0; i < length; i += BLOCK_SIZE) {
//         // Encrypt the IV/current block
//         u32 encrypted_block[BLOCK_SIZE / sizeof(u32)];
//         leaEncrypt(encrypted_block, iv_block, roundKeys);

//         // XOR with input to produce output
//         for (size_t j = 0; j < BLOCK_SIZE / sizeof(u32); ++j) {
//             output[i + j] = input[i + j] ^ encrypted_block[j];
//         }

//         // Update iv_block for next round
//         memcpy(iv_block, encrypted_block, BLOCK_SIZE);
//     }
// }

// // CFB Encryption
// void cfbEncrypt(u32* ciphertext, const u32* plaintext, size_t length, const u32* key, const u32* iv) {
//     u32 roundKeys[...];  // Define according to LEA specification
//     leaEncKeySchedule(roundKeys, key);

//     u32 iv_block[BLOCK_SIZE / sizeof(u32)];
//     memcpy(iv_block, iv, BLOCK_SIZE);  // Initialize with IV

//     for (size_t i = 0; i < length; i += BLOCK_SIZE) {
//         u32 encrypted_block[BLOCK_SIZE / sizeof(u32)];
//         leaEncrypt(encrypted_block, iv_block, roundKeys);  // Encrypt IV/previous block

//         // XOR with plaintext to produce ciphertext
//         for (size_t j = 0; j < BLOCK_SIZE / sizeof(u32); ++j) {
//             ciphertext[i + j] = plaintext[i + j] ^ encrypted_block[j];
//         }

//         // Update iv_block to current ciphertext for next round
//         memcpy(iv_block, &ciphertext[i], BLOCK_SIZE);
//     }
// }

// // CFB Decryption
// void cfbDecrypt(u32* plaintext, const u32* ciphertext, size_t length, const u32* key, const u32* iv) {
//     u32 roundKeys[...];  // Define according to LEA specification
//     leaDecKeySchedule(roundKeys, key);

//     u32 iv_block[BLOCK_SIZE / sizeof(u32)];
//     memcpy(iv_block, iv, BLOCK_SIZE);  // Initialize with IV

//     for (size_t i = 0; i < length; i += BLOCK_SIZE) {
//         u32 encrypted_block[BLOCK_SIZE / sizeof(u32)];
//         leaEncrypt(encrypted_block, iv_block, roundKeys);  // Encrypt IV/previous block

//         // XOR with ciphertext to produce plaintext
//         for (size_t j = 0; j < BLOCK_SIZE / sizeof(u32); ++j) {
//             plaintext[i + j] = ciphertext[i + j] ^ encrypted_block[j];
//         }

//         // Update iv_block to current ciphertext for next round
//         memcpy(iv_block, &ciphertext[i], BLOCK_SIZE);
//     }
// }

// // Function to increment the counter (assumes counter is 128 bits)
// void incrementCounter(u32* counter) {
//     for (int i = (BLOCK_SIZE / sizeof(u32)) - 1; i >= 0; i--) {
//         if (++counter[i] != 0) {
//             break;
//         }
//     }
// }

// // CTR Mode (Encryption and Decryption)
// void ctrOperate(u32* output, const u32* input, size_t length, const u32* key, const u32* counter) {
//     u32 roundKeys[...];  // Define according to LEA specification
//     leaEncKeySchedule(roundKeys, key);

//     u32 counter_block[BLOCK_SIZE / sizeof(u32)];
//     memcpy(counter_block, counter, BLOCK_SIZE);  // Initialize with counter value

//     for (size_t i = 0; i < length; i += BLOCK_SIZE) {
//         u32 encrypted_counter[BLOCK_SIZE / sizeof(u32)];
//         leaEncrypt(encrypted_counter, counter_block, roundKeys);  // Encrypt counter

//         // XOR with input to produce output
//         for (size_t j = 0; j < BLOCK_SIZE / sizeof(u32); ++j) {
//             output[i + j] = input[i + j] ^ encrypted_counter[j];
//         }

//         incrementCounter(counter_block);  // Increment counter
//     }
// }

// void ecbDecrypt(const u32* ciphertext, const u32* roundKeys, u32* plaintext, size_t blockSize) {
//     for (size_t offset = 0; offset < blockSize; offset += sizeof(u32)) {
//         leaDecrypt(&ciphertext[offset / sizeof(u32)], roundKeys, &plaintext[offset / sizeof(u32)]);
//     }
// }

// void cbcEncrypt(const u32* plaintext, const u32* roundKeys, u32* ciphertext, size_t blockSize, const u32* iv) {
//     u32 block[blockSize / sizeof(u32)];
//     memcpy(block, iv, blockSize);

//     for (size_t offset = 0; offset < blockSize; offset += sizeof(block)) {
//         xorBlocks(&plaintext[offset / sizeof(u32)], block, block, blockSize);
//         leaEncrypt(block, roundKeys, &ciphertext[offset / sizeof(u32)]);
//         memcpy(block, &ciphertext[offset / sizeof(u32)], blockSize);
//     }
// }

// void cbcDecrypt(const u32* ciphertext, const u32* roundKeys, u32* plaintext, size_t blockSize, const u32* iv) {
//     u32 block[blockSize / sizeof(u32)];
//     u32 temp[blockSize / sizeof(u32)];

//     memcpy(block, iv, blockSize);

//     for (size_t offset = 0; offset < blockSize; offset += sizeof(block)) {
//         memcpy(temp, &ciphertext[offset / sizeof(u32)], blockSize);
//         leaDecrypt(&ciphertext[offset / sizeof(u32)], roundKeys, &plaintext[offset / sizeof(u32)]);
//         xorBlocks(&plaintext[offset / sizeof(u32)], block, &plaintext[offset / sizeof(u32)], blockSize);
//         memcpy(block, temp, blockSize);
//     }
// }

// void cfbEncrypt(const u32* plaintext, const u32* roundKeys, u32* ciphertext, size_t blockSize, u32* iv) {
//     u32 feedback[blockSize / sizeof(u32)];
//     memcpy(feedback, iv, blockSize);

//     for (size_t offset = 0; offset < blockSize; offset += sizeof(u32)) {
//         leaEncrypt(feedback, roundKeys, feedback);
//         xorBlocks(&plaintext[offset / sizeof(u32)], feedback, &ciphertext[offset / sizeof(u32)], blockSize);
//         memcpy(feedback, &ciphertext[offset / sizeof(u32)], blockSize);
//     }
// }

// void cfbDecrypt(const u32* ciphertext, const u32* roundKeys, u32* plaintext, size_t blockSize, u32* iv) {
//     u32 feedback[blockSize / sizeof(u32)];
//     memcpy(feedback, iv, blockSize);

//     for (size_t offset = 0; offset < blockSize; offset += sizeof(u32)) {
//         leaEncrypt(feedback, roundKeys, feedback);
//         xorBlocks(&ciphertext[offset / sizeof(u32)], feedback, &plaintext[offset / sizeof(u32)], blockSize);
//         memcpy(feedback, &ciphertext[offset / sizeof(u32)], blockSize);
//     }
// }

// void ofbCrypt(const u32* input, const u32* roundKeys, u32* output, size_t blockSize, u32* iv) {
//     u32 feedback[blockSize / sizeof(u32)];
//     memcpy(feedback, iv, blockSize);

//     for (size_t offset = 0; offset < blockSize; offset += sizeof(u32)) {
//         leaEncrypt(feedback, roundKeys, feedback);
//         xorBlocks(&input[offset / sizeof(u32)], feedback, &output[offset / sizeof(u32)], blockSize);
//     }
// }

// void ctrCrypt(const u32* input, const u32* roundKeys, u32* output, size_t blockSize, u32* counter) {
//     u32 keystream[blockSize / sizeof(u32)];

//     for (size_t offset = 0; offset < blockSize; offset += sizeof(u32)) {
//         leaEncrypt(counter, roundKeys, keystream);
//         xorBlocks(&input[offset / sizeof(u32)], keystream, &output[offset / sizeof(u32)], blockSize);

//         // Increment the counter
//         for (int i = (blockSize / sizeof(u32)) - 1; i >= 0; i--) {
//             if (++counter[i] != 0) {
//                 break;
//             }
//         }
//     }
// }