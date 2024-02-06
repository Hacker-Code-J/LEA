/**
 * @file lea_movs.c
 * @brief Implementation related to Mode of Operations Validation System (MOVS) or other structures/functions in LEA.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lea_movs.h"

void printProgressBar(int current, int total) {
    int width = 50; // Width of the progress bar
    float progress = (float)current / total;
    int pos = width * progress;

    // ANSI Escape Codes for colors
    const char* GREEN = "\x1b[32m";
    const char* YELLOW = "\x1b[33m";
    const char* RED = "\x1b[31m";
    const char* RESET = "\x1b[0m";

    printf("\r[");
    for (int i = 0; i < width; ++i) {
        if (i < pos) printf("%s=", GREEN); // White for completed part
        else if (i == pos) printf("%s>", YELLOW); // Yellow for current position
        else printf("%s ", RED); // Red for remaining part
    }
    printf("%s] %d%% (%d/%d)", RESET, (int)(progress * 100.0), current, total);
}

void freeCryptoData(CryptoData* pData) {
    if (pData != NULL) {
        free(pData->key);
        free(pData->iv);
        free(pData->pt);
        free(pData->ct);
        pData->key = NULL;
        pData->iv = NULL;
        pData->pt = NULL;
        pData->ct = NULL;
    }
}

void parseHexLine(u32* pU32dst, const char* pI8src, size_t length) {
    for (size_t i = 0; i < length; i++) {
        u32 value;
        // Ensure not to read beyond the line's end
        if (sscanf(pI8src + i * 8, "%8x", &value) != 1) {
            // Handle parsing error, such as setting a default value or logging an error
            pU32dst[i] = 0; // Example: set to zero if parsing fails
        } else {
            pU32dst[i] = value;
        }
    }
}

size_t wordLength(const char* pHexString) {
    size_t hexLength = strlen(pHexString);

    size_t u32Length = hexLength / 8; // 0x00: 1-byte

    // If the hex string length is not a multiple of 8, add an extra element
    // if (hexLength % 8 != 0) {
    //     u32Length++;
    // }

    return u32Length;
}

bool readCryptoData(FILE* fp, CryptoData* pData) {
    char line[MAX_LINE_LENGTH];
    bool keyFound = false,
         ivFound = false,
         ptFound = false,
         ctFound = false;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "KEY =", 5) == 0 && !keyFound) {
            pData->keyLength = wordLength(line + 6);
            pData->key = (u32*)calloc(pData->keyLength, sizeof(u32));
            if (pData->key == NULL) return false;
            parseHexLine(pData->key, line, pData->keyLength);
            keyFound = true;
        } else if ((strncmp(line, "IV =", 4) == 0 || strncmp(line, "CTR =", 5) == 0) && !ivFound) {
            if (pData->key == NULL) {
                perror("Key not found");
                return false;
            }
            pData->iv = (u32*)calloc(pData->keyLength, sizeof(u32));
            if (pData->iv == NULL) return false;
            parseHexLine(pData->iv, line, pData->keyLength);
            ivFound = true;
        } else if (strncmp(line, "PT =", 4) == 0 && !ptFound) {
            pData->dataLength = wordLength(line + 5);
            pData->pt = (u32*)calloc(pData->dataLength, sizeof(u32));
            if (pData->pt == NULL) return false;
            parseHexLine(pData->pt, line, pData->dataLength);
            ptFound = true;
        } else if (strncmp(line, "CT =", 4) == 0 && !ctFound) {
            if (pData->pt == NULL) {
                perror("Plaintext not found");
                return false;
            }
            pData->ct = (u32*)calloc(pData->dataLength, sizeof(u32));
            if (pData->ct == NULL) return false;
            parseHexLine(pData->ct, line, pData->dataLength);
            ctFound = true;
        }
    }

    // if (!keyFound || !ivFound || !ptFound || !ctFound) {
    //     perror("Incomplete crypto data");
    //     return false;
    // }

    return true; // Return true on successful read
}

bool compareCryptoData(const CryptoData* pData1_mct, const CryptoData* pData2_mct) {
    if (pData1_mct->keyLength != pData2_mct->keyLength ||
        pData1_mct->dataLength != pData2_mct->dataLength) {
        return false;
    }

    size_t keyLen = pData1_mct->keyLength;
    size_t dataLen = pData1_mct->dataLength;
    
    // Compare contents key and iv
    for (size_t i = 0; i < keyLen; i++) {
        if (pData1_mct->key[i] != pData2_mct->key[i] ||
            pData1_mct->iv[i] != pData2_mct->iv[i]) {
            return false;
        }
    }

    // Compare contents pt and ct
    for (size_t i = 0; i < dataLen; i++) {
        if (pData1_mct->pt[i] != pData2_mct->pt[i] ||
            pData1_mct->ct[i] != pData2_mct->ct[i]) {
            return false;
        }
    }

    return true; // All comparisons passed, data structures are equal
}

// Utility function to print buffer in hexadecimal format to a file
void printHexToFile(FILE* pFile, const u32* data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        fprintf(pFile, "%08X", data[i]);
    }
    fprintf(pFile, "\n"); // Add a newline for formatting, if needed
}

void MOVS_LEA128CBC(void) {
    const char* folderPath = "../LEA128(CBC)MOVS/";
    char txtFileName_kat[50], txtFileName_mmt[50], txtFileName_mct[50];
    char reqFileName_kat[50], reqFileName_mmt[50], reqFileName_mct[50];
    char faxFileName_kat[50], faxFileName_mmt[50], faxFileName_mct[50];
    char rspFileName_kat[50], rspFileName_mmt[50], rspFileName_mct[50];
    
    // Construct full paths for input and output files
    snprintf(txtFileName_kat, sizeof(txtFileName_kat), "%s%s", folderPath, "LEA128(CBC)KAT.txt");
    snprintf(reqFileName_kat, sizeof(reqFileName_kat), "%s%s", folderPath, "LEA128(CBC)KAT.req");
    snprintf(faxFileName_kat, sizeof(faxFileName_kat), "%s%s", folderPath, "LEA128(CBC)KAT.fax");
    snprintf(rspFileName_kat, sizeof(rspFileName_kat), "%s%s", folderPath, "LEA128(CBC)KAT.rsp");
    
    snprintf(txtFileName_mmt, sizeof(txtFileName_mmt), "%s%s", folderPath, "LEA128(CBC)MMT.txt");
    snprintf(reqFileName_mmt, sizeof(reqFileName_mmt), "%s%s", folderPath, "LEA128(CBC)MMT.req");
    snprintf(faxFileName_mmt, sizeof(faxFileName_mmt), "%s%s", folderPath, "LEA128(CBC)MMT.fax");
    snprintf(rspFileName_mmt, sizeof(rspFileName_mmt), "%s%s", folderPath, "LEA128(CBC)MMT.rsp");

    snprintf(txtFileName_mct, sizeof(txtFileName_mct), "%s%s", folderPath, "LEA128(CBC)MCT.txt");
    snprintf(reqFileName_mct, sizeof(reqFileName_mct), "%s%s", folderPath, "LEA128(CBC)MCT.req");
    snprintf(faxFileName_mct, sizeof(faxFileName_mct), "%s%s", folderPath, "LEA128(CBC)MCT.fax");
    snprintf(rspFileName_mct, sizeof(rspFileName_mct), "%s%s", folderPath, "LEA128(CBC)MCT.rsp");
    
    create_LEA_CBC_KAT_ReqFile(txtFileName_kat, reqFileName_kat);
    create_LEA_CBC_MMT_ReqFile(txtFileName_mmt, reqFileName_mmt);
    create_LEA_CBC_MCT_ReqFile(txtFileName_mct, reqFileName_mct);

    create_LEA_CBC_KAT_FaxFile(txtFileName_kat, faxFileName_kat);
    create_LEA_CBC_MMT_FaxFile(txtFileName_mmt, faxFileName_mmt);
    create_LEA_CBC_MCT_FaxFile(txtFileName_mct, faxFileName_mct);

    create_LEA_CBC_KAT_RspFile(reqFileName_kat, rspFileName_kat);
    create_LEA_CBC_MMT_RspFile(reqFileName_mmt, rspFileName_mmt);
    create_LEA_CBC_MCT_RspFile(reqFileName_mct, rspFileName_mct);

    /********** Known Answer Test **********/
    printf("\nLEA128-CBC-KAT-TEST:\n");

    FILE* file1_kat = fopen(faxFileName_kat, "r");
    FILE* file2_kat = fopen(rspFileName_kat, "r");

    if (!file1_kat || !file2_kat) {
        perror("Error opening files");
        return;
    }

    CryptoData* pData1_kat = (CryptoData*)malloc(sizeof(CryptoData));
    CryptoData* pData2_kat = (CryptoData*)malloc(sizeof(CryptoData));
    if (pData1_kat == NULL || pData2_kat == NULL) {
        perror("Unable to allocate memory");
        return;
    }
    int result = 1; // Default to pass
    int idx = 1;
    int totalTests = 275; // Assuming a total of 10 tests
    int passedTests = 0;
    while (idx <= totalTests) {
        // Reset the structures for the next iteration
        memset(pData1_kat, 0, sizeof(CryptoData));
        memset(pData2_kat, 0, sizeof(CryptoData));
        if (!readCryptoData(file1_kat, pData1_kat) || !readCryptoData(file2_kat, pData2_kat)) {
            result = 0; // Indicate failure if read fails
            break;
        }

        if (!compareCryptoData(pData1_kat, pData2_kat)) {
            result = 0; // Fail
            printf("\nFAIL\n");
            break;
        }

        // Free the dynamically allocated memory
        freeCryptoData(pData1_kat);
        freeCryptoData(pData2_kat);

        passedTests++;
        printProgressBar(idx++, totalTests);
    }

    // printf("\n\nKAT Summary:\n");
    // printf("Passed: %d/%d\n", passedTests, totalTests);
    if (result) printf("\nPASS\n");
    // else printf("Some tests FAILED.\n\n");

    fclose(file1_kat);
    fclose(file2_kat);

    /********** Multi-block Message Test **********/
    printf("\nLEA128-CBC-MMT-TEST:\n");

    FILE* file1_mmt = fopen(faxFileName_mmt, "r");
    FILE* file2_mmt = fopen(rspFileName_mmt, "r");

    if (!file1_mmt || !file2_mmt) {
        perror("Error opening files");
        return;
    }

    CryptoData* pData1_mmt = (CryptoData*)malloc(sizeof(CryptoData));
    CryptoData* pData2_mmt = (CryptoData*)malloc(sizeof(CryptoData));
    if (pData1_mmt == NULL || pData2_mmt == NULL) {
        perror("Unable to allocate memory");
        return;
    }
    result = 1; // Default to pass
    idx = 1;
    totalTests = 10; // Assuming a total of 10 tests
    passedTests = 0;
    while (idx <= totalTests) {
        // Reset the structures for the next iteration
        memset(pData1_mmt, 0, sizeof(CryptoData));
        memset(pData2_mmt, 0, sizeof(CryptoData));
        if (!readCryptoData(file1_mmt, pData1_mmt) || !readCryptoData(file2_mmt, pData2_mmt)) {
            result = 0; // Indicate failure if read fails
            break;
        }

        if (!compareCryptoData(pData1_mmt, pData2_mmt)) {
            result = 0; // Fail
            printf("\nFAIL\n");
            break;
        }

        // Free the dynamically allocated memory
        freeCryptoData(pData1_mmt);
        freeCryptoData(pData2_mmt);

        passedTests++;
        printProgressBar(idx++, totalTests);
    }

    // printf("\n\nMMT Summary:\n");
    // printf("Passed: %d/%d\n", passedTests, totalTests);
    if (result) printf("\nPASS\n");
    // else printf("Some tests FAILED.\n\n");

    fclose(file1_mmt);
    fclose(file2_mmt);

    /********** Monte Carlo Test **********/
    printf("\nLEA128-CBC-MCT-TEST:\n");

    FILE* file1_mct = fopen(faxFileName_mct, "r");
    FILE* file2_mct = fopen(rspFileName_mct, "r");

    if (!file1_mct || !file2_mct) {
        perror("Error opening files");
        return;
    }

    CryptoData* pData1_mct = (CryptoData*)malloc(sizeof(CryptoData));
    CryptoData* pData2_mct = (CryptoData*)malloc(sizeof(CryptoData));
    if (pData1_mct == NULL || pData2_mct == NULL) {
        perror("Unable to allocate memory");
        return;
    }
    result = 1; // Default to pass
    idx = 1;
    totalTests = 100; // Assuming a total of 10 tests
    passedTests = 0;
    while (idx <= totalTests) {
        // sReset the structures for the next iteration
        memset(pData1_mct, 0, sizeof(CryptoData));
        memset(pData2_mct, 0, sizeof(CryptoData));
        if (!readCryptoData(file1_mct, pData1_mct) || !readCryptoData(file2_mct, pData2_mct)) {
            result = 0; // Indicate failure if read fails
            break;
        }

        if (!compareCryptoData(pData1_mct, pData2_mct)) {
            result = 0; // Fail
            printf("\nFAIL\n");
            break;
        }

        // Free the dynamically allocated memory
        freeCryptoData(pData1_mct);
        freeCryptoData(pData2_mct);

        passedTests++;
        printProgressBar(idx++, totalTests);
    }

    // printf("\n\nMCT Summary:\n");
    // printf("Passed: %d/%d\n", passedTests, totalTests);
    if (result) printf("\nPASS\n");
    // else printf("Some tests FAILED.\n\n");

    fclose(file1_mct);
    fclose(file2_mct);
}

