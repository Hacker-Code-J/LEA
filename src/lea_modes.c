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

void CBC_Encrypt_LEA(u32* restrict ct, 
                     const u32* restrict pt, const size_t dataLength,
                     const u32* restrict key, const size_t keyLength,
                     const u32* restrict iv) {
    // Assuming block size is the size of u32 * 4 (128 bits)
    const size_t blockSize = 4; 
    const size_t numBlocks = dataLength / blockSize;

    // Avoid allocating ct, it should be provided by the caller

    // Round keys should be computed outside if key does not change frequently
    u32 roundKeys[(keyLength == 4) ? 144 : (keyLength == 6) ? 168 : 192];
    leaEncKeySchedule(roundKeys, key);

    // Use restrict keyword if supported, and avoid using temp if not necessary
    u32 buffer[blockSize];

    // Assuming IV is not modified, make a copy if it needs to be preserved
    u32 currentIV[blockSize];
    memcpy(currentIV, iv, blockSize * sizeof(u32));

    // CBC encryption
    for (size_t i = 0; i < numBlocks; i++) {
        // XOR with IV or previous ciphertext block
        for (size_t j = 0; j < blockSize; j++) {
            buffer[j] = pt[i * blockSize + j] ^ currentIV[j];
        }

        // Encrypt the Block
        leaEncrypt(&ct[i * blockSize], buffer, roundKeys);

        // Update IV (current ciphertext block becomes next IV)
        memcpy(currentIV, &ct[i * blockSize], blockSize * sizeof(u32));
    }
}

void CBC_Decrypt_LEA(u32* restrict pt, 
                     const u32* restrict ct, const size_t dataLength,
                     const u32* restrict key, const size_t keyLength,
                     const u32* restrict iv) {
    // Assuming block size is the size of u32 * 4 (128 bits)
    const size_t blockSize = 4; 
    const size_t numBlocks = dataLength / blockSize;

    // Avoid allocating ct, it should be provided by the caller

    // Round keys should be computed outside if key does not change frequently
    u32 roundKeys[(keyLength == 4) ? 144 : (keyLength == 6) ? 168 : 192];
    leaDecKeySchedule(roundKeys, key);

    // Use restrict keyword if supported, and avoid using temp if not necessary
    u32 buffer[blockSize];

    // Assuming IV is not modified, make a copy if it needs to be preserved
    u32 currentIV[blockSize];
    memcpy(currentIV, iv, blockSize * sizeof(u32));

    // CBC encryption
    for (size_t i = 0; i < numBlocks; i++) {
        // XOR with IV or previous ciphertext block
        for (size_t j = 0; j < blockSize; j++) {
            buffer[j] = ct[i * blockSize + j] ^ currentIV[j];
        }

        // Decrypt the Block
        leaDecrypt(&pt[i * blockSize], buffer, roundKeys);

        // Update IV (current ciphertext block becomes next IV)
        memcpy(currentIV, &pt[i * blockSize], blockSize * sizeof(u32));
    }
}

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