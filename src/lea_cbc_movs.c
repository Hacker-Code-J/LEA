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

#if 0
bool readCryptoData(FILE* fp, CryptoData* pCryptoData) {
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "KEY =", 5) == 0) {
            pCryptoData->keyLength = wordLength(line + 6);
            pCryptoData->key = (u32*)calloc(pCryptoData->keyLength, sizeof(u32));
            if(pCryptoData->key == NULL) return;
            parseHexLine(pCryptoData->key, line, pCryptoData->keyLength);
        } else if (strncmp(line, "IV =", 4) == 0) {
            if (!pCryptoData->keyLength) {
                perror("Key Length is invalid");
                free(pCryptoData->key);
                return;
            }
            pCryptoData->iv = (u32*)calloc(pCryptoData->keyLength, sizeof(u32));
            if(pCryptoData->iv == NULL) {
                free(pCryptoData->key);
                return;
            }
            parseHexLine(pCryptoData->iv, line, pCryptoData->keyLength);
        } else if (strncmp(line, "PT =", 4) == 0) {
            pCryptoData->dataLength = wordLength(line + 5);
            pCryptoData->pt = (u32*)calloc(pCryptoData->dataLength, sizeof(u32));
            if(pCryptoData->pt == NULL) {
                free(pCryptoData->key);
                free(pCryptoData->iv);
                return;
            }
            parseHexLine(pCryptoData->pt, line, pCryptoData->dataLength);
        } else if (strncmp(line, "CT =", 4) == 0) {
            if (!pCryptoData->keyLength) {
                perror("Data Length is invalid");
                free(pCryptoData->key);
                free(pCryptoData->iv);
                free(pCryptoData->pt);
                return;
            }
            pCryptoData->ct = (u32*)calloc(pCryptoData->dataLength, sizeof(u32));
            if(pCryptoData->iv == NULL) {
                free(pCryptoData->key);
                free(pCryptoData->iv);
                free(pCryptoData->pt);
                return;
            }
            parseHexLine(pCryptoData->ct, line, pCryptoData->dataLength);
        }
    }

    return 0; // Return 0 on successful read
}
#endif

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

#if 0
void create_LEA128CBC_MMT_ReqFile(const char* pTxtFileName, const char* pReqFileName) {
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

    printf("LEA128(CBC)MMT.req file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}

void create_LEA128CBC_MMT_FaxFile(const char* pTxtFileName, const char* pFaxFileName) {
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

    printf("LEA128(CBC)MMT.fax file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}

void create_LEA128CBC_MMT_RspFile(const char* pReqFileName, const char* pRspFileName) {
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
                            pData->iv);
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

    printf("LEA128(CBC)MMT.rsp file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}
#endif