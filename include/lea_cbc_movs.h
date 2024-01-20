#include <stdbool.h>

#include "lea_modes.h"

#ifndef _LEA_CBC_MOVS_H
#define _LEA_CBC_MOVS_H

#define MAX_LINE_LENGTH 1034
#define MAX_TXT_SIZE 1028

/*
 * +--------------+----------------+
 * |     key      |    keyLength   |
 * | (u32* 8 byte)| (size_t 8 byte)|
 * +--------------+----------------+
 * |      iv      |   dataLength   |
 * | (u32* 8 byte)| (size_t 8 byte)|
 * +--------------+----------------+
 * |     pt       |      ct        |
 * | (u32* 8 byte)| (size_t 8 byte)|
 * +--------------+----------------+
*/
typedef struct {
    size_t keyLength;       // Length of pt or ct
    u32* key;               // Pointer for arbitrary length key
    u32* iv;                // Pointer for arbitrary length iv
    size_t dataLength;      // Length of pt or ct
    u32* pt;                // Pointer for arbitrary length plaintext
    u32* ct;                // Pointer for arbitrary length ciphertext
} CryptoData;               // 48-byte (8 * 6)

/* lea_cbc_movs */

void printProgressBar(int current, int total);

void freeCryptoData(CryptoData* pData);
void parseHexLine(u32* pU32dst, const char* pI8src, size_t length);
size_t wordLength(const char* pHexString);

bool readCryptoData(FILE* fp, CryptoData* pData);
bool compareCryptoData(const CryptoData* pData1, const CryptoData* pData2);

/* lea_cbc_kat */

void create_LEA_CBC_KAT_ReqFile(const char* pTxtFileName, const char* pReqFileName);
void create_LEA_CBC_KAT_FaxFile(const char* pTxtFileName, const char* pFaxFileName);
void create_LEA_CBC_KAT_RspFile(const char* pReqFileName, const char* pRspFileName);
void MOVS_LEA128CBC_KAT_TEST();

/* lea_cbc_mmt */

void create_LEA_CBC_MMT_ReqFile(const char* pTxtFileName, const char* pReqFileName);
void create_LEA_CBC_MMT_FaxFile(const char* pTxtFileName, const char* pFaxFileName);
void create_LEA_CBC_MMT_RspFile(const char* pReqFileName, const char* pRspFileName);
void MOVS_LEA128CBC_MMT_TEST();

/* lea_cbc_mct */

void create_LEA128CBC_MCT_ReqFile(const char* pTxtFileName, const char* pReqFileName);
void create_LEA128CBC_MCT_FaxFile(const char* pTxtFileName, const char* pFaxFileName);
void create_LEA128CBC_MCT_RspFile(const char* pReqFileName, const char* pRspFileName);
void MOVS_LEA128CBC_MCT_TEST();

#endif // _LEA_CBC_MOVS_H