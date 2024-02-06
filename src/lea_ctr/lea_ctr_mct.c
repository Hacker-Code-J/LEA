/**
 * @file lea_ctr_mct.c
 * @brief Implementation of the Monte Carlo Test (MCT) for the CTR mode of LEA.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lea_movs.h"

void create_LEA_CTR_MCT_ReqFile(const char* pTxtFileName, const char* pReqFileName) {
    FILE *pTxtFile, *pReqFile;
    char* pLine;
    size_t bufsize = MAX_LINE_LENGTH;
    int isFirstPT = 1; // Flag to check if it's the first PT line

    pTxtFile = fopen(pTxtFileName, "r"); // LEA128(CTR)MCT.txt
    if (pTxtFile == NULL) {
        perror("Error opening input file");
        return;
    }

    // Open the .req file for writing
    pReqFile = fopen(pReqFileName, "w"); // LEA128(CTR)MCT.req
    if (pReqFile == NULL) {
        perror("Error opening .req file");
        fclose(pTxtFile);
        return;
    }
    
    // Allocate initial buffer
    pLine = (char*)calloc(bufsize, sizeof(char));
    if(pLine == NULL) {
        perror("Unable to allocate memory");
        fclose(pTxtFile);
        fclose(pReqFile);
        return;
    }

    while (fgets(pLine, bufsize, pTxtFile)) {
        // Exit the loop if the line is empty (only contains a newline or nothing)
        if (!isFirstPT) break;
        if (strncmp(pLine, "KEY =", 5) == 0) {
            fputs(pLine, pReqFile);
        } else if (strncmp(pLine, "CTR =", 5) == 0) {
            fputs(pLine, pReqFile);
        } else if (strncmp(pLine, "PT =", 4) == 0) {
            isFirstPT = 0;
            fputs(pLine, pReqFile);
        }
    }

    free(pLine);
    fclose(pTxtFile);
    fclose(pReqFile);

    printf("LEA128(CTR)MCT.req file has been successfully created in 'LEA128(CTR)MOVS' folder.\n");
}

void create_LEA_CTR_MCT_FaxFile(const char* pTxtFileName, const char* pFaxFileName) {
    FILE *pTxtFile, *pFaxFile;
    char* pLine;
    size_t bufsize = MAX_LINE_LENGTH;
    int isFirstKey = 1; // Flag to check if it's the first KEY line

    pTxtFile = fopen(pTxtFileName, "r"); // LEA128(CTR)MCT.txt
    if (pTxtFile == NULL) {
        perror("Error opening input file");
        return;
    }

    // Open the .fax file for writing
    pFaxFile = fopen(pFaxFileName, "w"); // LEA128(CTR)MCT.fax
    if (pFaxFile == NULL) {
        perror("Error opening .fax file");
        fclose(pTxtFile);
        return;
    }
    
    // Allocate initial buffer
    pLine = (char*)calloc(bufsize, sizeof(char));
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
        } else if (strncmp(pLine, "CTR =", 5) == 0 ||
                   strncmp(pLine, "PT =", 4) == 0 ||
                   strncmp(pLine, "CT =", 4) == 0) {
            fputs(pLine, pFaxFile);
        }
    }

    free(pLine);
    fclose(pTxtFile);
    fclose(pFaxFile);

    printf("LEA128(CTR)MCT.fax file has been successfully created in 'LEA128(CTR)MOVS' folder.\n");
}


void create_LEA_CTR_MCT_RspFile(const char* pReqFileName, const char* pRspFileName) {
    FILE *pReqFile, *pRspFile;
    char* pLine;
    size_t bufsize = MAX_LINE_LENGTH;
    int isFirstCnt = 1; // Flag to check if it's the first COUNT line
    
    CryptoData* pData = (CryptoData*)calloc(1, sizeof(CryptoData));
    if (pData == NULL) {
        perror("Unable to allocate memory");
        return;
    }
    memset(pData, 0, sizeof(CryptoData));

    pReqFile = fopen(pReqFileName, "r"); // LEA128(CTR)MCT.req
    if (pReqFile == NULL) {
        perror("Error opening .req file");
        return;
    }

    // Open the .req file for writing
    pRspFile = fopen(pRspFileName, "w"); // LEA128(CTR)MCT.rsp
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

    while (fgets(pLine, bufsize, pReqFile)) {
        if (strncmp(pLine, "KEY =", 5) == 0) {
            pData->keyLength = wordLength(pLine + 6);
            pData->key = (u32*)malloc(pData->keyLength * sizeof(u32));
            if (pData->key == NULL) {
                perror("Unable to allocate memory for KEY");
                break;
            }
            parseHexLine(pData->key, pLine + 6, pData->keyLength);
        } else if (strncmp(pLine, "CTR =", 5) == 0) {
            pData->iv = (u32*)malloc(4 * sizeof(u32));
            if (pData->iv == NULL) {
                perror("Unable to allocate memory for IV");
                freeCryptoData(pData);
                break;
            }
            parseHexLine(pData->iv, pLine + 6, 4);
        } else if (strncmp(pLine, "PT =", 4) == 0) {
            pData->dataLength = wordLength(pLine + 5);
            pData->pt = (u32*)malloc(pData->dataLength * sizeof(u32));
            if (pData->pt == NULL) {
                perror("Unable to allocate memory for PT");
                freeCryptoData(pData);
                break;
            }
            parseHexLine(pData->pt, pLine + 5, pData->dataLength);
        }
    }

    // Constants
    const size_t keyLength = pData->keyLength;
    // const size_t dataLength = pData->dataLength;
    const size_t blockSize = 4; // 128 bits
    const size_t numRounds = 100;
    const size_t numBlocks = 1000;

    // Buffers
    u32 _key[keyLength];
    u32 _ctr[blockSize];
    u32 _pt[blockSize];
    u32 _ct[blockSize];

    // Initialize with initial values
    memcpy(_key, pData->key, keyLength * sizeof(u32));
    memcpy(_ctr, pData->iv, blockSize * sizeof(u32));
    memcpy(_pt, pData->pt, blockSize * sizeof(u32));

    for (size_t i = 0; i < numRounds; i++) {
        if (!isFirstCnt) fputc('\n', pRspFile);
        isFirstCnt = 0;
        fprintf(pRspFile, "COUNT = %lu\n", i);
        fprintf(pRspFile, "KEY = ");
        printHexToFile(pRspFile, _key, keyLength);

        fprintf(pRspFile, "CTR = ");
        printHexToFile(pRspFile, _ctr, blockSize);

        fprintf(pRspFile, "PT = ");
        printHexToFile(pRspFile, _pt, blockSize);

        for (size_t j = 0; j < numBlocks; j++) {
            leaEncrypt(_ct, _ctr, _key, LEA128); // LEA encryption
            for (size_t k = 0; k < blockSize; k++) {
                _ct[k] ^= _pt[k]; // XOR with PT
                _pt[k] = _ct[k]; // PT for next block is current CT
            }
            if (++_ctr[3] == 0) {
                if (++_ctr[2] == 0) {
                    if (++_ctr[1] == 0) {
                        ++_ctr[0];
                }
            }
        }
        }
        // Output last CT
        fprintf(pRspFile, "CT = ");
        printHexToFile(pRspFile, _ct, blockSize);

        // Key for next round
        for (size_t k = 0; k < keyLength; k++) _key[k] ^= _ct[k % blockSize];
        memcpy(_pt, _ct, blockSize * sizeof(uint32_t)); // PT[0] for next round
    }

    freeCryptoData(pData);
    free(pLine);
    fclose(pReqFile);
    fclose(pRspFile);

    printf("LEA128(CTR)MCT.rsp file has been successfully created in 'LEA128(CTR)MOVS' folder.\n");
}

void MOVS_LEA128CTR_MCT_TEST(void) {
    const char* folderPath = "../LEA128(CTR)MOVS/";
    char txtFileName[50];
    char reqFileName[50];
    char faxFileName[50];
    char rspFileName[50];
    
    // Construct full paths for input and output files
    snprintf(txtFileName, sizeof(txtFileName), "%s%s", folderPath, "LEA128(CTR)MCT.txt");
    snprintf(reqFileName, sizeof(reqFileName), "%s%s", folderPath, "LEA128(CTR)MCT.req");
    snprintf(faxFileName, sizeof(faxFileName), "%s%s", folderPath, "LEA128(CTR)MCT.fax");
    snprintf(rspFileName, sizeof(rspFileName), "%s%s", folderPath, "LEA128(CTR)MCT.rsp");
    
    create_LEA_CTR_MCT_ReqFile(txtFileName, reqFileName);
    create_LEA_CTR_MCT_FaxFile(txtFileName, faxFileName);
    create_LEA_CTR_MCT_RspFile(reqFileName, rspFileName);

    printf("\nLEA128-CTR-MCT-TEST:\n");

    FILE* file1 = fopen(faxFileName, "r");
    FILE* file2 = fopen(rspFileName, "r");

    if (!file1 || !file2) {
        perror("Error opening files");
        return;
    }

    CryptoData* pData1 = (CryptoData*)malloc(sizeof(CryptoData));
    CryptoData* pData2 = (CryptoData*)malloc(sizeof(CryptoData));
    if (pData1 == NULL || pData2 == NULL) {
        perror("Unable to allocate memory");
        return;
    }
    int result = 1; // Default to pass
    int idx = 1;
    int totalTests = 100; // Assuming a total of 10 tests
    int passedTests = 0;
    while (idx <= totalTests) {
        // sReset the structures for the next iteration
        memset(pData1, 0, sizeof(CryptoData));
        memset(pData2, 0, sizeof(CryptoData));
        if (!readCryptoData(file1, pData1) || !readCryptoData(file2, pData2)) {
            result = 0; // Indicate failure if read fails
            break;
        }

        if (!compareCryptoData(pData1, pData2)) {
            result = 0; // Fail
            printf("\nFAIL\n");
            break;
        }

        // Free the dynamically allocated memory
        freeCryptoData(pData1);
        freeCryptoData(pData2);

        passedTests++;
        printProgressBar(idx++, totalTests);
    }

    printf("\n\nTesting Summary:\n");
    printf("Passed: %d/%d\n", passedTests, totalTests);
    if (result) {
        printf("Perfect PASS !!!\n\n");
    } else {
        printf("Some tests FAILED.\n\n");
    }

    fclose(file1);
    fclose(file2);
}