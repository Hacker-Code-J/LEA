#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lea_cbc_movs.h"

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

#if 1
void cbcEncrypt(CryptoData* data) {
    // Assuming block size is the size of u32 * 4 (128 bits)
    size_t blockSize = 4; 
    size_t numBlocks = data->ptLength / blockSize;
    // size_t numBlocks = data->dataLength / blockSize;

    // Allocate memory for ciphertext
    data->ct = (u32*)malloc(numBlocks * blockSize * sizeof(u32));
    data->ctLength = numBlocks * blockSize;
    // data->dataLength = numBlocks * blockSize;

    // Generate round keys
    u32 roundKeys[TOTAL_RK];
    leaEncKeySchedule(roundKeys, data->key);

    u32 temp[blockSize];
    u32* iv = data->iv;

    // CBC encryption
    for (size_t i = 0; i < numBlocks; ++i) {
        // XOR with IV or previous ciphertext block
        for (size_t j = 0; j < blockSize; ++j) {
            temp[j] = data->pt[i * blockSize + j] ^ iv[j];
        }

        // Encrypt the block
        leaEncrypt(&data->ct[i * blockSize], temp, roundKeys);

        // Update IV (current ciphertext block becomes next IV)
        iv = &data->ct[i * blockSize];
    }
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

    printf("LEA128(CBC)MMT.rsp file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}
#endif
void MOVS_LEA128CBC_MMT_TEST() {
    const char* folderPath = "../LEA128(CBC)MOVS/";
    char txtFileName[50];
    char reqFileName[50];
    char faxFileName[50];
    char rspFileName[50];
    
    // Construct full paths for input and output files
    snprintf(txtFileName, sizeof(txtFileName), "%s%s", folderPath, "LEA128(CBC)MMT.txt");
    snprintf(reqFileName, sizeof(reqFileName), "%s%s", folderPath, "LEA128(CBC)MMT.req");
    snprintf(faxFileName, sizeof(faxFileName), "%s%s", folderPath, "LEA128(CBC)MMT.fax");
    snprintf(rspFileName, sizeof(rspFileName), "%s%s", folderPath, "LEA128(CBC)MMT.rsp");
    
    create_LEA128CBC_MMT_ReqFile(txtFileName, reqFileName);
    create_LEA128CBC_MMT_FaxFile(txtFileName, faxFileName);
    create_LEA128CBC_MMT_RspFile(reqFileName, rspFileName);

    printf("\nLEA128-CBC-MMT-TEST:\n");

    FILE* file1 = fopen(faxFileName, "r");
    FILE* file2 = fopen(rspFileName, "r");

    if (!file1 || !file2) {
        perror("Error opening files");
        return;
    }

    CryptoData data1, data2;
    memset(&data1, 0, sizeof(CryptoData));
    memset(&data2, 0, sizeof(CryptoData));
    int result = 1; // Default to pass
    int idx = 1;
    int totalTests = 10; // Assuming a total of 275 tests
    int passedTests = 0;
    while (idx <= totalTests) {
        if (readCryptoData(file1, &data1) == -1 || readCryptoData(file2, &data2) == -1) {
            result = 0; // Indicate failure if read fails
            break;
        }

        if (!compareCryptoData(&data1, &data2)) {
            result = 0; // Fail
            printf("\nFAIL\n");
            break;
        }

        // Free the dynamically allocated memory
        freeCryptoData(&data1);
        freeCryptoData(&data2);

        // Reset the structures for the next iteration
        memset(&data1, 0, sizeof(CryptoData));
        memset(&data2, 0, sizeof(CryptoData));

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

#if 0
void create_LEA128CBC_MMT_RspFile(const char* inputFileName, const char* outputFileName) {
    FILE *reqFile, *rspFile;
    char line[INITIAL_BUF_SIZE];
    int isFirstKey = 1;
    CryptoData data;
    memset(&data, 0, sizeof(CryptoData));

    reqFile = fopen(inputFileName, "r");
    if (reqFile == NULL) {
        perror("Error opening input file");
        return;
    }

    rspFile = fopen(outputFileName, "w");
    if (rspFile == NULL) {
        perror("Error opening output file");
        fclose(reqFile);
        return;
    }

    while (fgets(line, sizeof(line), reqFile) != NULL) {
        if (strncmp(line, "KEY =", 5) == 0) {
            if (!isFirstKey) {
                fputc('\n', rspFile);
            }
            isFirstKey = 0;
            parseHexLine(data.key, line + 6);
            fputs(line, rspFile);
        } else if (strncmp(line, "IV =", 4) == 0) {
            parseHexLine(data.iv, line + 5);
            fputs(line, rspFile);
        } else if (strncmp(line, "PT =", 4) == 0) {
            data.ptLength = determineLength(line + 5);
            data.pt = (u32*)malloc(data.ptLength * sizeof(u32));
            if (data.pt == NULL) {
                perror("Unable to allocate memory for PT");
                break;
            }
            parseHexLineVariable(data.pt, line + 5, data.ptLength);
            fputs(line, rspFile);

            data.ct = (u32*)malloc(data.ptLength * sizeof(u32));
            if (data.ct == NULL) {
                perror("Unable to allocate memory for CT");
                free(data.pt);
                break;
            }
            CBC_Encrypt_LEA(data.ct, data.pt, data.ptLength, data.key, data.iv);

            fprintf(rspFile, "CT = ");
            for (size_t i = 0; i < data.ptLength - 1; i++) {
                fprintf(rspFile, "%08X", data.ct[i]);
            }
            fprintf(rspFile, "\n");

            freeCryptoData(&data);
        }
    }

    fclose(reqFile);
    fclose(rspFile);

    printf("LEA128(CBC)MMT.rsp file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}

void MOVS_LEA128CBC_MMT_TEST() {
    const char* folderPath = "../LEA128(CBC)MOVS/";
    char txtFileName[50];
    char reqFileName[50];
    char faxFileName[50];
    char rspFileName[50];
    
    // Construct full paths for input and output files
    snprintf(txtFileName, sizeof(txtFileName), "%s%s", folderPath, "LEA128(CBC)MMT.txt");
    snprintf(reqFileName, sizeof(reqFileName), "%s%s", folderPath, "LEA128(CBC)MMT.req");
    snprintf(faxFileName, sizeof(faxFileName), "%s%s", folderPath, "LEA128(CBC)MMT.fax");
    snprintf(rspFileName, sizeof(rspFileName), "%s%s", folderPath, "LEA128(CBC)MMT.rsp");
    
    create_LEA128CBC_MMT_ReqFile(txtFileName, reqFileName);
    create_LEA128CBC_MMT_FaxFile(txtFileName, faxFileName);
    create_LEA128CBC_MMT_RspFile(reqFileName, rspFileName);

    printf("\nLEA128-CBC-MMT-TEST:\n");

    FILE* file1 = fopen(faxFileName, "r");
    FILE* file2 = fopen(rspFileName, "r");

    if (!file1 || !file2) {
        perror("Error opening files");
        return;
    }

    DataSet data1, data2;
    memset(&data1, 0, sizeof(DataSet));
    memset(&data2, 0, sizeof(DataSet));
    int result = 1; // Default to pass
    int idx = 1;
    int totalTests = 275; // Assuming a total of 275 tests
    int passedTests = 0;
    while (1) {
        if (readDataSet(file1, &data1) == -1 || readDataSet(file2, &data2) == -1) break;

        if (!compareDataSets(&data1, &data2)) {
            result = 0; // Fail
            printf("\nFAIL\n");
            break;
        }

        memset(&data1, 0, sizeof(DataSet));
        memset(&data2, 0, sizeof(DataSet));

        // printf("PASS(%d/275)\n", idx++);
        passedTests++;
        printProgressBar(idx++, totalTests);
    }
    
    printf("\n\nTesting Summary:\n");
    printf("Passed: %d/%d\n", passedTests, totalTests);
    if (result) {
        printf("Perfect PASS !!!\n");
    } else {
        printf("Some tests FAILED.\n");
    }

    fclose(file1);
    fclose(file2);
}
#endif
// =====================================================
// If the "CT" (Ciphertext) value is longer and doesn't fit into the existing structure of DataSet, you will need to adjust your data structure and parsing logic to accommodate the longer CT values. Here's how you can modify the program:

// Extend the DataSet Structure: Modify the DataSet structure to include a larger array for the CT value.

// Adjust the Parsing Logic: Update the readDataSet function to read and store the longer CT value.

// Modify Comparison Logic: Since you mentioned that CT values can be different, the comparison logic might remain the same. However, if you need to compare CT as well, adjust the compareDataSets function accordingly.

// Here's an example of how you might modify the code:
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// typedef unsigned int u32;

// typedef struct {
//     u32 key[4];
//     u32 iv[4];
//     u32 pt[4];
//     u32 ct[8]; // Assuming CT is twice as long
// } DataSet;

// // ... [Other functions like parseHexLine, skipPrefix, etc.]

// void parseLongHexLine(u32 arr[8], const char* line) {
//     for (int i = 0; i < 8; i++) { // Now reads 8 segments
//         sscanf(line + i * 8, "%8x", &arr[i]);
//     }
// }

// int readDataSet(FILE *fp, DataSet *dataSet) {
//     char line[80]; // Larger buffer for longer CT line

//     // ... [Read KEY, IV, PT as before]

//     // Read CT
//     if (fgets(line, sizeof(line), fp) == NULL) return -1;
//     parseLongHexLine(dataSet->ct, skipPrefix(line));
    
//     return 0;
// }

// // ... [No change in compareDataSets if CT comparison is not required]

// // ... [Rest of the main program, testFiles, etc.]