#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lea_cbc_movs.h"

void freeCryptoData(CryptoData* cryptoData) {
    if (cryptoData != NULL) {
        free(cryptoData->pt);
        free(cryptoData->ct);
        cryptoData->pt = NULL;
        cryptoData->ct = NULL;
    }
}

void parseHexLineVariable(u32* arr, const char* line, size_t length) {
    for (size_t i = 0; i < length; i++) {
        u32 value;
        // Ensure not to read beyond the line's end
        if (sscanf(line + i * 8, "%8x", &value) != 1) {
            // Handle parsing error, such as setting a default value or logging an error
            arr[i] = 0; // Example: set to zero if parsing fails
        } else {
            arr[i] = value;
        }
    }
}

size_t determineLength(const char* hexString) {
    // Calculate the length of the hexadecimal string
    size_t hexLength = strlen(hexString);

    // Convert hex length to the length of the u32 array
    size_t u32Length = hexLength / 8;

    // If the hex string length is not a multiple of 8, add an extra element
    if (hexLength % 8 != 0) {
        u32Length++;
    }

    return u32Length;
}

int readCryptoData(FILE* fp, CryptoData* cryptoData) {
    char line[MAX_LINE_LENGTH]; // Assuming each line will not exceed this length

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "KEY =", 5) == 0) {
            parseHexLine(cryptoData->key, line + 6); // Skip "KEY = "
        } else if (strncmp(line, "IV =", 4) == 0) {
            parseHexLine(cryptoData->iv, line + 5); // Skip "IV = "
        } else if (strncmp(line, "PT =", 4) == 0) {
            cryptoData->ptLength = determineLength(line + 5); // Calculate length
            cryptoData->pt = (u32*)malloc(cryptoData->ptLength * sizeof(u32));
            if (cryptoData->pt == NULL) return -1;
            parseHexLineVariable(cryptoData->pt, line + 5, cryptoData->ptLength);
        } else if (strncmp(line, "CT =", 4) == 0) {
            cryptoData->ctLength = determineLength(line + 5); // Calculate length
            cryptoData->ct = (u32*)malloc(cryptoData->ctLength * sizeof(u32));
            if (cryptoData->ct == NULL) {
                free(cryptoData->pt); // Free pt if ct allocation fails
                return -1;
            }
            parseHexLineVariable(cryptoData->ct, line + 5, cryptoData->ctLength);
        }
        // Add more conditions here if there are more data types
    }

    return 0; // Return 0 on successful read
}

bool compareCryptoData(const CryptoData* data1, const CryptoData* data2) {
    // Compare fixed-size arrays: key and iv
    for (int i = 0; i < 4; i++) {
        if (data1->key[i] != data2->key[i] || data1->iv[i] != data2->iv[i]) {
            return 0; // Not equal
        }
    }

    // Compare lengths and contents of pt
    if (data1->ptLength != data2->ptLength) {
        return 0; // Not equal
    }
    for (size_t i = 0; i < data1->ptLength; i++) {
        if (data1->pt[i] != data2->pt[i]) {
            return 0; // Not equal
        }
    }

    // Compare lengths and contents of ct
    if (data1->ctLength != data2->ctLength) {
        return 0; // Not equal
    }
    for (size_t i = 0; i < data1->ctLength; i++) {
        if (data1->ct[i] != data2->ct[i]) {
            return 0; // Not equal
        }
    }

    return 1; // All comparisons passed, data structures are equal
}

#if 1
void printProgressBar(int current, int total) {
    int width = 50; // Width of the progress bar
    float progress = (float)current / total;
    int pos = width * progress;

    // ANSI Escape Codes for colors
    const char* GREEN = "\x1b[32m";
    const char* YELLOW = "\x1b[33m";
    const char* RED = "\x1b[31m";
    // const char* CYAN = "\x1b[36m";
    const char* RESET = "\x1b[0m";

    printf("\r[");
    for (int i = 0; i < width; ++i) {
        if (i < pos) printf("%s=", GREEN); // White for completed part
        else if (i == pos) printf("%s>", YELLOW); // Yellow for current position
        else printf("%s ", RED); // Red for remaining part
    }
    printf("%s] %d%% (%d/%d)", RESET, (int)(progress * 100.0), current, total);
    fflush(stdout); // Flush the output buffer

    // ==============================================
    // int width = 50; // Width of the progress bar
    // float progress = (float)current / total;
    // int pos = width * progress;

    // printf("\r[");
    // for (int i = 0; i < width; ++i) {
    //     if (i < pos) printf("=");
    //     else if (i == pos) printf(">");
    //     else printf(" ");
    // }
    // printf("] %d%% (%d/%d)", (int)(progress * 100.0), current, total);
    // fflush(stdout); // Flush the output buffer
}

