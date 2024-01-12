#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUF_SIZE 1500

#include "lea_cbc_movs.h"

void parseHexToU32(const char *hex, u32* output) {
    // Temporary buffer to hold two hexadecimal digits
    char hexPair[3] = { 0x00, };

    // Loop over the hex string two characters at a time
    for (int i = 0; i < 4; i += 2) {
        // Copy two hex digits into the temporary buffer
        hexPair[0] = hex[i];
        hexPair[1] = hex[i + 1];

        // Convert the hex pair to a single byte and store it in the output array
        u32 byte;
        sscanf(hexPair, "%02x", &byte);
        output[i / 8] |= byte << (24 - 8 * (i / 8));
    }
}

void parseLine(char* line, u32* output) {
    // Assuming line format is like "KEY = 0123456789ABCDEF..."
    // Skip the "KEY = " part to get to the hex string
    const char* hexString = line + 6;

    // Reset the output array
    for (size_t i = 0; i < sizeof(u32); i++)
        output[i] = 0x00;

    // Parse the hex string into the output array
    for (int i = 0; i < BLOCK_SIZE; i++)
        parseHexToU32(hexString, output + i);
}

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

    while (fgets(line, sizeof(line), reqFile) != NULL) {
        DATA data;
        if (strncmp(line, "KEY =", 5) == 0) {
            if (!isFirstKey) {
                // If not the first KEY, add a newline before writing the line
                fputc('\n', rspFile);
            }
            isFirstKey = 0;
            fputs(line, rspFile);
            // parseLine(line + 6, data.key);
            // fprintf(rspFile, "%s", line); // Write KEY to the response file
        } else if (strncmp(line, "IV =", 4) == 0) {
            if (!isFirstKey) {
                // If not the first KEY, add a newline before writing the line
                fputc('\n', rspFile);
            }
            isFirstKey = 0;
            parseLine(line + 5, data.iv);
            fprintf(rspFile, "%s", line); // Write IV to the response file
        } else if (strncmp(line, "PT =", 4) == 0) {
            if (!isFirstKey) {
                // If not the first KEY, add a newline before writing the line
                fputc('\n', rspFile);
            }
            isFirstKey = 0;
            parseLine(line + 5, data.pt);
            fprintf(rspFile, "%s\n", line); // Write PT to the response file

            CBC_Encrypt_LEA(data.ct, data.pt, BLOCK_SIZE, data.key, data.iv);

            // Write CT to the response file
            fprintf(rspFile, "CT = ");
            for (int i = 0; i < 4; i++) {
                fprintf(rspFile, "%08X", data.ct[i]);
            }
            fprintf(rspFile, "\n");
        }
    }

    free(line);
    fclose(reqFile);
    fclose(rspFile);
    
    printf(".rsp file has been successfully created in 'LEA128(CBC)MOVS' folder.\n");
}