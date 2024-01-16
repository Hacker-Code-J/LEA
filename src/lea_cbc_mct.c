#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lea_cbc_movs.h"

void create_LEA128CBC_MCT_ReqFile(const char* pTxtFileName, const char* pReqFileName) {
    FILE *pTxtFile, *pReqFile;
    char* pLine;
    size_t bufsize = MAX_LINE_LENGTH;
    int isFirstKey = 1; // Flag to check if it's the first KEY line

    pTxtFile = fopen(pTxtFileName, "r"); // LEA128(CBC)MMT.txt
    if (pTxtFile == NULL) {
        perror("Error opening input file");
        return;
    }

    // Open the .req file for writing
    pReqFile = fopen(pReqFileName, "w"); // LEA128(CBC)MMT.req
    if (pReqFile == NULL) {
        perror("Error opening .req file");
        fclose(pTxtFile);
        return;
    }
    
    // Allocate initial buffer
    pLine = (char*)calloc(bufsize, sizeof(char)); // 500 x 1
    if(pLine == NULL) {
        perror("Unable to allocate memory");
        fclose(pTxtFile);
        fclose(pReqFile);
        return;
    }

    // Read the source file line by line
    while (fgets(pLine, bufsize, pTxtFile)) {
        if (strncmp(pLine, "KEY =", 5) == 0) {
            // If not the first KEY, add a newline before writing the line
            if (!isFirstKey) fputc('\n', pReqFile);
            isFirstKey = 0;
            fputs(pLine, pReqFile);
        } else if (strncmp(pLine, "IV =", 4) == 0 || strncmp(pLine, "PT =", 4) == 0) {
            fputs(pLine, pReqFile);
        }
    }

    free(pLine);
    fclose(pTxtFile);
    fclose(pReqFile);

    printf("LEA128(CBC)MCT.req file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}
void create_LEA128CBC_MCT_FaxFile(const char* pTxtFileName, const char* pFaxFileName) {
    FILE *pTxtFile, *pFaxFile;
    char* pLine;
    size_t bufsize = MAX_LINE_LENGTH;
    int isFirstKey = 1; // Flag to check if it's the first KEY line

    pTxtFile = fopen(pTxtFileName, "r"); // LEA128(CBC)MMT.txt
    if (pTxtFile == NULL) {
        perror("Error opening input file");
        return;
    }

    // Open the .req file for writing
    pFaxFile = fopen(pFaxFileName, "w"); // LEA128(CBC)MMT.fax
    if (pFaxFile == NULL) {
        perror("Error opening .req file");
        fclose(pTxtFile);
        return;
    }
    
    // Allocate initial buffer
    pLine = (char*)calloc(bufsize, sizeof(char)); // 500 x 1
    if(pLine == NULL) {
        perror("Unable to allocate memory");
        fclose(pTxtFile);
        fclose(pFaxFile);
        return;
    }

    // Read the source file line by line
    while (fgets(pLine, bufsize, pTxtFile)) {
        if (strncmp(pLine, "COUNT =", 7) == 0) {
            if (!isFirstKey) fputc('\n', pFaxFile);
            isFirstKey = 0;
            fputs(pLine, pFaxFile);
        } else if (strncmp(pLine, "KEY =", 5) == 0) {
            fputs(pLine, pFaxFile);
        } else if (strncmp(pLine, "IV =", 4) == 0 ||
                   strncmp(pLine, "PT =", 4) == 0 ||
                   strncmp(pLine, "CT =", 4) == 0) {
            fputs(pLine, pFaxFile);
        }
    }

    free(pLine);
    fclose(pTxtFile);
    fclose(pFaxFile);

    printf("LEA128(CBC)MCT.fax file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}

void create_LEA128CBC_MCT_RspFile(const char* pReqFileName, const char* pRspFileName) {
    FILE *pReqFile, *pRspFile;
    char* pLine;
    size_t bufsize = MAX_LINE_LENGTH;
    int isFirstKey = 1;
    
    CryptoData* pData = (CryptoData*)malloc(sizeof(CryptoData));
    if (pData == NULL) {
        perror("Unable to allocate memory");
        exit(1);
    }
    memset(pData, 0, sizeof(CryptoData));

    pReqFile = fopen(pReqFileName, "r"); // LEA128(CBC)MMT.req
    if (pReqFile == NULL) {
        perror("Error opening input file");
        return;
    }

    // Open the .req file for writing
    pRspFile = fopen(pRspFileName, "w"); // LEA128(CBC)MMT.rsp
    if (pRspFile == NULL) {
        perror("Error opening .req file");
        fclose(pReqFile);
        return;
    }
    
    // Allocate initial buffer
    pLine = (char*)calloc(bufsize, sizeof(char));
    if(pLine == NULL) {
        perror("Unable to allocate memory");
        fclose(pReqFile);
        fclose(pRspFile);
        return;
    }

    int cnt = 0;
    // Read the source file line by line
    while (fgets(pLine, bufsize, pReqFile)) {
        if (pData->ptLength != 0 && pData->ctLength == 0) {
            pData->ctLength = pData->ptLength;
            pData->ct = (u32*)malloc(pData->ctLength * sizeof(u32));
            if (pData->ct == NULL) {
                perror("Unable to allocate memory for CT");
                free(pData->pt);
                break;
            }

            fprintf(pRspFile, "CT = ");
            cbcEncrypt(pData);
            // CBC_Encrypt_LEA(pData->ct, pData->pt, pData->ptLength, pData->key, pData->iv);
            for (size_t i = 0; i < pData->ctLength; i++) {
                fprintf(pRspFile, "%08X", pData->ct[i]);
            }
            fprintf(pRspFile, "\n");
            freeCryptoData(pData);
            memset(pData, 0, sizeof(CryptoData));
        } else if (strncmp(pLine, "KEY =", 5) == 0) {
            if (!isFirstKey) fputc('\n', pRspFile);
            isFirstKey = 0;
            fprintf(pRspFile, "COUNT = %d\n", cnt++);
            parseHexLine(pData->key, pLine + 6);
            fputs(pLine, pRspFile);
        } else if (strncmp(pLine, "IV =", 4) == 0) {
            parseHexLine(pData->iv, pLine + 5);
            fputs(pLine, pRspFile);
        } else if (strncmp(pLine, "PT =", 4) == 0) {
            pData->ptLength = determineLength(pLine + 5);
            pData->pt = (u32*)malloc(pData->ptLength * sizeof(u32));
            if (pData->pt == NULL) {
                perror("Unable to allocate memory for PT");
                break;
            }
            parseHexLine(pData->pt, pLine + 5);
            fputs(pLine, pRspFile);
        }
    }
    pData->ctLength = pData->ptLength;
    pData->ct = (u32*)malloc(pData->ctLength * sizeof(u32));
    if (pData->ct == NULL) {
        perror("Unable to allocate memory for CT");
        free(pData->pt);
    }

    fprintf(pRspFile, "CT = ");
    cbcEncrypt(pData);
    for (size_t i = 0; i < pData->ctLength; i++) {
        fprintf(pRspFile, "%08X", pData->ct[i]);
    }
    fprintf(pRspFile, "\n");

    free(pData);  
    free(pLine);
    fclose(pReqFile);
    fclose(pRspFile);

    printf("LEA128(CBC)MCT.rsp file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}

#if 0
void customEncrypt(u32* key, u32* iv, u32* pt, size_t keylen) {
    u32 ct[1000 * BLOCK_SIZE]; // Assuming 1000 blocks of ciphertext
    u32 newKey[BLOCK_SIZE];
    u32 temp[BLOCK_SIZE];
    u32 roundKeys[TOTAL_RK];

    for (int i = 0; i < 100; i++) {
        printf("Key[%d]: ", i);
        for (int k = 0; k < BLOCK_SIZE; k++) printf("%u ", key[k]);
        printf("\n");

        printf("IV[%d]: ", i);
        for (int k = 0; k < BLOCK_SIZE; k++) printf("%u ", iv[k]);
        printf("\n");

        printf("PT[0]: %u\n", pt[0]);

        leaEncKeySchedule(roundKeys, key);

        for (int j = 0; j < 1000; j++) {
            if (j == 0) {
                for (int k = 0; k < BLOCK_SIZE; k++) {
                    temp[k] = pt[k] ^ iv[k];
                }
            } else {
                for (int k = 0; k < BLOCK_SIZE; k++) {
                    temp[k] = pt[j * BLOCK_SIZE + k] ^ ct[(j - 1) * BLOCK_SIZE + k];
                }
            }

            leaEncrypt(&ct[j * BLOCK_SIZE], temp, roundKeys);

            // Print CT[j]
            printf("CT[%d]: ", j);
            for (int k = 0; k < BLOCK_SIZE; k++) printf("%u ", ct[j * BLOCK_SIZE + k]);
            printf("\n");
        }

        // Key updating logic
        if (keylen == 128) {
            for (int k = 0; k < BLOCK_SIZE; k++) {
                newKey[k] = key[k] ^ ct[999 * BLOCK_SIZE + k];
            }
        } else if (keylen == 192) {
            // Assuming CT64 implies taking 64 bits (2 u32) from the CT array
            for (int k = 0; k < BLOCK_SIZE; k++) {
                newKey[k] = key[k] ^ ((ct[998 * BLOCK_SIZE + k] << 32) | ct[999 * BLOCK_SIZE + k]);
            }
        } else if (keylen == 256) {
            for (int k = 0; k < BLOCK_SIZE; k++) {
                newKey[k] = key[k] ^ (ct[998 * BLOCK_SIZE + k] | ct[999 * BLOCK_SIZE + k]);
            }
        }

        // Update IV and PT for the next round
        memcpy(iv, &ct[999 * BLOCK_SIZE], BLOCK_SIZE * sizeof(u32));
        memcpy(pt, &ct[998 * BLOCK_SIZE], BLOCK_SIZE * sizeof(u32));

        // Copy the new key to the key array
        memcpy(key, newKey, BLOCK_SIZE * sizeof(u32));
    }
}
#endif
// void MOVS_LEA128CBC_MCT_TEST();