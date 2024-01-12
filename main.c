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

int main(int argc, char** argv) {
    // lea128_test();
    // lea192_test();
    // lea256_test();

    // lea128_ECB_test();

    // const char* folderPath = "../LEA128(CBC)MOVS/";
    // char txtFileName[50];
    // char reqFileName[50];
    // char faxFileName[50];
    // char rspFileName[50];
    
    // // Construct full paths for input and output files
    // snprintf(txtFileName, sizeof(txtFileName), "%s%s", folderPath, "LEA128(CBC)KAT.txt");
    // snprintf(reqFileName, sizeof(reqFileName), "%s%s", folderPath, "LEA128(CBC)KAT.req");
    // snprintf(faxFileName, sizeof(faxFileName), "%s%s", folderPath, "LEA128(CBC)KAT.fax");
    // snprintf(rspFileName, sizeof(rspFileName), "%s%s", folderPath, "LEA128(CBC)KAT.rsp");
    
    // create_LEA128CBC_KAT_ReqFile(txtFileName, reqFileName);
    // create_LEA128CBC_KAT_FaxFile(txtFileName, faxFileName);
    // create_LEA128CBC_KAT_RspFile(reqFileName, rspFileName);
    
    MOVS_LEA128CBC_KAT_TEST();

    return 0;
}
