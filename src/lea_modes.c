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

void PKCS7_PAD_32bit(u32* block, size_t block_len_bits, size_t input_len_bits) {
    size_t block_len_bytes = block_len_bits / 0x08;
    size_t input_len_bytes = input_len_bits / 0x08;

    if (block_len_bits < input_len_bits) {
        fprintf(stderr, "Block length must be greater than or equal to input length.\n");
        return;
    }

    // Calculate the padding value
    u8 padding_byte_value = block_len_bytes - input_len_bytes;
    
    // // Apply the padding in a big-endian manner
    // u8* byte_ptr = (u8*)block;
    // size_t start_padding_index = input_len_bytes;

    // // Find the start of the last 32-bit word to be padded
    // size_t word_index = (start_padding_index + 3) / sizeof(u32);
    // size_t word_start = word_index * sizeof(u32);

    // // Padding within the last 32-bit word
    // for (size_t i = start_padding_index; i < word_start + sizeof(u32); ++i) {
    //     byte_ptr[i] = padding_byte_value;
    // }

    // Apply the padding in a little-endian manner
    u8* byte_ptr = (u8*)block;
    size_t start_padding_index = input_len_bytes;

    // Padding within the last 32-bit word
    for (size_t i = start_padding_index; i < block_len_bytes; ++i) {
        byte_ptr[i] = padding_byte_value;
    }
}

void PKCS7_BYTE_PAD_32bit(u32* block, size_t block_len_bytes, size_t input_len_bytes) {
    if (block_len_bytes < input_len_bytes) {
        fprintf(stderr, "Block length must be greater than or equal to input length.\n");
        return;
    }

    if (block_len_bytes < input_len_bytes) {
        fprintf(stderr, "Block length must be greater than or equal to input length.\n");
        return;
    }

    // Reverse byte order before padding
    size_t num_words = input_len_bytes / sizeof(u32);
    for (size_t i = 0; i < num_words; i++) {
        block[i] = REVERSE_BYTE_ORDER(block[i]);
    }

    // Calculate the padding value
    u8 padding_byte_value = block_len_bytes - input_len_bytes;

    // Apply the padding
    u8* byte_ptr = (u8*)block;
    for (size_t i = input_len_bytes; i < block_len_bytes; ++i) {
        byte_ptr[i] = padding_byte_value;
    }

    // Reorder bytes for little-endian storage after padding
    num_words = block_len_bytes / sizeof(u32);
    for (size_t i = 0; i < num_words; i++) {
        block[i] = REVERSE_BYTE_ORDER(block[i]);
    }

    // // Calculate the padding value
    // u8 padding_byte_value = block_len_bytes - input_len_bytes;

    // // Apply the padding in a little-endian manner
    // u8* byte_ptr = (u8*)block;

    // // Identify the word and the byte within the word where padding starts
    // size_t word_index = input_len_bytes / sizeof(u32);
    // size_t byte_index_within_word = input_len_bytes % sizeof(u32);

    // // Start padding from the identified position
    // for (size_t i = 0; i < padding_byte_value; ++i) {
    //     if (word_index * sizeof(u32) + byte_index_within_word + i < block_len_bytes) {
    //         byte_ptr[word_index * sizeof(u32) + byte_index_within_word + i] = padding_byte_value;
    //     }
    // }

    // // Calculate the padding value
    // u8 padding_byte_value = block_len_bytes - input_len_bytes;

    // // Apply the padding in a little-endian manner
    // u8* byte_ptr = (u8*)block;
    // size_t start_padding_index = input_len_bytes % sizeof(u32); // Start padding within the last used word

    // // Identify the word that needs padding
    // size_t word_index = input_len_bytes / sizeof(u32);

    // // Start padding from the correct byte in the identified word
    // for (size_t i = 0; i < padding_byte_value; ++i) {
    //     if (start_padding_index + i < sizeof(u32)) {
    //         byte_ptr[word_index * sizeof(u32) + start_padding_index + i] = padding_byte_value;
    //     }
    // }
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