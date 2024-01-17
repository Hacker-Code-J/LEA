#include <stdbool.h>

#include "lea_modes.h"

#ifndef _LEA_CBC_MOVS_H
#define _LEA_CBC_MOVS_H

#define INITIAL_BUF_SIZE 1500
#define MAX_LINE_LENGTH 1034
#define MAX_TXT_SIZE 1028

// +--------------+----------------+--------------+-----------------+
// |     pt       |    ptLength    |      ct      |   ctLength      |
// | (u32* 8 byte)| (size_t 8 byte)| (u32* 8 byte)| (size_t 8 byte) |
// +--------------+----------------+--------------+-----------------+
// |                           key[4]                               |
// |                  (u32 4*4 bytes = 16 bytes)                    |
// +----------------------------------------------------------------+
// |                           iv[4]                                |
// |                  (u32 4*4 bytes = 16 bytes)                    |
// +----------------------------------------------------------------+
//  Total size: 64 bytes
typedef struct {
    u32* pt;                // Pointer for arbitrary length plaintext
    size_t ptLength;        // Length of pt
    u32* ct;                // Pointer for arbitrary length ciphertext
    size_t ctLength;        // Length of ct
    u32 key[4];             // Fixed 128 bits for key
    u32 iv[4];              // Fixed 128 bits for iv
} CryptoData;               // 64-byte (8 + 8 + 8 + 8 + 16 + 16)

// typedef struct {
//     u32* key;               // Pointer for arbitrary length key
//     u32* iv;                // Pointer for arbitrary length iv
//     size_t keyLength;       // Length of pt or ct
//     u32* pt;                // Pointer for arbitrary length plaintext
//     u32* ct;                // Pointer for arbitrary length ciphertext
//     size_t dataLength;      // Length of pt or ct
// } CryptoData;               // 48-byte (8 * 6)

// +--------------+-----------------+------------------------------+
// |  pt/ct union |   dataLength    |          key[4]              |
// | (u32* 8 byte)| (size_t 8 byte) | (u32 4*4 bytes = 16 bytes)   |
// +--------------+--------------+---------------------------------+
// |                           iv[4]                               |
// |                  (u32 4*4 bytes = 16 bytes)                   |
// +---------------------------------------------------------------+
//  Total size: 48 bytes
// typedef struct {
//     union {
//         u32* pt;        // Pointer for arbitrary length plaintext
//         u32* ct;        // Pointer for arbitrary length ciphertext
//     };
//     size_t dataLength;   // Length of pt or ct
//     u32 key[4];          // Fixed 128 bits for key
//     u32 iv[4];           // Fixed 128 bits for IV
// } CryptoData;            // 48-byte (8 + 8 + 16 + 16)


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

void parseHexLine(u32* pArr, const char* pLine);
const char* skipPrefix(const char* line);
#endif

void create_LEA128CBC_KAT_ReqFile(const char* inputFileName, const char* outputFileName);
void create_LEA128CBC_KAT_FaxFile(const char* inputFileName, const char* outputFileName);
void create_LEA128CBC_KAT_RspFile(const char* inputFileName, const char* outputFileName);
void MOVS_LEA128CBC_KAT_TEST();

void create_LEA128CBC_MMT_ReqFile(const char* pTxtFileName, const char* pReqFileName);
void create_LEA128CBC_MMT_FaxFile(const char* pTxtFileName, const char* pFaxFileName);
void cbcEncrypt(CryptoData* data);
void create_LEA128CBC_MMT_RspFile(const char* pReqFileName, const char* pRspFileName);
void MOVS_LEA128CBC_MMT_TEST();

void create_LEA128CBC_MCT_ReqFile(const char* pTxtFileName, const char* pReqFileName);
void create_LEA128CBC_MCT_FaxFile(const char* pTxtFileName, const char* pFaxFileName);
void create_LEA128CBC_MCT_RspFile(const char* pReqFileName, const char* pRspFileName);
void MOVS_LEA128CBC_MCT_TEST();

#endif // _LEA_CBC_MOVS_H