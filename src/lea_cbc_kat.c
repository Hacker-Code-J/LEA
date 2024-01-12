#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "lea_cbc_movs.h"

void parseHexLine(u32 arr[4], const char* line) {
    // Assuming line is in the format "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
    // for (int i = 0; i < 4; i++) {
    //     char buffer[9]; // 8 characters for 32 bits + null terminator
    //     strncpy(buffer, line + i * 8, 8);
    //     buffer[8] = '\0';
    //     arr[i] = strtoul(buffer, NULL, 16);
    // }
    for (int i = 0; i < 4; i++) {
        sscanf(line + i * 8, "%8x", &arr[i]);
    }
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

    printf(".req file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
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

    printf(".fax file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}

void create_LEA128CBC_KAT_RspFile(const char* inputFileName, const char* outputFileName) {
    FILE *reqFile, *rspFile;
    char* line;
    size_t bufsize = INITIAL_BUF_SIZE;
    int isFirstKey = 1; // Flag to check if it's the first KEY line
    
    DATA data;
    memset(&data, 0, sizeof(DATA));

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
            memset(&data, 0, sizeof(DATA));
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
    
    printf(".rsp file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
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

    const char* file1 = faxFileName;
    const char* file2 = rspFileName;

    FILE *fp1 = fopen(file1, "r");
    FILE *fp2 = fopen(file2, "r");

    char line1[INITIAL_BUF_SIZE], line2[INITIAL_BUF_SIZE];
    char last_ct1[INITIAL_BUF_SIZE], last_ct2[INITIAL_BUF_SIZE];

    if (!fp1 || !fp2) {
        perror("Error opening files");
        exit(1); // Terminate the program due to file open error
    }

    while (!feof(fp1) && !feof(fp2)) {
        int foundCT1 = 0, foundCT2 = 0;

        // Read paragraphs from both files
        while (fgets(line1, INITIAL_BUF_SIZE, fp1) != NULL) {
            if (strncmp(line1, "CT", 2) == 0) {
                strcpy(last_ct1, line1);
                foundCT1 = 1;
            }
        }
        while (fgets(line2, INITIAL_BUF_SIZE, fp2) != NULL) {
            if (strncmp(line2, "CT", 2) == 0) {
                strcpy(last_ct2, line2);
                foundCT2 = 1;
            }
        }

        // Ensure both paragraphs had a CT line
        if (!foundCT1 || !foundCT2) {
            printf("FAIL\n");
            fclose(fp1);
            fclose(fp2);
            exit(1); // Terminate the program due to missing CT line
        }

        // Compare last CT strings
        if (strcmp(last_ct1, last_ct2) != 0) {
            printf("FAIL\n");
            fclose(fp1);
            fclose(fp2);
            exit(1); // Terminate the program due to mismatch
        } else {
            printf("PASS\n"); // Print PASS for each matched pair
        }
    }

    fclose(fp1);
    fclose(fp2);
}