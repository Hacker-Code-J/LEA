#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "lea_modes.h"

// KEY = C9592F08A1DCC81A14C584C2C2081441
// IV = F88C5A41C9E71DB9606A0010945DB585
// PT = BD8B6ECEC60294FF8414BCAE46A1D755AE99A8F7BFBC7C0736A415851E
// 06E76A9F35874617E760286C3CA8BEA0FC92BDA520C58373950EFC593C51C29035C
// 688165D61B8B915925B72462F3AF5A2AF42862C982D0EFA3A5F6E399E0E1373BAA6
// CB0DE66CD714946141393B69701994ACF9C2093DB8736BFA20A4E3B3CF462B8E654
// CFDA9976ACC027F1DB39436EBABC3A5EC00FA98890693148408F7EB398106
// CT = A59F43B4DAE787741D801B5E8054240722288D3A350154ABD4C965922C7438E0C826DF5F9ECBE4F3BF0775B515A292448E8D295F7CCC70F822C95BD648254DF449AFA640AE07743C5868B1AFE524728B11C021C9B6744F06CD61A862AA0D4916327B0DEAE96FEE5065CD56235C3477B8CDC7822AC3CA27FB16287C3CB82B82BB94295BE8F855FF30A302818F1C40DB10BE8C0F775626811F61D6C6898E708EE4


void CBC_lea128_test(void) {
    const int KEY_SIZE = 4; // 4-word = 128-bit
    
    u32 key[KEY_SIZE];
    const char* keyString = "C9592F08A1DCC81A14C584C2C2081441";
    stringToWordArray(key, keyString);
    printf("Key: \n");
    printBigEndian(key, KEY_SIZE);

    const char* ivString = "F88C5A41C9E71DB9606A0010945DB585";
    u32 iv[4];
    stringToWordArray(iv, ivString);

    const char* plainString = "BD8B6ECEC60294FF8414BCAE46A1D755AE99A8F7BFBC7C0736A415851E06E76A9F35874617E760286C3CA8BEA0FC92BDA520C58373950EFC593C51C29035C688165D61B8B915925B72462F3AF5A2AF42862C982D0EFA3A5F6E399E0E1373BAA6CB0DE66CD714946141393B69701994ACF9C2093DB8736BFA20A4E3B3CF462B8E654CFDA9976ACC027F1DB39436EBABC3A5EC00FA98890693148408F7EB398106";
    u32 plain[strlen(plainString) / 8];
    stringToWordArray(plain, plainString);
    printf("\nPlain-Text: \n");
    printBigEndian(plain, strlen(plainString) / 8);
    printLittleEndian(plain, strlen(plainString) / 8);

    u32 cipher[strlen(plainString) / 8];
    CBC_Encrypt_LEA(cipher, plain, strlen(plainString) / 8,
                key, KEY_SIZE, iv, LEA128);
    
    const char* ansString = "A59F43B4DAE787741D801B5E8054240722288D3A350154ABD4C965922C7438E0C826DF5F9ECBE4F3BF0775B515A292448E8D295F7CCC70F822C95BD648254DF449AFA640AE07743C5868B1AFE524728B11C021C9B6744F06CD61A862AA0D4916327B0DEAE96FEE5065CD56235C3477B8CDC7822AC3CA27FB16287C3CB82B82BB94295BE8F855FF30A302818F1C40DB10BE8C0F775626811F61D6C6898E708EE4";
    u32 ans[strlen(ansString) / 8];
    stringToWordArray(ans, ansString);
    printf("\nCipher Text: \n");
    printBigEndian(ans, strlen(ansString) / 8);
    
    printf("\nEncrypted Text: \n");
    printBigEndian(cipher, strlen(plainString) / 8);
    printLittleEndian(cipher, strlen(plainString) / 8);

    u32 encrypted[strlen(plainString) / 8];
    u32 decrypted[strlen(plainString) / 8];
    memcpy(encrypted, cipher, strlen(plainString) / 2);
    printf("\nEncrypted Text: \n");
    printLittleEndian(encrypted, strlen(plainString) / 8);

    CBC_Decrypt_LEA(decrypted, encrypted, strlen(plainString) / 8,
                    key, KEY_SIZE, iv, LEA128);
    printf("\nDecrypted Text: \n");
    printBigEndian(decrypted, strlen(plainString) / 8);
}
// void lea192_CBC_test(void);
// void lea256_CBC_test(void);

// void lea128_CTR_test(void);
// void lea192_CTR_test(void);
// void lea256_CTR_test(void);

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