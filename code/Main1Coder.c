/*

	  TEAM MEMBERS

Student Name: Mihretab Meaza
student Number: 041106564
Professor: Paulo Sousa
Compilers Assignment 1
language name: mplusplus


Student Name: Mohamad Al Dakkak
student Number: 041120078
Professor: Paulo Sousa
Compilers Assignment 1
language name: mplusplus

*/


/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@           %&@@@@@@@@@@@     @@    ”
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    ”
# ECHO "    @@      @& @   @ @       @     @@    ”
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    ”
# ECHO "    @@      & @ @  @@              @@    ”
# ECHO "    @@       @/ @*@ @ @   @        @@    ”
# ECHO "    @@           @@@@  @@ @ @      @@    ”
# ECHO "    @@            /@@    @@@ @     @@    ”
# ECHO "    @@     @      / /     @@ @     @@    ”
# ECHO "    @@     @ @@   /@/   @@@ @      @@    ”
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@         S O F I A           @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[CODER SCRIPT ..........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: MainCoder.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
* Function list: (...).
*************************************************************/

/*
 *.............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: Preprocessor directives
 * The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define directive does not have any effect on other compiler projects 
 * (Gcc, VSCode, Codeblocks, etc.).
 *.............................................................................
 */

#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

// is there any denfine for this is my compiler.h 
#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

 /* Check for ANSI C compliancy */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/*
 * TODO .......................................................................
 * Basically, change all datatypes to your language definitions
 * (see "Compilers.h")
 */

/*
 * -------------------------------------------------------------
 *  Function declarations
 * -------------------------------------------------------------
 */

/*
************************************************************
* Coder starting method
* Params:
*	- Mode: Cypher/decypher
*	- Input: Filename
*   - Key: Encrypt word
*	- Output: Filename
************************************************************
*/

mplusplus_intg main1Coder(mplusplus_intg argc, mplusplus_strg* argv) {
    if (argc < 5) {
        printf("Usage: %s [cypher=1|decypher=0] <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    mplusplus_strg operation = "";
    mplusplus_strg inputFileName = "";
    mplusplus_strg key = STR_LANGNAME;
    mplusplus_strg outputFileName = "";

    if (argc > 4) {
        operation = argv[2];
        inputFileName = argv[3];
        outputFileName = argv[4];

        if (atoi(operation) == CYPHER)
            cypher(inputFileName, outputFileName, key);
        else if (atoi(operation) == DECYPHER)
            decypher(inputFileName, outputFileName, key);
        else {
            errorPrint("Error: Unknown operation %s. Use 'cypher' or 'decypher'.\n", operation);
            return EXIT_FAILURE;
        }

        printf("Operation '%s' completed successfully.\n", operation);

        mplusplus_intg size = (mplusplus_intg)getSizeOfFile(outputFileName);
        if (size > 0) {
            FILE* file = fopen(outputFileName, "r");
            if (file) {
                mplusplus_strg output = (mplusplus_strg)malloc(size + 1);
                if (output) {
                    mplusplus_intg bytesRead = (mplusplus_intg)fread(output, 1, size, file);
                    output[bytesRead] = '\0';
                    printf("OUTPUT:\n%s\n", output);
                    free(output);
                }
                fclose(file);
            } else {
                errorPrint("Failed to open output file for reading\n");
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}
