/**
 * @file lea_movs.h
 * @brief Header file for defining Mode of Operations Validation System (MOVS) or related structures/functions used in the LEA algorithm.
 * 
 */

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
    u32* key;               // Pointer for arbitrary length key
    u32* iv;                // Pointer for arbitrary length iv
    u32* pt;                // Pointer for arbitrary length plaintext
    u32* ct;                // Pointer for arbitrary length ciphertext
    size_t keyLength;       // Length of pt or ct
    size_t dataLength;      // Length of pt or ct
} CryptoData;               // 48-byte (8 * 6)

/* lea_movs */

void printProgressBar(int current, int total);

void freeCryptoData(CryptoData* pData);
void parseHexLine(u32* pU32dst, const char* pI8src, size_t length);
size_t wordLength(const char* pHexString);

bool readCryptoData(FILE* fp, CryptoData* pData);
bool compareCryptoData(const CryptoData* pData1, const CryptoData* pData2);

void printHexToFile(FILE* pFile, const u32* data, size_t length);
// void xorBuffers(u32* result, const u32* a, const u32* b, size_t length);

/* lea_cbc_kat */

void create_LEA_CBC_KAT_ReqFile(const char* pTxtFileName, const char* pReqFileName);
void create_LEA_CBC_KAT_FaxFile(const char* pTxtFileName, const char* pFaxFileName);
void create_LEA_CBC_KAT_RspFile(const char* pReqFileName, const char* pRspFileName);
void MOVS_LEA128CBC_KAT_TEST(void);

/* lea_cbc_mmt */

void create_LEA_CBC_MMT_ReqFile(const char* pTxtFileName, const char* pReqFileName);
void create_LEA_CBC_MMT_FaxFile(const char* pTxtFileName, const char* pFaxFileName);
void create_LEA_CBC_MMT_RspFile(const char* pReqFileName, const char* pRspFileName);
void MOVS_LEA128CBC_MMT_TEST(void);

/* lea_cbc_mct */

void create_LEA_CBC_MCT_ReqFile(const char* pTxtFileName, const char* pReqFileName);
void create_LEA_CBC_MCT_FaxFile(const char* pTxtFileName, const char* pFaxFileName);
void create_LEA_CBC_MCT_RspFile(const char* pReqFileName, const char* pRspFileName);
void MOVS_LEA128CBC_MCT_TEST(void);

/* lea_cbc_movs */

void MOVS_LEA128CBC(void);

/* lea_ctr_kat */

void create_LEA_CTR_KAT_ReqFile(const char* pTxtFileName, const char* pReqFileName);
void create_LEA_CTR_KAT_FaxFile(const char* pTxtFileName, const char* pFaxFileName);
void create_LEA_CTR_KAT_RspFile(const char* pReqFileName, const char* pRspFileName);
void MOVS_LEA128CTR_KAT_TEST(void);

/* lea_ctr_mmt */

void create_LEA_CTR_MMT_ReqFile(const char* pTxtFileName, const char* pReqFileName);
void create_LEA_CTR_MMT_FaxFile(const char* pTxtFileName, const char* pFaxFileName);
void create_LEA_CTR_MMT_RspFile(const char* pReqFileName, const char* pRspFileName);
void MOVS_LEA128CTR_MMT_TEST(void);

/* lea_ctr_mct */

void create_LEA_CTR_MCT_ReqFile(const char* pTxtFileName, const char* pReqFileName);
void create_LEA_CTR_MCT_FaxFile(const char* pTxtFileName, const char* pFaxFileName);
void create_LEA_CTR_MCT_RspFile(const char* pReqFileName, const char* pRspFileName);
void MOVS_LEA128CTR_MCT_TEST(void);

/* lea_ctr_movs */

void MOVS_LEA128CTR(void);

#endif /* _LEA_CBC_MOVS_H */