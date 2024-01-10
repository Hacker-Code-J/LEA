#include "lea_modes.h"

void pkcs7_pad(u8* block, size_t block_len, size_t input_len) {
    if (block_len < input_len) {
        fprintf(stderr, "Block length must be greater than input length.\n");
        return;
    }

    u8 padding_value = block_len - input_len;
    for (size_t i = input_len; i < block_len; ++i) {
        block[i] = padding_value;
    }
}

void PKCS7_BYTE_PAD_32bit(u32* block, size_t block_len_bytes, size_t input_len_bytes) {
    if (block_len_bytes < input_len_bytes) {
        fprintf(stderr, "Block length must be greater than or equal to input length.\n");
        return;
    }

    for (int i = 0; i < block_len_bytes / sizeof(u32); i++) {
        printf("P[%02d]: 0x%08x\n", i, block[i]);
    }

    // Calculate the padding value
    u8 padding_byte_value = block_len_bytes - input_len_bytes;
    if (padding_byte_value == 0) {
        return; // No padding needed
    }

    // Apply the padding
    u8* byte_ptr = (u8*)block;
    // printf("Byte 0: 0x%02x\n", byte_ptr[0]);
    // printf("Byte 1: 0x%02x\n", byte_ptr[1]);
    // printf("Byte 2: 0x%02x\n", byte_ptr[2]);
    // printf("Byte 3: 0x%02x\n", byte_ptr[3]);
    // printf("Byte 4: 0x%02x\n", byte_ptr[4]);
    // printf("Byte 5: 0x%02x\n", byte_ptr[5]);
    // printf("Byte 6: 0x%02x\n", byte_ptr[6]);
    // printf("Byte 7: 0x%02x\n", byte_ptr[7]);
    // printf("Byte 8: 0x%02x\n", byte_ptr[8]);
    // printf("Byte 9: 0x%02x\n", byte_ptr[9]);
    // printf("Byte 10: 0x%02x\n", byte_ptr[10]);
    // printf("Byte 11: 0x%02x\n", byte_ptr[11]);
    // printf("Byte 12: 0x%02x\n", byte_ptr[12]);
    // printf("Byte 13: 0x%02x\n", byte_ptr[13]);
    // printf("Byte 14: 0x%02x\n", byte_ptr[14]);
    // printf("Byte 15: 0x%02x\n", byte_ptr[15]);
    
    int s = 0x00;
    int t = 0x00;
    if (input_len_bytes == 2 && byte_ptr[3] == 0x00) s++;
    if (input_len_bytes == 2 && byte_ptr[2] == 0x00) t++;
    for (size_t i =
        (input_len_bytes > 3) ?
         input_len_bytes - (padding_byte_value % sizeof(u32)) : 0;
         i < block_len_bytes; i++) {
        printf("Byte %ld: 0x%02x\n", i, byte_ptr[i]);
        if (!byte_ptr[i])
            byte_ptr[i] = padding_byte_value;
    }
    if (s) byte_ptr[3] = 0x00;
    if (t) byte_ptr[2] = 0x00;
    
    for (int i = 0; i < block_len_bytes / sizeof(u32); i++) {
        printf("P[%02d]: 0x%08x\n", i, block[i]);
    }
}

// void xorBlocks(const u32* src1, const u32* src2, u32* dst, size_t blockSize) {
//     for (size_t i = 0; i < blockSize / sizeof(u32); i++) {
//         dst[i] = src1[i] ^ src2[i];
//     }
// }

// void ecbEncrypt(const u32* plaintext, const u32* roundKeys, u32* ciphertext, size_t blockSize) {
//     for (size_t offset = 0; offset < blockSize; offset += sizeof(u32)) {
//         leaEncrypt(&plaintext[offset / sizeof(u32)], roundKeys, &ciphertext[offset / sizeof(u32)]);
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