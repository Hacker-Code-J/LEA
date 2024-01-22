#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lea_cbc_movs.h"

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
        } else if (strncmp(line, "IV =", 4) == 0 && !ivFound) {
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

bool compareCryptoData(const CryptoData* pData1, const CryptoData* pData2) {
    if (pData1->keyLength != pData2->keyLength ||
        pData1->dataLength != pData2->dataLength) {
        return false;
    }

    size_t keyLen = pData1->keyLength;
    size_t dataLen = pData1->dataLength;
    
    // Compare contents key and iv
    for (size_t i = 0; i < keyLen; i++) {
        if (pData1->key[i] != pData2->key[i] ||
            pData1->iv[i] != pData2->iv[i]) {
            return false;
        }
    }

    // Compare contents pt and ct
    for (size_t i = 0; i < dataLen; i++) {
        if (pData1->pt[i] != pData2->pt[i] ||
            pData1->ct[i] != pData2->ct[i]) {
            return false;
        }
    }

    return true; // All comparisons passed, data structures are equal
}

void printHexToFile(FILE* pFile, const u32* data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        fprintf(pFile, "%08X", data[i]);
    }
    fprintf(pFile, "\n"); // Add a newline for formatting, if needed
}

void xorBuffers(u32* result, const u32* a, const u32* b, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        result[i] = a[i] ^ b[i];
    }
}