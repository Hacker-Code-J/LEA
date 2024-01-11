/**
 * @file main.c
 * @brief Main entry point for the LEA encryption algorithm demonstration.
 *
 * This file contains the main function to demonstrate the usage of the LEA
 * encryption algorithm. It may include example usage or benchmarking.
 */

#include "lea.h"
#include "lea_modes.h"

int main(void) {
    lea128_test();
    // lea192_test();
    // lea256_test();

    // lea128_ECB_test();
    
    return 0;
}
