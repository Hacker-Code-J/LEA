/**
 * @file main.c
 * @brief Main entry point for the LEA encryption algorithm demonstration.
 *
 * This file contains the main function to demonstrate the usage of the LEA
 * encryption algorithm. It may include example usage or benchmarking.
 */

#if 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lea.h"
#include "lea_modes.h"
#include "lea_cbc_movs.h"

int main(int argc, char** argv) {
    // lea128_test();
    // lea192_test();
    // lea256_test();
    
    CBC_lea128_test();

    // MOVS_LEA128CBC_KAT_TEST();
    // MOVS_LEA128CBC_MMT_TEST();

    // const char* folderPath = "../LEA128(CBC)MOVS/";
    // char txtFileName[50];
    // char reqFileName[50];
    // char faxFileName[50];
    // char rspFileName[50];

    // snprintf(txtFileName, sizeof(txtFileName), "%s%s", folderPath, "LEA128(CBC)MCT.txt");
    // snprintf(reqFileName, sizeof(reqFileName), "%s%s", folderPath, "LEA128(CBC)MCT.req");
    // snprintf(faxFileName, sizeof(faxFileName), "%s%s", folderPath, "LEA128(CBC)MCT.fax");
    // snprintf(rspFileName, sizeof(rspFileName), "%s%s", folderPath, "LEA128(CBC)MCT.rsp");

    // create_LEA_CBC_MCT_ReqFile(txtFileName, reqFileName);
    // create_LEA_CBC_MCT_FaxFile(txtFileName, faxFileName);
    // create_LEA_CBC_MCT_RspFile(reqFileName, rspFileName);

    return 0;
}
#endif