int readDataSet(FILE* fp, DataSet* dataSet) {
    char line[50];
    if (fgets(line, sizeof(line), fp) == NULL) return -1; // Read KEY
    parseHexLine(dataSet->key, skipPrefix(line));
    
    if (fgets(line, sizeof(line), fp) == NULL) return -1; // Read IV
    parseHexLine(dataSet->iv, skipPrefix(line));
    
    if (fgets(line, sizeof(line), fp) == NULL) return -1; // Read PT
    parseHexLine(dataSet->pt, skipPrefix(line));
    
    if (fgets(line, sizeof(line), fp) == NULL) return -1; // Read CT
    parseHexLine(dataSet->ct, skipPrefix(line));

    if (fgets(line, sizeof(line), fp) == NULL) return -1; // Skip BLANK

    return 0;
}

bool compareDataSets(const DataSet* data1, const DataSet* data2) {
    for (int i = 0; i < 4; i++) {
        if (data1->key[i] != data2->key[i] ||
            data1->iv[i] != data2->iv[i] ||
            data1->pt[i] != data2->pt[i] ||
            data1->ct[i] != data2->ct[i]) {
            return 0;
        }
    }
    return 1;
}

void parseHexLine(u32* pArr, const char* pLine) {
    // Assuming line is in the format "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
    int i = 0;
    while (pLine[i * 8] != '\0') {
        u32 value;
        if (sscanf(pLine + i * 8, "%8x", &value) != 1) {
            // Handle error or invalid input
            break;
        }
        pArr[i] = value;
        i++;
    }
    // for (int i = 0; i < 4; i++) {
    //     u32 value;
    //     sscanf(pLine + i * 8, "%8x", &value);
    //     *(pArr + i) = value;
    // }
}

// Function to skip the prefix and return the pointer to the hexadecimal part
const char* skipPrefix(const char* line) {
    // while (*line != '\0' && *line != '=') {
    //     line++;
    // }
    // return line + 1; // Skip '=' and space

    while (*line != '\0' && *line != '=') {
        line++;
    }
    while (*line == '=' || *line == ' ') {
        line++; // Skip '=' and any spaces after it
    }
    return line;
}

