#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "lea.h"
#include "lea_modes.h"

void lea128_test(void) {
    const int KEY_SIZE = 4; // 4-byte = 128-bit
    const int TOTAL_RK = 144;
    
    u32 key[KEY_SIZE];
    const char* keyString = "0f1e2d3c4b5a69788796a5b4c3d2e1f0"; // 128-bit Key
    // const char* keyString = "0f1e2d3c4b5a69788796a5b4c3d2e1f0f0e1d2c3b4a59687"; // 192-bit Key
    // const char* keyString = "0f1e2d3c4b5a69788796a5b4c3d2e1f0f0e1d2c3b4a5968778695a4b3c2d1e0f"; // 256-bit Key
    stringToWordArray(key, keyString);

    // Print the plaintext
    printf("Key: \n");
    printBigEndian(key, KEY_SIZE);
    // printLittleEndian(key, KEY_SIZE);

    // 192 * 24 = 4608 = 32 * 144
    // 192 * 28 = 5376 = 32 * 168
    // 192 * 32 = 6144 = 32 * 192
    u32 enc_roundkey[TOTAL_RK];
    
    leaEncKeySchedule(enc_roundkey, key, LEA128);
    printEncRoundKeys(enc_roundkey, LEA128);

    u32 plain[4] = { 0x00, };
    u32 cipher[4];

    const char* plainString = "101112131415161718191a1b1c1d1e1f"; // for 128-bit Key
    // const char* plainString = "202122232425262728292a2b2c2d2e2f"; // for 192-bit Key
    // const char* plainString = "303132333435363738393a3b3c3d3e3f"; // for 256-bit Key
    
    // const char* plainString = "4F524954484D"; // 48-bit
    stringToWordArray(plain, plainString);

    printf("\nPlain-Text: \n");
    printBigEndian(plain, 4);
    printLittleEndian(plain, 4);

    // PKCS7_BYTE_PAD_32bit(plain, 16,
    //     (strlen(plainString) % 2) ? (strlen(plainString) + 1) / 2 : strlen(plainString) / 2);

    printf("\nPadding Plain-Text: \n");
    printBigEndian(plain, 4);
    printLittleEndian(plain, 4);
    

#if 1
    // u64 start, end;
    // start = rdtsc();
    leaEncrypt(cipher, plain, key, LEA128);
    // end = rdtsc();
    // printf("Cycles: %lu\n", end - start);
    
    printf("\nCipher-Text: \n");
    printBigEndian(cipher, sizeof(u32));
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
    leaDecKeySchedule(dec_roundkey, key, LEA128);
    printDecRoundKeys(dec_roundkey, LEA128);
    // printf("\nDecryption RoundKey: \n");
    // for (int i = 0, j = 0; i < TOTAL_RK; i++) {
    //     if((i % 6) == 0) printf("\nDec_Round[%02d] | ", j++);
    //     printf("%08x:", dec_roundkey[i]);
    // }
    // printf("\n");

    leaDecrypt(decrypted, encrypted, key, LEA128);
    
    printf("\nDecrypted Text: \n");
    printBigEndian(decrypted, sizeof(32));
    // printLittleEndian(decrypted, sizeof(u32));

    // double dec_time = measure_time(leaDecrypt, encrypted, dec_roundkey, decrypted);
    // printf("%.3f ns\n", dec_time*1000000000);
#endif
}

#if 0
void lea192_test() {
    u32 key[KEY_SIZE];
    const char* keyString = "0f1e2d3c4b5a69788796a5b4c3d2e1f0f0e1d2c3b4a59687"; // 192-bit Key
    stringToWordArray(key, keyString);

    // Print the plaintext
    printf("Key: \n");
    printBigEndian(key, KEY_SIZE);
    // printLittleEndian(key, KEY_SIZE);

    u32 enc_roundkey[TOTAL_RK];
    leaEncKeySchedule(enc_roundkey, key);
    printEncRoundKeys(enc_roundkey);

    u32 plain[4] = { 0x00, };
    u32 cipher[4];

    const char* plainString = "202122232425262728292a2b2c2d2e2f"; // for 192-bit Key
    stringToWordArray(plain, plainString);

    printf("\nPlain-Text: \n");
    printBigEndian(plain, 4);
    // printLittleEndian(plain, 4);

    leaEncrypt(cipher, plain, enc_roundkey);
    
    printf("\nCipher-Text: \n");
    printBigEndian(cipher, sizeof(u32));
    // printLittleEndian(cipher, sizeof(u32));

    // double enc_time = measure_time(leaEncrypt, plain, enc_roundkey, cipher);
    // printf("%.3f ns\n", enc_time*1000000000);

    u32 encrypted[4];
    u32 decrypted[4];

    memcpy(encrypted, cipher, 16);
    printf("\nEncrypted Text: \n");
    printLittleEndian(encrypted, sizeof(u32));

    u32 dec_roundkey[TOTAL_RK];
    leaDecKeySchedule(dec_roundkey, key);
    printDecRoundKeys(dec_roundkey);
    
    leaDecrypt(decrypted, encrypted, dec_roundkey);
    
    printf("\nDecrypted Text: \n");
    printBigEndian(decrypted, sizeof(32));
    // printLittleEndian(decrypted, sizeof(u32));

    // double dec_time = measure_time(leaDecrypt, encrypted, dec_roundkey, decrypted);
    // printf("%.3f ns\n", dec_time*1000000000);
}

void lea256_test() {
    u32 key[KEY_SIZE];
    const char* keyString = "0f1e2d3c4b5a69788796a5b4c3d2e1f0f0e1d2c3b4a5968778695a4b3c2d1e0f"; // 256-bit Key
    stringToWordArray(key, keyString);

    // Print the plaintext
    printf("Key: \n");
    printBigEndian(key, KEY_SIZE);
    // printLittleEndian(key, KEY_SIZE);

    u32 enc_roundkey[TOTAL_RK];
    leaEncKeySchedule(enc_roundkey, key);
    printEncRoundKeys(enc_roundkey);

    u32 plain[4] = { 0x00, };
    u32 cipher[4];

    const char* plainString = "303132333435363738393a3b3c3d3e3f"; // for 256-bit Key
    stringToWordArray(plain, plainString);

    printf("\nPlain-Text: \n");
    printBigEndian(plain, 4);
    // printLittleEndian(plain, 4);

    leaEncrypt(cipher, plain, enc_roundkey);
    
    printf("\nCipher-Text: \n");
    printBigEndian(cipher, sizeof(u32));
    printLittleEndian(cipher, sizeof(u32));

    // double enc_time = measure_time(leaEncrypt, plain, enc_roundkey, cipher);
    // printf("%.3f ns\n", enc_time*1000000000);

    u32 encrypted[4];
    u32 decrypted[4];

    memcpy(encrypted, cipher, 16);
    printf("\nEncrypted Text: \n");
    printLittleEndian(encrypted, sizeof(u32));

    u32 dec_roundkey[TOTAL_RK];
    leaDecKeySchedule(dec_roundkey, key);
    printDecRoundKeys(dec_roundkey);
    
    leaDecrypt(decrypted, encrypted, dec_roundkey);
    
    printf("\nDecrypted Text: \n");
    printBigEndian(decrypted, sizeof(32));
    // printLittleEndian(decrypted, sizeof(u32));

    // double dec_time = measure_time(leaDecrypt, encrypted, dec_roundkey, decrypted);
    // printf("%.3f ns\n", dec_time*1000000000);
}
#endif