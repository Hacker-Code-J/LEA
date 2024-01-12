#include <stdbool.h>

#include "lea.h"
#include "lea_modes.h"

#ifndef _LEA_CBC_MOVS_H
#define _LEA_CBC_MOVS_H

#define INITIAL_BUF_SIZE 1500
#define MAX_LINE_LENGTH 150

typedef struct {
    u32 key[4];
    u32 iv[4];
    u32 pt[4];
    u32 ct[4];
} DATA;

// void parseHexToU32(const char *hex, u32* output);
// void parseLine(char* line, u32* output);
void parseHexLine(u32 arr[4], const char* line);

void create_LEA128CBC_KAT_ReqFile(const char* inputFileName, const char* outputFileName);
void create_LEA128CBC_KAT_FaxFile(const char* inputFileName, const char* outputFileName);
void create_LEA128CBC_KAT_RspFile(const char* inputFileName, const char* outputFileName);

bool compare_sets(FILE* file1, FILE* file2);

void MOVS_LEA128CBC_KAT_TEST();

#endif // _LEA_CBC_MOVS_H