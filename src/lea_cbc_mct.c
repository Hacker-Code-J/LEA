#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lea_cbc_movs.h"

void create_LEA_CBC_MCT_ReqFile(const char* pTxtFileName, const char* pReqFileName) {
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
        // if (strncmp(pLine, "COUNT =", 7) == 0) {
        //     if (!isFirstKey) fputc('\n', pReqFile);
        //     isFirstKey = 0;
        //     fputs(pLine, pReqFile);
        // } else if (strncmp(pLine, "KEY =", 5) == 0) {
        //     fputs(pLine, pReqFile);
        // } else if (strncmp(pLine, "IV =", 4) == 0 ||
        //            strncmp(pLine, "PT =", 4) == 0) {
        //     fputs(pLine, pReqFile);
        // }
        
        if (strncmp(pLine, "KEY =", 5) == 0) {
            // If not the first KEY, add a newline before writing the line
            if (!isFirstKey) fputc('\n', pReqFile);
            isFirstKey = 0;
            fputs(pLine, pReqFile);
        } else if (strncmp(pLine, "IV =", 4) == 0 ||
                   strncmp(pLine, "PT =", 4) == 0) {
            fputs(pLine, pReqFile);
        }
    }

    free(pLine);
    fclose(pTxtFile);
    fclose(pReqFile);

    printf("LEA128(CBC)MCT.req file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}

void create_LEA_CBC_MCT_FaxFile(const char* pTxtFileName, const char* pFaxFileName) {
    FILE *pTxtFile, *pFaxFile;
    char* pLine;
    size_t bufsize = MAX_LINE_LENGTH;
    int isFirstKey = 1; // Flag to check if it's the first KEY line

    pTxtFile = fopen(pTxtFileName, "r"); // LEA128(CBC)MCT.txt
    if (pTxtFile == NULL) {
        perror("Error opening input file");
        return;
    }

    // Open the .fax file for writing
    pFaxFile = fopen(pFaxFileName, "w"); // LEA128(CBC)MCT.fax
    if (pFaxFile == NULL) {
        perror("Error opening .fax file");
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


void create_LEA_CBC_MCT_RspFile(const char* pReqFileName, const char* pRspFileName) {
    FILE *pReqFile, *pRspFile;
    char* pLine;
    size_t bufsize = MAX_LINE_LENGTH;
    int isFirstKey = 1;
    
    CryptoData* pData = (CryptoData*)malloc(sizeof(CryptoData));
    if (pData == NULL) {
        perror("Unable to allocate memory");
        return;
    }
    memset(pData, 0, sizeof(CryptoData));

    pReqFile = fopen(pReqFileName, "r"); // LEA128(CBC)MCT.req
    if (pReqFile == NULL) {
        perror("Error opening .req file");
        return;
    }

    // Open the .req file for writing
    pRspFile = fopen(pRspFileName, "w"); // LEA128(CBC)MCT.rsp
    if (pRspFile == NULL) {
        perror("Error opening .rsp file");
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

    // Read the source file line by line
    int cnt = 0;
    while (fgets(pLine, bufsize, pReqFile)) {
        if (strncmp(pLine, "KEY =", 5) == 0) {
            if (!isFirstKey) fputc('\n', pRspFile);
            isFirstKey = 0;
            fprintf(pRspFile, "COUNT = %d\n", cnt++);
            pData->keyLength = wordLength(pLine + 6);
            pData->key = (u32*)malloc(pData->keyLength * sizeof(u32));
            if (pData->key == NULL) {
                perror("Unable to allocate memory for KEY");
                break;
            }
            parseHexLine(pData->key, pLine + 6, pData->keyLength);
            fputs(pLine, pRspFile);
        } else if (strncmp(pLine, "IV =", 4) == 0) {
            pData->iv = (u32*)malloc(4 * sizeof(u32));
            if (pData->iv == NULL) {
                perror("Unable to allocate memory for IV");
                freeCryptoData(pData);
                break;
            }
            parseHexLine(pData->iv, pLine + 5, 4);
            fputs(pLine, pRspFile);
        } else if (strncmp(pLine, "PT =", 4) == 0) {
            pData->dataLength = wordLength(pLine + 5);
            pData->pt = (u32*)malloc(pData->dataLength * sizeof(u32));
            if (pData->pt == NULL) {
                perror("Unable to allocate memory for PT");
                freeCryptoData(pData);
                break;
            }
            parseHexLine(pData->pt, pLine + 5, pData->dataLength);
            fputs(pLine, pRspFile);
        } else {
            if (!pData->dataLength) continue;
            pData->ct = (u32*)malloc(pData->dataLength * sizeof(u32));
            if (pData->ct == NULL) {
                perror("Unable to allocate memory for CT");
                freeCryptoData(pData);
                break;
            }

            fprintf(pRspFile, "CT = ");
            
            u32 _pt[pData->dataLength]; // Buffer to hold modified plaintext for each iteration
            memcpy(_pt, pData->pt, pData->dataLength * sizeof(u32)); // Initialize with original plaintext

            for (int j = 0; j < 1000; ++j) {
                // Encrypt using CBC_Encrypt_LEA
                CBC_Encrypt_LEA(pData->ct,
                                _pt, pData->dataLength,
                                pData->key, pData->keyLength,
                                pData->iv);
                // Prepare modifiedPt for the next iteration
                // First block
                _pt[0] = pData->iv[0]; // PT[j+1] = IV[i]

                // Subsequent blocks
                for (size_t i = 1; i < pData->dataLength; ++i) {
                    _pt[i] = pData->ct[i - 1]; // PT[j+1] = CT[j-1]
                }
            }
            // for (int idx = 0; idx < 1000; idx ++) {
            //     if (!idx) {
            //         CBC_Encrypt_LEA(pData->ct,
            //                 pData->pt, pData->dataLength,
            //                 pData->key, pData->keyLength,
            //                 pData->iv);        
            //     }
            // }
            // CBC_Encrypt_LEA(pData->ct,
            //                 pData->pt, pData->dataLength,
            //                 pData->key, pData->keyLength,
            //                 pData->iv);
            for (size_t i = 0; i < pData->dataLength; i++) {
                fprintf(pRspFile, "%08X", pData->ct[i]);
            }
            fprintf(pRspFile, "\n");
            freeCryptoData(pData);
            memset(pData, 0, sizeof(CryptoData));
        }
    }
    pData->ct = (u32*)malloc(pData->dataLength * sizeof(u32));
    if (pData->ct == NULL) {
        perror("Unable to allocate memory for CT");
        freeCryptoData(pData);
        return;
    }

    fprintf(pRspFile, "CT = ");
    CBC_Encrypt_LEA(pData->ct,
                    pData->pt, pData->dataLength,
                    pData->key, pData->keyLength,
                    pData->iv);
    for (size_t i = 0; i < pData->dataLength; i++) {
        fprintf(pRspFile, "%08X", pData->ct[i]);
    }
    fprintf(pRspFile, "\n");
    freeCryptoData(pData);
    
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