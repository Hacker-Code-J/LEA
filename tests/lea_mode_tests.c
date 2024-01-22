#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "lea.h"
#include "lea_modes.h"

#if 0
void lea128_ECB_test() {
    u32 key[KEY_SIZE];
    const char* keyString = "0f1e2d3c4b5a69788796a5b4c3d2e1f0"; // 128-bit Key
    stringToWordArray(key, keyString);

    // Print the plaintext
    printf("Key: \n");
    printBigEndian(key, KEY_SIZE);
    // printLittleEndian(key, KEY_SIZE);
    
    const char* plainString = "101112131415161718191a1b1c1d1e1f4F524954484D";
    int plain_len_word = (strlen(plainString) % 32) ? ((strlen(plainString) / 32) + 1) : (strlen(plainString) / 32);
    
    u32* plain = (u32*)calloc(plain_len_word, sizeof(u32));
    u32* cipher = (u32*)calloc(plain_len_word, sizeof(u32));

    printf("%d %ld\n", plain_len_word, sizeof(plain) / sizeof(u32*));

    stringToWordArray(plain, plainString);
    printf("\nPlain-Text: \n");
    printBigEndian(plain, sizeof(u32) * plain_len_word);
    printLittleEndian(plain, sizeof(u32) * plain_len_word);

    PKCS7_BYTE_PAD_32bit(plain, 16 * plain_len_word,
        (strlen(plainString) % 2) ? (strlen(plainString) + 1) / 2 : strlen(plainString) / 2);

    printf("\nPlain-Text with Padding: \n");
    printBigEndian(plain, sizeof(u32) * plain_len_word);
    printLittleEndian(plain, sizeof(u32) * plain_len_word);

    // ECB_Encrypt_LEA(cipher, plain, plain_len_word, key);
    
    printf("\nCipher-Text: \n");
    printBigEndian(cipher, sizeof(u32) * plain_len_word);
    printLittleEndian(cipher, sizeof(u32) * plain_len_word);
    // double enc_time = measure_time(leaEncrypt, plain, enc_roundkey, cipher);
    // printf("%.3f ns\n", enc_time*1000000000);

#if 0
    4F524954484D
    u32* encrypted = (u32*)calloc(plain_len_word, sizeof(u32));
    // u32* encrypted = cipher;
    u32* decrypted = (u32*)calloc(plain_len_word, sizeof(u32));

    // memcpy(encrypted, cipher, 16);
    for (int i = 0; i < sizeof(u32) * plain_len_word; i++)
        encrypted[i] = cipher[i];
    printf("\nEncrypted Text: \n");
    printLittleEndian(encrypted, sizeof(u32) * plain_len_word);

    ECB_Decrypt_LEA(decrypted, encrypted, plain_len_word, key);
    free(encrypted);

    printf("\nDecrypted Text: \n");
    printBigEndian(decrypted, sizeof(32) * plain_len_word);
    printLittleEndian(decrypted, sizeof(u32) * plain_len_word);

    // double dec_time = measure_time(leaDecrypt, encrypted, dec_roundkey, decrypted);
    // printf("%.3f ns\n", dec_time*1000000000);

    free(cipher);
    free(decrypted);
#endif
}
#endif