void create_LEA128CBC_KAT_ReqFile(const char* inputFileName, const char* outputFileName) {
    FILE *infile, *reqFile;
    char* line;
    size_t bufsize = INITIAL_BUF_SIZE;
    int isFirstKey = 1; // Flag to check if it's the first KEY line

    // Open the source text file for reading
    infile = fopen(inputFileName, "r");
    if (infile == NULL) {
        perror("Error opening input file");
        return;
    }

    // Open the .req file for writing
    reqFile = fopen(outputFileName, "w");
    if (reqFile == NULL) {
        perror("Error opening .req file");
        fclose(infile);
        return;
    }

    // Allocate initial buffer
    line = (char*)malloc(bufsize * sizeof(char));
    if (line == NULL) {
        perror("Unable to allocate memory");
        fclose(infile);
        fclose(reqFile);
        return;
    }

    // Read the source file line by line
    while (fgets(line, bufsize, infile) != NULL) {
        if (strncmp(line, "KEY", 3) == 0) {
            if (!isFirstKey) {
                // If not the first KEY, add a newline before writing the line
                fputc('\n', reqFile);
            }
            isFirstKey = 0;
            fputs(line, reqFile);
        } else if (strncmp(line, "IV", 2) == 0 || strncmp(line, "PT", 2) == 0) {
            fputs(line, reqFile);
        }
    }

    // Free the allocated line buffer and close files
    free(line);
    fclose(infile);
    fclose(reqFile);

    printf("LEA128(CBC)KAT.req file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}

// Usage:
// int main() {
//     const char *inputFileName = "input.txt";
//     const char *outputFileName = "output.req";

//     createReqFile(inputFileName, outputFileName);

//     printf(".req file has been successfully created.\n");
//     return 0;
// }
// int main() {
//     const char *folderPath = "LEA128 (CBC) MOVS/";
//     char inputFilePath[300];
//     char outputFilePath[300];

//     // Construct full paths for input and output files
//     snprintf(inputFilePath, sizeof(inputFilePath), "%s%s", folderPath, "input.txt");
//     snprintf(outputFilePath, sizeof(outputFilePath), "%s%s", folderPath, "output.req");

//     createReqFile(inputFilePath, outputFilePath);

//     printf(".req file has been successfully created in 'LEA128 (CBC) MOVS' folder.\n");
//     return 0;
// }


void create_LEA128CBC_KAT_FaxFile(const char* inputFileName, const char* outputFileName) {
    FILE *infile, *faxFile;
    char* line;
    size_t bufsize = INITIAL_BUF_SIZE;
    int isFirstKey = 1; // Flag to check if it's the first KEY line

    // Open the source text file for reading
    infile = fopen(inputFileName, "r");
    if (infile == NULL) {
        perror("Error opening input file");
        return;
    }

    // Open the .fax file for writing
    faxFile = fopen(outputFileName, "w");
    if (faxFile == NULL) {
        perror("Error opening .fax file");
        fclose(infile);
        return;
    }

    // Allocate initial buffer
    line = (char*)malloc(bufsize * sizeof(char));
    if (line == NULL) {
        perror("Unable to allocate memory");
        fclose(infile);
        fclose(faxFile);
        return;
    }

    // Read the source file line by line
    while (fgets(line, bufsize, infile) != NULL) {
        if (strncmp(line, "KEY", 3) == 0) {
            if (!isFirstKey) {
                // If not the first KEY, add a newline before writing the line
                fputc('\n', faxFile);
            }
            isFirstKey = 0;
            fputs(line, faxFile);
        } else if (strncmp(line, "IV", 2) == 0 || strncmp(line, "PT", 2) == 0 || strncmp(line, "CT", 2) == 0) {
            fputs(line, faxFile);
        }
    }

    // Free the allocated line buffer and close files
    free(line);
    fclose(infile);
    fclose(faxFile);

    printf("LEA128(CBC)KAT.fax file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}

void create_LEA128CBC_KAT_RspFile(const char* inputFileName, const char* outputFileName) {
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

    printf("LEA128(CBC)KAT.rsp file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
#if 0
    FILE *reqFile, *rspFile;
    char* line;
    size_t bufsize = INITIAL_BUF_SIZE;
    int isFirstKey = 1; // Flag to check if it's the first KEY line
    
    DataSet data;
    memset(&data, 0, sizeof(DataSet));

    // Open the source text file for reading
    reqFile = fopen(inputFileName, "r");
    if (reqFile == NULL) {
        perror("Error opening input file");
        return;
    }

    // Open the .rsp file for writing
    rspFile = fopen(outputFileName, "w");
    if (rspFile == NULL) {
        perror("Error opening .rsp file");
        fclose(reqFile);
        return;
    }

    // Allocate initial buffer
    line = (char*)malloc(bufsize * sizeof(char));
    if (line == NULL) {
        perror("Unable to allocate memory");
        fclose(reqFile);
        fclose(rspFile);
        return;
    }

    while (fgets(line, bufsize, reqFile) != NULL) {
        if (strncmp(line, "KEY =", 5) == 0) {
            if (!isFirstKey) {
                // If not the first KEY, add a newline before writing the line
                fputc('\n', rspFile);
            }
            isFirstKey = 0;
            parseHexLine(data.key, line + 6);
            fputs(line, rspFile);
        } else if (strncmp(line, "IV =", 4) == 0) {
            parseHexLine(data.iv, line + 5);
            fputs(line, rspFile);
        } else if (strncmp(line, "PT =", 4) == 0) {
            parseHexLine(data.pt, line + 5);
            fputs(line, rspFile);

            CBC_Encrypt_LEA(data.ct, data.pt, BLOCK_SIZE, data.key, data.iv);

            // Write CT to the response file
            fprintf(rspFile, "CT = ");
            for (int i = 0; i < 4; i++) {
                fprintf(rspFile, "%08X", data.ct[i]);
            }
            fprintf(rspFile, "\n");
            
            // fwrite(&data, sizeof(DATA), 1, rspFile);
            memset(&data, 0, sizeof(DataSet));
        }
    }

    free(line);
    fclose(reqFile);
    fclose(rspFile);
    
    printf("LEA128(CBC)KAT.rsp file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
#endif
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
    
    create_LEA128CBC_KAT_ReqFile(txtFileName, reqFileName);
    create_LEA128CBC_KAT_FaxFile(txtFileName, faxFileName);
    create_LEA128CBC_KAT_RspFile(reqFileName, rspFileName);

    printf("\nLEA128-CBC-KAT-TEST:\n");

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
    int totalTests = 275; // Assuming a total of 275 tests
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

#if 0
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

#endif   

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
#endif 