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
} DataSet;

void printProgressBar(int current, int total);

int readDataSet(FILE* fp, DataSet* dataSet);
bool compareDataSets(const DataSet* data1, const DataSet* data2);

// void parseHexToU32(const char *hex, u32* output);
// void parseLine(char* line, u32* output);
void parseHexLine(u32 arr[4], const char* line);
const char* skipPrefix(const char* line);

void create_LEA128CBC_KAT_ReqFile(const char* inputFileName, const char* outputFileName);
void create_LEA128CBC_KAT_FaxFile(const char* inputFileName, const char* outputFileName);

void create_LEA128CBC_KAT_RspFile(const char* inputFileName, const char* outputFileName);
void MOVS_LEA128CBC_KAT_TEST();

void create_LEA128CBC_MMT_ReqFile(const char* inputFileName, const char* outputFileName);
void create_LEA128CBC_MMT_FaxFile(const char* inputFileName, const char* outputFileName);

void create_LEA128CBC_MMT_RspFile(const char* inputFileName, const char* outputFileName);
void MOVS_LEA128CBC_MMT_TEST();

#endif // _LEA_CBC_MOVS_H