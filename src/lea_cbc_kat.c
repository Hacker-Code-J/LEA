#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lea_cbc_movs.h"

void create_LEA_CBC_KAT_ReqFile(const char* pTxtFileName, const char* pReqFileName) {
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

    printf("LEA128(CBC)KAT.req file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}

void create_LEA_CBC_KAT_FaxFile(const char* pTxtFileName, const char* pFaxFileName) {
FILE *pTxtFile, *pFaxFile;
    char* pLine;
    size_t bufsize = MAX_LINE_LENGTH;
    int isFirstKey = 1; // Flag to check if it's the first KEY line

    pTxtFile = fopen(pTxtFileName, "r"); // LEA128(CBC)MMT.txt
    if (pTxtFile == NULL) {
        perror("Error opening input file");
        return;
    }

    // Open the .fax file for writing
    pFaxFile = fopen(pFaxFileName, "w"); // LEA128(CBC)MMT.fax
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
        if (strncmp(pLine, "KEY =", 5) == 0) {
            // If not the first KEY, add a newline before writing the line
            if (!isFirstKey) fputc('\n', pFaxFile);
            isFirstKey = 0;
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

    printf("LEA128(CBC)KAT.fax file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}

void create_LEA_CBC_KAT_RspFile(const char* pReqFileName, const char* pRspFileName) {
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

    pReqFile = fopen(pReqFileName, "r"); // LEA128(CBC)KAT.req
    if (pReqFile == NULL) {
        perror("Error opening .req file");
        return;
    }

    // Open the .rsp file for writing
    pRspFile = fopen(pRspFileName, "w"); // LEA128(CBC)KAT.rsp
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
    while (fgets(pLine, bufsize, pReqFile)) {
        if (strncmp(pLine, "KEY =", 5) == 0) {
            if (!isFirstKey) fputc('\n', pRspFile);
            isFirstKey = 0;
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
            CBC_Encrypt_LEA(pData->ct,
                            pData->pt, pData->dataLength,
                            pData->key, pData->keyLength,
                            pData->iv, LEA128);
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
                    pData->iv, LEA128);
    for (size_t i = 0; i < pData->dataLength; i++) {
        fprintf(pRspFile, "%08X", pData->ct[i]);
    }
    fprintf(pRspFile, "\n");
    freeCryptoData(pData);
    
    free(pLine);
    fclose(pReqFile);
    fclose(pRspFile);

    printf("LEA128(CBC)KAT.rsp file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}
void MOVS_LEA128CBC_KAT_TEST() {
    const char* folderPath = "../LEA128(CBC)MOVS/";
    char txtFileName[50];
    char reqFileName[50];
    char faxFileName[50];
    char rspFileName[50];
    
    // Construct full paths for input and output files
    snprintf(txtFileName, sizeof(txtFileName), "%s%s", folderPath, "LEA128(CBC)KAT.txt");
    snprintf(reqFileName, sizeof(reqFileName), "%s%s", folderPath, "LEA128(CBC)KAT.req");
    snprintf(faxFileName, sizeof(faxFileName), "%s%s", folderPath, "LEA128(CBC)KAT.fax");
    snprintf(rspFileName, sizeof(rspFileName), "%s%s", folderPath, "LEA128(CBC)KAT.rsp");
    
    create_LEA_CBC_KAT_ReqFile(txtFileName, reqFileName);
    create_LEA_CBC_KAT_FaxFile(txtFileName, faxFileName);
    create_LEA_CBC_KAT_RspFile(reqFileName, rspFileName);

    printf("\nLEA128-CBC-KAT-TEST:\n");

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
    int totalTests = 275; // Assuming a total of 275 tests
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