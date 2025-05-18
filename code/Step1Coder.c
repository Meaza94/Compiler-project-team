/*
Name: Mihretab Meaza
* Student ID: 041106564




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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

// Function to perform the Vigenère cipher (encoding or decoding)
void vigenereFile(const mplusplus_strg inputFileName, const mplusplus_strg outputFileName, const mplusplus_strg key, mplusplus_intg encode) {
	// TO_DO: Define the input and output files (ex: FILE* inputFile, FILE* outputFile

	/*
	 * STEP 1: Open files for reading and writing
	 * "r" = read mode (text mode)
	 * "w" = write mode (creates new file or overwrites existing)
	 */
    FILE* inputFile = fopen(inputFileName, "r"); 
	FILE* outputFile = fopen(outputFileName, "w");


	// TO_DO: Use defensive programming (checking files)
    if(!inputFile || !outputFile) {
	

		// Clean up: close any file that DID open successfully
        if (inputFile) fclose(inputFile);
        if (outputFile) fclose(outputFile);


		// Report error using the error printing function
		errorPrint("Error opening files\n");
		return; //Exit early - can't proceed without files
	}




	// TO_DO: Define local variables
	// STEP 2: Define local variables
	mplusplus_intg ch;                // Current character being processed for 
	mplusplus_intg keyIndex = 0;            // Current position in the key for encoding/decoding
	mplusplus_intg keyLen = strlen(key);    // Length of the key "MPlusPlus" for encoding/decoding

	
// TO_DO: Logic: check if it is encode / decode to change the char (using Vigenere algorithm) - next function
// STEP 3: Process each character from input file
    while ((ch = fgetc(inputFile)) != EOF) {
      // Only process visible ASCII characters (32-126)
        if (ch >= ASCII_START && ch <= ASCII_END) {
            // Get shift value from current key character
            mplusplus_intg shift = key[keyIndex % keyLen] - ASCII_START;

            // Apply cipher transformation based on mode
            if (encode == CYPHER) {
                // Encryption: shift forward
                ch = ((ch - ASCII_START + shift) % ASCII_RANGE) + ASCII_START;
            }
            else {
                // Decryption: shift backward
                ch = ((ch - ASCII_START - shift + ASCII_RANGE) % ASCII_RANGE) + ASCII_START;
            }

            // Move to next key character
			keyIndex++;
        }

        // Write processed character to output file
        fputc(ch, outputFile);
    }

    // STEP 4: Close the files
    fclose(inputFile);
    fclose(outputFile);
}


// Function to perform the Vigenère cipher (encoding or decoding)
mplusplus_strg vigenereMem(const mplusplus_strg inputFileName, const mplusplus_strg key, mplusplus_intg encode) {
	// TO_DO define the return type and local variables
    // Define the return type and local variables
    mplusplus_strg output = NULL;

    // Check defensive programming
    if (inputFileName == NULL || key == NULL) {
        errorPrint("Input file name or key is NULL\n");
        return NULL; // Exit early
    }

    // Get the size of the file
    mplusplus_intg size = getSizeOfFile(inputFileName);

    // Check if file exists and has content
    if (size <= 0) {
        errorPrint("File doesn't exist or is empty\n");
        return NULL;
    }

    // Allocate memory for the output string
    output = (mplusplus_strg)malloc(size + 1); // +1 for null terminator
    if (output == NULL) {
        errorPrint("Memory allocation failed\n");
        return NULL;
    }

    // Read file content
    FILE* file = fopen(inputFileName, "r");
    if (!file) {
        free(output);  // Free allocated memory
        errorPrint("Failed to open file for reading\n");
        return NULL;   // Couldn't open file
    }

    // Read entire file into memory - CORRECTED LINE
    mplusplus_intg bytesRead = fread(output, 1, size, file);
    output[bytesRead] = '\0';  // Null-terminate
    fclose(file);

    // Apply cipher to memory
    mplusplus_intg keyLen = strlen(key);
    mplusplus_intg keyIndex = 0;


	// Process each character in the output string
    for (mplusplus_intg i = 0; i < bytesRead; i++) {
        if (output[i] >= ASCII_START && output[i] <= ASCII_END) {
            // Get shift from current key character
            mplusplus_intg shift = key[keyIndex % keyLen] - ASCII_START;

            if (encode == CYPHER) {
                // Encryption
                output[i] = ((output[i] - ASCII_START + shift) % ASCII_RANGE) + ASCII_START;
            }
            else {
                // Decryption
                output[i] = ((output[i] - ASCII_START - shift + ASCII_RANGE) % ASCII_RANGE) + ASCII_START;
            }

            keyIndex++;
        }
    }

    return output;
}

// Function to encode (cypher)
void cypher(const mplusplus_strg inputFileName, const mplusplus_strg outputFileName, const mplusplus_strg key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER);

}

// Function to decode (decypher)
void decypher(const mplusplus_strg inputFileName, const mplusplus_strg outputFileName, const mplusplus_strg key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}

// TO_DO: Get file size (util method)
mplusplus_intg getSizeOfFile(const mplusplus_strg filename) {
    // Declare size variable once
    mplusplus_intg size = 0;

    // Open the file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        errorPrint("Error opening file: %s\n", filename);
        return -1; // Error
    }

    // Seek to the end of the file
    fseek(file, 0, SEEK_END);

    // Get the current position in the file (updates the existing size variable)
    size = ftell(file);

    // Close the file
    fclose(file);

    return size;
}

