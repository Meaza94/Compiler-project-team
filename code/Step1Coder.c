/*
 
      TEAM MEMBERS

Student Name: Mihretab Meaza
student Number: 041120078
Professor: Paulo Sousa
Compilers Assignment 1 
language name: mplusplus


Student Name: Mohamad Al Dakkak
student Number: 041106564
Professor: Paulo Sousa
Compilers Assignment 1
language name: mplusplus

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
    FILE* inputFile = NULL; // Pointer for the input file
    FILE* outputFile = NULL; // Pointer for the output file

    // TO_DO: Use defensive programming (checking files)
    inputFile = fopen(inputFileName, "r");


    if (inputFile == NULL) { // Check if the file is empty or not found in the same path
        printf("Error opening input file: %s\n", inputFileName);
        exit(EXIT_FAILURE);//question for the teacher: exit 0
    }

	// TO_DO: Open the output file for writing
    outputFile = fopen(outputFileName, "w");

    if (outputFile == NULL) { // Check if the file is empty or not found in the same path
        printf("Error opening output file: %s\n", outputFileName);
        fclose(inputFile); // Close the input file before exiting
        exit(EXIT_FAILURE);
    }

	// TO_DO: Define local variables for Vigenère Cipher logic
    mplusplus_intg KeyIndex = 0; // To track the index of the key
    mplusplus_intg KeyLength = strlen(key); // The length of the key mplusplus_strg key = "KEY"; // The key used for encoding/decoding
    mplusplus_char ch = 0; // Character to read from the input file and to be encoded or decoded

    // TO_DO: Logic: check if it is encode / decode to change the char (using Vigenere algorithm) - next function
    while ((ch = fgetc(inputFile)) != EOF) { // EOF = End of File. When reaching the end of the file, it will stop looping (reading from the file).
        if (ch >= ASCII_START && ch <= ASCII_END) { // Check if the character is within the ASCII Domain
            mplusplus_char keyChar = key[KeyIndex % KeyLength]; // Get the corresponding character from the key
            mplusplus_intg shift = keyChar - ASCII_START; // Calculate the shift based on the key character

            if (encode == CYPHER) {//0=CYPHER
                ch = ((ch - ASCII_START + shift) % ASCII_RANGE) + ASCII_START;
            }
            else if (encode == DECYPHER) {//1=DECYPHER
                ch = ((ch - ASCII_START - shift + ASCII_RANGE) % ASCII_RANGE) + ASCII_START;
            }
            KeyIndex++; // Move to the next character in the key
        }

        fputc(ch, outputFile); // Write the transformed character to the output file
    }

    // TO_DO: Close the files
    fclose(inputFile);
    fclose(outputFile);

    printf("Operation completed successfully. Output saved to '%s'.\n", outputFileName);
}

// Function to perform the Vigenère cipher (encoding or decoding)
mplusplus_strg vigenereMem(const mplusplus_strg inputFileName, const mplusplus_strg key, mplusplus_intg encode) {
    // TO_DO: Defensive Programming - Validate Inputs
    if (!inputFileName || !key) { // Check if inputFileName or key is NULL
        printf("Error: Invalid arguments provided to vigenereMem.\n");
        return NULL; // Return NULL for invalid inputs
    }

    // Step 1: Get the size of the input file
    mplusplus_intg size = getSizeOfFile(inputFileName);
    if (size == -1) { // Check if getSizeOfFile failed
        printf("Error: Unable to determine the size of file '%s'.\n", inputFileName);
        return NULL; // Return NULL if the file size cannot be determined
    }
    //Note: finding the size of the file is related to the allocating memory for the output string(To achive the dynmaic memory)
    //and malloc require the programmer to specify the amount of memory to allocate. 
    // Step 2: Allocate memory for the output string (+1 for null terminator) due that the memory start form index 0
    mplusplus_strg output = (mplusplus_strg)malloc(size + 1);
    if (!output) { // Check if malloc failed
        printf("Error: Memory allocation failed.\n");
        return NULL; // Return NULL if memory allocation fails
    }

    // Step 3: Open the input file for reading
    FILE* inputFile = fopen(inputFileName, "r");
    if (!inputFile) { // Check if the file can be opened
        printf("Error: Unable to open input file '%s'.\n", inputFileName);
        free(output); // Free allocated memory before exiting
        return NULL; // Return NULL if the file cannot be opened
    }

    // Step 4: Initialize Local varaibles variables for Vigenère Cipher logic
    mplusplus_intg KeyIndex = 0; // Tracks the position in the key
    mplusplus_intg KeyLength = strlen(key); // Length of the key
    mplusplus_char ch; // Character read from the input file
    mplusplus_intg outputIndex = 0; // Index for writing to the output string

    // Step 5: Process the file character by character(same as the vigener file function)
    while ((ch = fgetc(inputFile)) != EOF) { // Read the file character by character
        if (ch >= ASCII_START && ch <= ASCII_END) { // Check if the character is within the valid ASCII domane
            mplusplus_char keyChar = key[KeyIndex % KeyLength]; // Get the corresponding key character
            mplusplus_intg shift = keyChar - ASCII_START; // Calculate the shift value

            if (encode == CYPHER) {
                // Encryption logic
                ch = ((ch - ASCII_START + shift) % ASCII_RANGE) + ASCII_START;
            }
            else if (encode == DECYPHER) {
                // Decryption logic
                ch = ((ch - ASCII_START - shift + ASCII_RANGE) % ASCII_RANGE) + ASCII_START;
            }

            KeyIndex++; // Move to the next character in the key
        }

        // Write the transformed character to the output string
        output[outputIndex++] = ch;
    }

    // Step 6: Null-terminate the output string
    output[outputIndex] = '\0';

    // Step 7: Close the input file
    fclose(inputFile);

    // Step 8: Return the output string
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
    // Defensive Programming: Validate input
    if (!filename) { // Check if the filename is NULL
        printf("Error: Invalid filename provided to getSizeOfFile.\n");
        return -1; // Return an error code (-1) for invalid input
    }

    // Open the file in binary mode for reading
    FILE* file = fopen(filename, "rb");
    if (!file) { // Check if the file can be opened
        printf("Error: Unable to open file '%s' in getSizeOfFile.\n", filename);
        return -1; // Return an error code (-1) if the file cannot be opened
    }

    // Move the file pointer to the end of the file
    if (fseek(file, 0, SEEK_END) != 0) { // Check if fseek fails
        printf("Error: Unable to seek to the end of file '%s'.\n", filename);
        fclose(file); // Close the file before exiting
        return -1; // Return an error code (-1) if fseek fails
    }

    // Get the current position of the file pointer using ftell (size of the file)
    mplusplus_intg size = ftell(file);
    if (size == -1L) { // Check if ftell fails
        printf("Error: Unable to determine the size of file '%s'.\n", filename);
        fclose(file); // Close the file before exiting
        return -1; // Return an error code (-1) if ftell fails
    }

    // Close the file
    fclose(file);

    // Return the size of the file
    return size;
}
