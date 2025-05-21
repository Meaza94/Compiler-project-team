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
void vigenereFile(const mplusplus_strg inputFileName, const mplusplus_strg outputFileName, const mplusplus_strg key, const mplusplus_intg encode) {
    // Function implementation remains unchanged


    // TO_DO: Define the input and output files (ex: FILE* inputFile, FILE* outputFile
    FILE* inputFile = NULL; // Pointer for the input file
    FILE* outputFile = NULL; // Pointer for the output file

    // TO_DO: Use defensive programming (checking files)
    inputFile = fopen(inputFileName, "r");

    //Mohamad and maza
    if (inputFile == NULL) { // Check if the file is empty or not found in the same path
        printf("Error opening input file: %s\n", inputFileName);
        exit(EXIT_FAILURE);//question for the teacher: exit 0
    }

	// TO_DO: Open the output file for writing
    outputFile = fopen(outputFileName, "w");

    if (outputFile == NULL) { // Check if the file is empty or not found in the same path
        printf("Error opening output file: %s\n", outputFileName);
        fclose(inputFile); // Close the input file before exiting
		exit(EXIT_FAILURE); // Exit with failure status if the output file cannot be opened
    }


	// TO_DO: Define local variables for Vigenère Cipher logic
    mplusplus_intg KeyIndex = 0; // To track the index of the key
    mplusplus_intg KeyLength = strlen(key); // The length of the key mplusplus_strg key = "KEY"; // The key used for encoding/decoding
    mplusplus_char ch = 0; // Character to read from the input file and to be encoded or decoded



// ============================================================================
// Begin reading the input file character by character using fgetc().
// The goal is to encrypt or decrypt each character using a repeating key
// (in this case, "mplusplus") which is 9 characters long.
//
// Here's how it works:
// 1. 'ch' holds each character read from the file.
// 2. 'keyIndex' tracks the current position in the key string.
// 3. We apply an encryption/decryption operation using the key character
//    corresponding to 'keyIndex'.
//
// Since the key is shorter than the file content, we cycle through the key
// repeatedly. This is done by incrementing 'keyIndex' after each operation,
// and resetting it back to 0 once it reaches the length of the key 8.
//
//
// The loop continues until the end-of-file (EOF) is reached, ensuring every
// character in the file is processed exactly once.
// ============================================================================


    // TO_DO: Logic: check if it is encode / decode to change the char (using Vigenere algorithm) - next function
 while ((ch = fgetc(inputFile)) != EOF) { // EOF = End of File. When reaching the end of the file, it will stop looping (reading from the file).
        if (ch >= ASCII_START && ch <= ASCII_END) { // Check if the character is within the ASCII Domain
            mplusplus_char keyChar = key[KeyIndex % KeyLength]; // Get the corresponding character from the key
            mplusplus_intg shift = keyChar - ASCII_START; // Calculate the shift based on the key character

			if (encode == CYPHER) {// 1=CYPHER
                ch = ((ch - ASCII_START + shift) % ASCII_RANGE) + ASCII_START;
            }
            else if (encode == DECYPHER) {//0=DECYPHER
                ch = ((ch - ASCII_START - shift + ASCII_RANGE) % ASCII_RANGE) + ASCII_START;
            }
            KeyIndex++; // Move to the next character in the key
        }

        fputc(ch, outputFile); // Write the transformed character to the output file

	}//end of while loop

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

    

    // ============================================================================
// Begin reading the input file character by character using fgetc().
// The goal is to encrypt or decrypt each character using a repeating key
// (in this case, "mplusplus") which is 9 characters long.
//
// Here's how it works:
// 1. 'ch' holds each character read from the file.
// 2. 'keyIndex' tracks the current position in the key string.
// 3. We apply an encryption/decryption operation using the key character
//    corresponding to 'keyIndex'.
//
// Since the key is shorter than the file content, we cycle through the key
// repeatedly. This is done by incrementing 'keyIndex' after each operation,
// and resetting it back to 0 once it reaches the length of the key (i.e. 9).
//
// This approach follows the principle of the Vigenère cipher, where a repeated
// key is used to transform each character of the plaintext or ciphertext.
// This makes the encryption more secure than a simple Caesar cipher.
//
// The loop continues until the end-of-file (EOF) is reached, ensuring every
// character in the file is processed exactly once.
// ============================================================================
  

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


// ============================================================================
// Function: cypher
// Purpose : Encodes (encrypts) the contents of an input file using a given key.
//           This function wraps around the generic 'vigenereFile' function,
//           passing the mode CYPHER to specify encryption.
// Parameters:
//   - inputFileName  : Name of the file to read from (plaintext).
//   - outputFileName : Name of the file to write to (ciphertext).
//   - key            : The encryption key to be used in the cipher.
// ============================================================================

// Function to encode (cypher)
void cypher(const mplusplus_strg inputFileName, const mplusplus_strg outputFileName, const mplusplus_strg key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER);
}


// ============================================================================
// Function: decypher
// Purpose : Decodes (decrypts) the contents of an input file using a given key.
//           Similar to cypher, but sets the mode to DECYPHER to reverse the
//           encryption process.
// Parameters:
//   - inputFileName  : Name of the file to read from (ciphertext).
//   - outputFileName : Name of the file to write to (plaintext).
//   - key            : The decryption key to be used (should match the key
//                      used to encrypt the file).
// ============================================================================


// Function to decode (decypher)
void decypher(const mplusplus_strg inputFileName, const mplusplus_strg outputFileName, const mplusplus_strg key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}


// ============================================================================
// Function: getSizeOfFile
// Purpose : Utility method to determine the size of a given file in bytes.
//           Useful for diagnostics, file operations, or buffer allocation.
//
// Steps:
//   1. Validate that the filename provided is not NULL.
//   2. Attempt to open the file in binary mode ("rb").
//   3. Move the file pointer to the end using fseek to determine its size.
//   4. Use ftell to get the current position (which will be the file size).
//   5. Handle any errors during these steps gracefully and return -1 if needed.
//
// Parameters:
//   - filename : Name of the file whose size is to be determined.
//
// Returns:
//   - File size in bytes on success.
//   - -1 if an error occurs (invalid filename, failed file access, etc.).
// ============================================================================

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
