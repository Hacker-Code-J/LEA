#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lea_cbc_movs.h"

#if 0
void create_LEA128CBC_MMT_ReqFile(const char* inputFileName, const char* outputFileName) {
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

    printf("LEA128(CBC)MMT.req file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}

void create_LEA128CBC_MMT_FaxFile(const char* inputFileName, const char* outputFileName) {
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

    printf("LEA128(CBC)MMT.fax file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}

void create_LEA128CBC_MMT_RspFile(const char* inputFileName, const char* outputFileName) {
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
        
            memset(&data, 0, sizeof(DataSet));
        }
    }

    free(line);
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