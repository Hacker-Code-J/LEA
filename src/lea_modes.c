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

void xorBlocks(const u32* src1, const u32* src2, u32* dst, size_t blockSize) {
    for (size_t i = 0; i < blockSize / sizeof(u32); i++) {
        dst[i] = src1[i] ^ src2[i];
    }
}

void ecbEncrypt(const u32* plaintext, const u32* roundKeys, u32* ciphertext, size_t blockSize) {
    for (size_t offset = 0; offset < blockSize; offset += sizeof(u32)) {
        leaEncrypt(&plaintext[offset / sizeof(u32)], roundKeys, &ciphertext[offset / sizeof(u32)]);
    }
}

void ecbDecrypt(const u32* ciphertext, const u32* roundKeys, u32* plaintext, size_t blockSize) {
    for (size_t offset = 0; offset < blockSize; offset += sizeof(u32)) {
        leaDecrypt(&ciphertext[offset / sizeof(u32)], roundKeys, &plaintext[offset / sizeof(u32)]);
    }
}

void cbcEncrypt(const u32* plaintext, const u32* roundKeys, u32* ciphertext, size_t blockSize, const u32* iv) {
    u32 block[blockSize / sizeof(u32)];
    memcpy(block, iv, blockSize);

    for (size_t offset = 0; offset < blockSize; offset += sizeof(block)) {
        xorBlocks(&plaintext[offset / sizeof(u32)], block, block, blockSize);
        leaEncrypt(block, roundKeys, &ciphertext[offset / sizeof(u32)]);
        memcpy(block, &ciphertext[offset / sizeof(u32)], blockSize);
    }
}

void cbcDecrypt(const u32* ciphertext, const u32* roundKeys, u32* plaintext, size_t blockSize, const u32* iv) {
    u32 block[blockSize / sizeof(u32)];
    u32 temp[blockSize / sizeof(u32)];

    memcpy(block, iv, blockSize);

    for (size_t offset = 0; offset < blockSize; offset += sizeof(block)) {
        memcpy(temp, &ciphertext[offset / sizeof(u32)], blockSize);
        leaDecrypt(&ciphertext[offset / sizeof(u32)], roundKeys, &plaintext[offset / sizeof(u32)]);
        xorBlocks(&plaintext[offset / sizeof(u32)], block, &plaintext[offset / sizeof(u32)], blockSize);
        memcpy(block, temp, blockSize);
    }
}

void cfbEncrypt(const u32* plaintext, const u32* roundKeys, u32* ciphertext, size_t blockSize, u32* iv) {
    u32 feedback[blockSize / sizeof(u32)];
    memcpy(feedback, iv, blockSize);

    for (size_t offset = 0; offset < blockSize; offset += sizeof(u32)) {
        leaEncrypt(feedback, roundKeys, feedback);
        xorBlocks(&plaintext[offset / sizeof(u32)], feedback, &ciphertext[offset / sizeof(u32)], blockSize);
        memcpy(feedback, &ciphertext[offset / sizeof(u32)], blockSize);
    }
}

void cfbDecrypt(const u32* ciphertext, const u32* roundKeys, u32* plaintext, size_t blockSize, u32* iv) {
    u32 feedback[blockSize / sizeof(u32)];
    memcpy(feedback, iv, blockSize);

    for (size_t offset = 0; offset < blockSize; offset += sizeof(u32)) {
        leaEncrypt(feedback, roundKeys, feedback);
        xorBlocks(&ciphertext[offset / sizeof(u32)], feedback, &plaintext[offset / sizeof(u32)], blockSize);
        memcpy(feedback, &ciphertext[offset / sizeof(u32)], blockSize);
    }
}

void ofbCrypt(const u32* input, const u32* roundKeys, u32* output, size_t blockSize, u32* iv) {
    u32 feedback[blockSize / sizeof(u32)];
    memcpy(feedback, iv, blockSize);

    for (size_t offset = 0; offset < blockSize; offset += sizeof(u32)) {
        leaEncrypt(feedback, roundKeys, feedback);
        xorBlocks(&input[offset / sizeof(u32)], feedback, &output[offset / sizeof(u32)], blockSize);
    }
}

void ctrCrypt(const u32* input, const u32* roundKeys, u32* output, size_t blockSize, u32* counter) {
    u32 keystream[blockSize / sizeof(u32)];

    for (size_t offset = 0; offset < blockSize; offset += sizeof(u32)) {
        leaEncrypt(counter, roundKeys, keystream);
        xorBlocks(&input[offset / sizeof(u32)], keystream, &output[offset / sizeof(u32)], blockSize);

        // Increment the counter
        for (int i = (blockSize / sizeof(u32)) - 1; i >= 0; i--) {
            if (++counter[i] != 0) {
                break;
            }
        }
    }
}