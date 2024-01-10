/**
 * @file main.c
 * @brief Main entry point for the LEA encryption algorithm demonstration.
 *
 * This file contains the main function to demonstrate the usage of the LEA
 * encryption algorithm. It may include example usage or benchmarking.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lea.h"

int main(void) {
    u32 key[KEY_SIZE];
    // const char* keyString = "0f1e2d3c4b5a69788796a5b4c3d2e1f0"; // 128-bit Key
    // const char* keyString = "0f1e2d3c4b5a69788796a5b4c3d2e1f0f0e1d2c3b4a59687"; // 192-bit Key
    const char* keyString = "0f1e2d3c4b5a69788796a5b4c3d2e1f0f0e1d2c3b4a5968778695a4b3c2d1e0f"; // 256-bit Key
    stringToWordArray(keyString, key);
    
    // Print the plaintext
    printf("Key: \n");
    printLittleEndian(key, KEY_SIZE);

    // 192 * 24 = 4608 = 32 * 144
    // 192 * 28 = 5376 = 32 * 168
    // 192 * 32 = 6144 = 32 * 192
    u32 enc_roundkey[TOTAL_RK];
    leaEncKeySchedule(key, enc_roundkey);
    printf("\nEncryption RoundKey: \n");
    for (int i = 0, j = 0; i < TOTAL_RK; i++) {
        if((i % 6) == 0) printf("\nEnc_Round[%02d] | ", j++);
        printf("%08x:", enc_roundkey[i]);
    }
    printf("\n");

    u32 plain[4];
    u32 cipher[4];

    // const char* plainString = "101112131415161718191a1b1c1d1e1f"; // for 128-bit Key
    // const char* plainString = "202122232425262728292a2b2c2d2e2f"; // for 192-bit Key
    const char* plainString = "303132333435363738393a3b3c3d3e3f"; // for 256-bit Key
    
    printf("\nPlain-Text: \n");
    printf("%s\n", plainString);
    stringToWordArray(plainString, plain);

    leaEncrypt(plain, enc_roundkey, cipher);
    
    printf("\nCipher-Text: \n");
    printLittleEndian(cipher, sizeof(u32));

    // double enc_time = measure_time(leaEncrypt, plain, enc_roundkey, cipher);
    // printf("%.3f ns\n", enc_time*1000000000);

    u32 encrypted[4];
    u32 decrypted[4];

    memcpy(encrypted, cipher, 16);
    printf("\nEncrypted Text: \n");
    printLittleEndian(encrypted, sizeof(u32));

    // 192 * 24 = 4608 = 32 * 144
    // 192 * 28 = 5376 = 32 * 168
    // 192 * 32 = 6144 = 32 * 192
    u32 dec_roundkey[TOTAL_RK];
    leaDecKeySchedule(key, dec_roundkey);
    printf("\nDecryption RoundKey: \n");
    for (int i = 0, j = 0; i < TOTAL_RK; i++) {
        if((i % 6) == 0) printf("\nDec_Round[%02d] | ", j++);
        printf("%08x:", dec_roundkey[i]);
    }
    printf("\n");

    leaDecrypt(encrypted, dec_roundkey, decrypted);
    
    printf("\nDecrypted Text: \n");
    printLittleEndian(decrypted, sizeof(u32));

    // double dec_time = measure_time(leaDecrypt, encrypted, dec_roundkey, decrypted);
    // printf("%.3f ns\n", dec_time*1000000000);

}
