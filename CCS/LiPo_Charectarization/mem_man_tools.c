/*
 * mem_man_tools.c
 *
 *  Created on: Feb 19, 2022
 *      Author: JacobFaseler
 */

/*
 * in the case of a need for memory optimization, the stack can be drastically reduced to free up SRAM.
 * The amount of SRAM currently being used, as well as its origin in memory can be found in MSP_EXP432P401R_TIRTOS.cmd
 * The following code snippet can be updated with values there=from and run to re-initialize SRAM values to a known state (0x55).
 * After initializing the SRAM, the program WILL NOT RUN. the snippet must be removed from execution and the code run again in order to
 * analyze how much space is being taken with the memory browser.
 * Whenever re-assigning the amount of SRAM to allocate to the heap, be sure to leave some buffer.
 *
 * To use, in main include #include "Deprecated/mem_man_tools.h" and call sram_wipe();
 */


void sram_wipe (){
    int  hi, hlen;                   /* Heap Iterator, Heap Length */
    int* hloc = (int*)0x20000000;    /* Heap Start Location */

    /* Initialize SRAM heap */
    hlen = 0x8000 / sizeof(char);
    for (hi = hlen ; hi >=0 ; hi--){
        *(hloc + (hi * sizeof(char))) = 'U';
    }
}