void MOVS_LEA128CTR(void) {
    const char* folderPath = "../LEA128(CTR)MOVS/";
    char txtFileName_kat[50], txtFileName_mmt[50], txtFileName_mct[50];
    char reqFileName_kat[50], reqFileName_mmt[50], reqFileName_mct[50];
    char faxFileName_kat[50], faxFileName_mmt[50], faxFileName_mct[50];
    char rspFileName_kat[50], rspFileName_mmt[50], rspFileName_mct[50];
    
    // Construct full paths for input and output files
    snprintf(txtFileName_kat, sizeof(txtFileName_kat), "%s%s", folderPath, "LEA128(CTR)KAT.txt");
    snprintf(reqFileName_kat, sizeof(reqFileName_kat), "%s%s", folderPath, "LEA128(CTR)KAT.req");
    snprintf(faxFileName_kat, sizeof(faxFileName_kat), "%s%s", folderPath, "LEA128(CTR)KAT.fax");
    snprintf(rspFileName_kat, sizeof(rspFileName_kat), "%s%s", folderPath, "LEA128(CTR)KAT.rsp");
    
    snprintf(txtFileName_mmt, sizeof(txtFileName_mmt), "%s%s", folderPath, "LEA128(CTR)MMT.txt");
    snprintf(reqFileName_mmt, sizeof(reqFileName_mmt), "%s%s", folderPath, "LEA128(CTR)MMT.req");
    snprintf(faxFileName_mmt, sizeof(faxFileName_mmt), "%s%s", folderPath, "LEA128(CTR)MMT.fax");
    snprintf(rspFileName_mmt, sizeof(rspFileName_mmt), "%s%s", folderPath, "LEA128(CTR)MMT.rsp");

    snprintf(txtFileName_mct, sizeof(txtFileName_mct), "%s%s", folderPath, "LEA128(CTR)MCT.txt");
    snprintf(reqFileName_mct, sizeof(reqFileName_mct), "%s%s", folderPath, "LEA128(CTR)MCT.req");
    snprintf(faxFileName_mct, sizeof(faxFileName_mct), "%s%s", folderPath, "LEA128(CTR)MCT.fax");
    snprintf(rspFileName_mct, sizeof(rspFileName_mct), "%s%s", folderPath, "LEA128(CTR)MCT.rsp");
    
    create_LEA_CTR_KAT_ReqFile(txtFileName_kat, reqFileName_kat);
    create_LEA_CTR_MMT_ReqFile(txtFileName_mmt, reqFileName_mmt);
    create_LEA_CTR_MCT_ReqFile(txtFileName_mct, reqFileName_mct);

    create_LEA_CTR_KAT_FaxFile(txtFileName_kat, faxFileName_kat);
    create_LEA_CTR_MMT_FaxFile(txtFileName_mmt, faxFileName_mmt);
    create_LEA_CTR_MCT_FaxFile(txtFileName_mct, faxFileName_mct);

    create_LEA_CTR_KAT_RspFile(reqFileName_kat, rspFileName_kat);
    create_LEA_CTR_MMT_RspFile(reqFileName_mmt, rspFileName_mmt);
    create_LEA_CTR_MCT_RspFile(reqFileName_mct, rspFileName_mct);

    /********** Known Answer Test **********/
    printf("\nLEA128-CTR-KAT-TEST:\n");

    FILE* file1_kat = fopen(faxFileName_kat, "r");
    FILE* file2_kat = fopen(rspFileName_kat, "r");

    if (!file1_kat || !file2_kat) {
        perror("Error opening files");
        return;
    }

    CryptoData* pData1_kat = (CryptoData*)malloc(sizeof(CryptoData));
    CryptoData* pData2_kat = (CryptoData*)malloc(sizeof(CryptoData));
    if (pData1_kat == NULL || pData2_kat == NULL) {
        perror("Unable to allocate memory");
        return;
    }
    int result = 1; // Default to pass
    int idx = 1;
    int totalTests = 275; // Assuming a total of 10 tests
    int passedTests = 0;
    while (idx <= totalTests) {
        // Reset the structures for the next iteration
        memset(pData1_kat, 0, sizeof(CryptoData));
        memset(pData2_kat, 0, sizeof(CryptoData));
        if (!readCryptoData(file1_kat, pData1_kat) || !readCryptoData(file2_kat, pData2_kat)) {
            result = 0; // Indicate failure if read fails
            break;
        }

        if (!compareCryptoData(pData1_kat, pData2_kat)) {
            result = 0; // Fail
            printf("\nFAIL\n");
            break;
        }

        // Free the dynamically allocated memory
        freeCryptoData(pData1_kat);
        freeCryptoData(pData2_kat);

        passedTests++;
        printProgressBar(idx++, totalTests);
    }

    // printf("\n\nKAT Summary:\n");
    // printf("Passed: %d/%d\n", passedTests, totalTests);
    if (result) printf("\nPASS\n");
    // else printf("Some tests FAILED.\n\n");

    fclose(file1_kat);
    fclose(file2_kat);

    /********** Multi-block Message Test **********/
    printf("\nLEA128-CTR-MMT-TEST:\n");

    FILE* file1_mmt = fopen(faxFileName_mmt, "r");
    FILE* file2_mmt = fopen(rspFileName_mmt, "r");

    if (!file1_mmt || !file2_mmt) {
        perror("Error opening files");
        return;
    }

    CryptoData* pData1_mmt = (CryptoData*)malloc(sizeof(CryptoData));
    CryptoData* pData2_mmt = (CryptoData*)malloc(sizeof(CryptoData));
    if (pData1_mmt == NULL || pData2_mmt == NULL) {
        perror("Unable to allocate memory");
        return;
    }
    result = 1; // Default to pass
    idx = 1;
    totalTests = 10; // Assuming a total of 10 tests
    passedTests = 0;
    while (idx <= totalTests) {
        // Reset the structures for the next iteration
        memset(pData1_mmt, 0, sizeof(CryptoData));
        memset(pData2_mmt, 0, sizeof(CryptoData));
        if (!readCryptoData(file1_mmt, pData1_mmt) || !readCryptoData(file2_mmt, pData2_mmt)) {
            result = 0; // Indicate failure if read fails
            break;
        }

        if (!compareCryptoData(pData1_mmt, pData2_mmt)) {
            result = 0; // Fail
            printf("\nFAIL\n");
            break;
        }

        // Free the dynamically allocated memory
        freeCryptoData(pData1_mmt);
        freeCryptoData(pData2_mmt);

        passedTests++;
        printProgressBar(idx++, totalTests);
    }

    // printf("\n\nMMT Summary:\n");
    // printf("Passed: %d/%d\n", passedTests, totalTests);
    if (result) printf("\nPASS\n");
    // else printf("Some tests FAILED.\n\n");

    fclose(file1_mmt);
    fclose(file2_mmt);

    /********** Monte Carlo Test **********/
    printf("\nLEA128-CTR-MCT-TEST:\n");

    FILE* file1_mct = fopen(faxFileName_mct, "r");
    FILE* file2_mct = fopen(rspFileName_mct, "r");

    if (!file1_mct || !file2_mct) {
        perror("Error opening files");
        return;
    }

    CryptoData* pData1_mct = (CryptoData*)malloc(sizeof(CryptoData));
    CryptoData* pData2_mct = (CryptoData*)malloc(sizeof(CryptoData));
    if (pData1_mct == NULL || pData2_mct == NULL) {
        perror("Unable to allocate memory");
        return;
    }
    result = 1; // Default to pass
    idx = 1;
    totalTests = 100; // Assuming a total of 10 tests
    passedTests = 0;
    while (idx <= totalTests) {
        // sReset the structures for the next iteration
        memset(pData1_mct, 0, sizeof(CryptoData));
        memset(pData2_mct, 0, sizeof(CryptoData));
        if (!readCryptoData(file1_mct, pData1_mct) || !readCryptoData(file2_mct, pData2_mct)) {
            result = 0; // Indicate failure if read fails
            break;
        }

        if (!compareCryptoData(pData1_mct, pData2_mct)) {
            result = 0; // Fail
            printf("\nFAIL\n");
            break;
        }

        // Free the dynamically allocated memory
        freeCryptoData(pData1_mct);
        freeCryptoData(pData2_mct);

        passedTests++;
        printProgressBar(idx++, totalTests);
    }

    // printf("\n\nMCT Summary:\n");
    // printf("Passed: %d/%d\n", passedTests, totalTests);
    if (result) printf("\nPASS\n");
    // else printf("Some tests FAILED.\n\n");

    fclose(file1_mct);
    fclose(file2_mct);
}