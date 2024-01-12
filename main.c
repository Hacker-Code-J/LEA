/**
 * @file main.c
 * @brief Main entry point for the LEA encryption algorithm demonstration.
 *
 * This file contains the main function to demonstrate the usage of the LEA
 * encryption algorithm. It may include example usage or benchmarking.
 */

#include "lea.h"
#include "lea_modes.h"
#include "lea_cbc_movs.h"

int main(void) {
    // lea128_test();
    // lea192_test();
    // lea256_test();

    // lea128_ECB_test();

    const char* folderPath = "../LEA128(CBC)MOVS/";
    char txtFilePath[50];
    char reqFilePath[50];
    char faxFilePath[50];
    char rspFilePath[50];

    // Construct full paths for input and output files
    snprintf(txtFilePath, sizeof(txtFilePath), "%s%s", folderPath, "LEA128(CBC)KAT.txt");
    snprintf(reqFilePath, sizeof(reqFilePath), "%s%s", folderPath, "LEA128(CBC)KAT.req");
    snprintf(faxFilePath, sizeof(faxFilePath), "%s%s", folderPath, "LEA128(CBC)KAT.fax");
    snprintf(rspFilePath, sizeof(rspFilePath), "%s%s", folderPath, "LEA128(CBC)KAT.rsp");

    create_LEA128CBC_KAT_ReqFile(txtFilePath, reqFilePath);
    
    create_LEA128CBC_KAT_FaxFile(txtFilePath, faxFilePath);
    
    create_LEA128CBC_KAT_RspFile(reqFilePath, rspFilePath);
    
    return 0;
}
