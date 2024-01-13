#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lea_cbc_movs.h"

void printProgressBar(int current, int total) {
    int width = 50; // Width of the progress bar
    float progress = (float)current / total;
    int pos = width * progress;

    // ANSI Escape Codes for colors
    const char* GREEN = "\x1b[32m";
    const char* YELLOW = "\x1b[33m";
    const char* RED = "\x1b[31m";
    const char* CYAN = "\x1b[36m";
    // const char* RESET = "\x1b[0m";

    printf("\r[");
    for (int i = 0; i < width; ++i) {
        if (i < pos) printf("%s=", GREEN); // White for completed part
        else if (i == pos) printf("%s>", YELLOW); // Yellow for current position
        else printf("%s ", RED); // Red for remaining part
    }
    printf("%s] %d%% (%d/%d)", CYAN, (int)(progress * 100.0), current, total);
    fflush(stdout); // Flush the output buffer

    // int width = 50; // Width of the progress bar
    // float progress = (float)current / total;
    // int pos = width * progress;

    // // ANSI Escape Codes for colors
    // const char* RED = "\x1b[31m";
    // const char* GREEN = "\x1b[32m";
    // const char* YELLOW = "\x1b[33m";
    // const char* RESET = "\x1b[0m";

    // printf("\r[");
    // for (int i = 0; i < width; ++i) {
    //     if (i < pos) printf("%s=", GREEN); // Green for completed part
    //     else if (i == pos) printf("%s>", YELLOW); // Yellow for current position
    //     else printf("%s ", RED); // Red for remaining part
    // }
    // printf("%s] %d%% (%d/%d", RESET, (int)(progress * 100.0), current, total);
    // fflush(stdout); // Flush the output buffer
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

void parseHexLine(u32 arr[4], const char* line) {
    // Assuming line is in the format "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
    // for (int i = 0; i < 4; i++) {
    //     char buffer[9]; // 8 characters for 32 bits + null terminator
    //     strncpy(buffer, line + i * 8, 8);
    //     buffer[8] = '\0';
    //     arr[i] = strtoul(buffer, NULL, 16);
    // }

    // for (int i = 0; i < 4; i++) {
    //     sscanf(line + i * 8, "%8x", &arr[i]);
    // }

    for (int i = 0; i < 4; i++) {
        u32 value;
        sscanf(line + i * 8, "%8x", &value);
        arr[i] = value;
    }
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

// void parseHexToU32(const char *hex, u32* output) {
//     // Temporary buffer to hold two hexadecimal digits
//     char hexPair[3] = { 0x00, };

//     // Loop over the hex string two characters at a time
//     for (int i = 0; i < 32; i += 2) {
//         // Copy two hex digits into the temporary buffer
//         hexPair[0] = hex[i];
//         hexPair[1] = hex[i + 1];

//         // Convert the hex pair to a single byte and store it in the output array
//         u32 byte;
//         sscanf(hexPair, "%02x", &byte);
//         output[i / 8] |= byte << (24 - 8 * (i / 8));
//     }
// }

// void parseLine(char* line, u32* output) {
//     // Assuming line format is like "KEY = 0123456789ABCDEF..."
//     // Skip the "KEY = " part to get to the hex string
//     const char* hexString = line;

//     // Reset the output array
//     for (size_t i = 0; i < sizeof(u32); i++)
//         output[i] = 0x00;

//     // Parse the hex string into the output array
//     for (int i = 0; i < BLOCK_SIZE; i++)
//         parseHexToU32(hexString, output + i);
// }

// Usage:
// char testLine[] = "KEY = 0123456789ABCDEFFEDCBA9876543210";
// u32 output[4] = {0};

// parseLine(testLine, output);

// // Print the output for verification
// for (int i = 0; i < 4; i++) {
//     printf("output[%d] = %08x\n", i, output[i]);
// }

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
        // else if (strncmp(line, "CT =", 4) == 0) {
        //     parseHexLine(data.ct, line + 5);
        //     // Write to file and reset struct
        //     fwrite(&data, sizeof(DATA), 1, rspFile);
        //     memset(&data, 0, sizeof(DATA));
        // }
    }

    free(line);
    fclose(reqFile);
    fclose(rspFile);
    
    printf("LEA128(CBC)KAT.rsp file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}

// bool compare_sets(FILE* file1, FILE* file2) {
//     char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];

//     for (int i = 0; i < 4; i++) {
//         if (!fgets(line1, MAX_LINE_LENGTH, file1) || !fgets(line2, MAX_LINE_LENGTH, file2)) {
//             return false; // Error or end of file
//         }

//         if (i < 3 && strcmp(line1, line2) != 0) {
//             return false; // Mismatch in KEY, IV, or PT
//         }
//     }

//     return true;
// }

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

    // if (file1 == NULL || file2 == NULL) {
    //     perror("Error opening files");
    //     return;
    // }

    // char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
    // int allMatch = 1;

    // while (fgets(line1, MAX_LINE_LENGTH, file1) != NULL &&
    //        fgets(line2, MAX_LINE_LENGTH, file2) != NULL) {

    //     if (strcmp(line1, line2) != 0) {
    //         // Allow difference in CT lines
    //         if (strncmp(line1, "CT", 2) == 0 && strncmp(line2, "CT", 2) == 0) 
    //             continue;

    //         allMatch = 0;
    //         break;
    //     }
    // }

    // // Check for file ending consistency
    // if (!feof(file1) || !feof(file2)) {
    //     allMatch = 0;
    // }

    // fclose(file1);
    // fclose(file2);

    // if (allMatch) {
    //     printf("PASS\n");
    // } else {
    //     printf("FAIL\n");
    // }
}