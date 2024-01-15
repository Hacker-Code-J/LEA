#include <stdbool.h>

#include "lea.h"
#include "lea_modes.h"

#ifndef _LEA_CBC_MOVS_H
#define _LEA_CBC_MOVS_H

#define INITIAL_BUF_SIZE 1500
#define MAX_LINE_LENGTH 150

typedef struct {
    u32 key[4];     // Fixed 128 bits for key
    u32 iv[4];      // Fixed 128 bits for iv
    u32* pt;        // Pointer for arbitrary length plaintext
    size_t ptLength; // Length of pt
    u32* ct;        // Pointer for arbitrary length ciphertext
    size_t ctLength; // Length of ct
} CryptoData;


void freeCryptoData(CryptoData* cryptoData);
size_t determineLength(const char* hexString);
int readCryptoData(FILE* fp, CryptoData* cryptoData);
void parseHexLineVariable(u32* arr, const char* line, size_t length);
bool compareCryptoData(const CryptoData* data1, const CryptoData* data2);

// const char* skipPrefix(const char* line);
// char* parseLine(const char* line);
// char* readLineAndAllocate(FILE* fp, char* line);
// int readDataSet(FILE* fp, DataSet* dataSet);
// bool compareDataSets(const DataSet* data1, const DataSet* data2);

#if 1
typedef struct {
    u32 key[4];
    u32 iv[4];
    u32 pt[4];
    u32 ct[4];
} DataSet;

void printProgressBar(int current, int total);

int readDataSet(FILE* fp, DataSet* dataSet);
bool compareDataSets(const DataSet* data1, const DataSet* data2);

void parseHexLine(u32* arr, const char* line);
const char* skipPrefix(const char* line);
#endif

void create_LEA128CBC_KAT_ReqFile(const char* inputFileName, const char* outputFileName);
void create_LEA128CBC_KAT_FaxFile(const char* inputFileName, const char* outputFileName);
void create_LEA128CBC_KAT_RspFile(const char* inputFileName, const char* outputFileName);
void MOVS_LEA128CBC_KAT_TEST();

void create_LEA128CBC_MMT_ReqFile(const char* inputFileName, const char* outputFileName);
void create_LEA128CBC_MMT_FaxFile(const char* inputFileName, const char* outputFileName);
void create_LEA128CBC_MMT_RspFile(const char* inputFileName, const char* outputFileName);
void MOVS_LEA128CBC_MMT_TEST();

#endif // _LEA_CBC_MOVS_H