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
#include "lea_movs.h"

int main(int argc, char** argv) {
    /* CBC: Cipher Block Chaining */

    // lea128_test();
    // lea192_test();
    // lea256_test();
    
    // CBC_lea128_test();

    // MOVS_LEA128CBC_KAT_TEST();
    // MOVS_LEA128CBC_MMT_TEST();
    // MOVS_LEA128CBC_MCT_TEST();

    MOVS_LEA128CBC();
    puts("");
    MOVS_LEA128CTR();

    /* CTR: CounTeR */

    // CTR_lea128_test();

    // MOVS_LEA128CTR_KAT_TEST();
    // MOVS_LEA128CTR_MMT_TEST();
    // MOVS_LEA128CTR_MCT_TEST();

    // const char* folderPath = "../LEA128(CTR)MOVS/";
    // char txtFileName[50];
    // char reqFileName[50];
    // char faxFileName[50];
    // char rspFileName[50];
    
    // // Construct full paths for input and output files
    // snprintf(txtFileName, sizeof(txtFileName), "%s%s", folderPath, "LEA128(CTR)MCT.txt");
    // snprintf(reqFileName, sizeof(reqFileName), "%s%s", folderPath, "LEA128(CTR)MCT.req");
    // snprintf(faxFileName, sizeof(faxFileName), "%s%s", folderPath, "LEA128(CTR)MCT.fax");
    // snprintf(rspFileName, sizeof(rspFileName), "%s%s", folderPath, "LEA128(CTR)MCT.rsp");
    
    // create_LEA_CTR_MCT_ReqFile(txtFileName, reqFileName);
    // create_LEA_CTR_MCT_FaxFile(txtFileName, faxFileName);
    // create_LEA_CTR_MCT_RspFile(reqFileName, rspFileName);

    return 0;
}
#endif
