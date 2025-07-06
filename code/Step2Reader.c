/*

      TEAM MEMBERS

Student Name: Mihretab Meaza
student Number: 041106564
Professor: Paulo Sousa
Compilers Assignment 2
language name: mplusplus


Student Name: Mohamad Al Dakkak
student Number: 041120078
Professor: Paulo Sousa
Compilers Assignment 2
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
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "    @@                             @@    "
# ECHO "    @@           %&@@@@@@@@@@@     @@    "
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    "
# ECHO "    @@      @& @   @ @       @     @@    "
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    "
# ECHO "    @@      & @ @  @@              @@    "
# ECHO "    @@       @/ @*@ @ @   @        @@    "
# ECHO "    @@           @@@@  @@ @ @      @@    "
# ECHO "    @@            /@@    @@@ @     @@    "
# ECHO "    @@     @      / /     @@ @     @@    "
# ECHO "    @@     @ @@   /@/   @@@ @      @@    "
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    "
# ECHO "    @@                             @@    "
# ECHO "    @@         S O F I A           @@    "
# ECHO "    @@                             @@    "
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
***********************************************************
* File name: Step2Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file contains the implementation of the Reader/Buffer
*          data structure used by the compiler to manage input text.
*          The Reader provides dynamic memory management, character
*          access, and position tracking for parsing operations.
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *   Defensive Programming means always checking parameters for validity
 *   before using them to prevent crashes and undefined behavior.
 *.............................................................................
 */

 // Include standard C library headers for various operations
#include <ctype.h>   // For character classification functions (isprint, isdigit)
#include <string.h>  // For string manipulation functions (strlen, strchr)

// Include our custom header files
#ifndef COMPILERS_H_
#include "Compilers.h"    // Contains our custom data types (mplusplus_intg, etc.)
#endif

#ifndef CODER_H_
#include "Step1Coder.h"   // Contains encryption/decryption functions
#endif

#ifndef READER_H_
#include "Step2Reader.h"  // Contains Reader structure and function declarations
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates and initializes a new Reader/Buffer in memory
*          This is like creating a smart, expandable array for characters
*
* How it works:
* 1. Validates the requested size parameter
* 2. Allocates memory for the Reader structure itself
* 3. Allocates separate memory for the character content
* 4. Initializes all fields to their default values
* 5. Returns a pointer to the new Reader
*
* Parameters:
*   size = initial capacity for the character buffer (in characters)
*
* Return value:
*   BufferPointer = pointer to the newly created Reader
*   NULL = if creation failed (memory allocation error)
*
* Memory Layout Created:
*   [Reader Structure] ──► [Character Array]
*   Contains metadata     Contains actual text
*
* Algorithm: Dynamic memory allocation with defensive programming
*************************************************************
*/
BufferPointer readerCreate(mplusplus_intg size) {
    // Declare a pointer that will hold our new Reader structure
    BufferPointer readerPointer = NULL;

    /* ▼ DEFENSIVE PROGRAMMING: Validate the size parameter */
    // If user passed an invalid size (0 or negative), use a safe default
    if (size <= 0) {
        size = READER_DEFAULT_SIZE;  // Use 250 as default size
    }

    // If user requested more than maximum allowed, reject the request
    if (size > READER_MAX_SIZE) {
        /* Size too large - could cause memory issues */
        return NULL;  // Return failure
    }

    /* ▼ STEP 1: Allocate memory for the Reader structure itself */
    // calloc() allocates memory and initializes it to zero
    // sizeof(Buffer) gets the size needed for our Reader structure
    readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));

    /* ▼ DEFENSIVE PROGRAMMING: Check if allocation succeeded */
    if (readerPointer == NULL) {
        // Memory allocation failed - system is out of memory
        return NULL;  // Return failure indicator
    }

    /* ▼ STEP 2: Allocate memory for the character content array */
    // This is separate from the structure - it holds the actual text
    // malloc() allocates memory without initializing it
    // (size_t) casts our integer to the type malloc expects
    mplusplus_strg content = (mplusplus_strg)malloc((size_t)size * sizeof(mplusplus_char));

    /* ▼ DEFENSIVE PROGRAMMING: Check if content allocation succeeded */
    if (content == NULL) {
        // Content allocation failed - cleanup and return failure
        free(readerPointer);  // Free the structure we already allocated
        return NULL;         // Return failure indicator
    }

    /* ▼ STEP 3: Initialize the character frequency histogram to zero */
    // NCHAR = 128 (ASCII characters 0-127)
    // This loop sets all histogram counts to 0
    for (mplusplus_intg i = 0; i < NCHAR; i++) {
        readerPointer->histogram[i] = 0;  // No characters counted yet
    }

    /* ▼ STEP 4: Initialize error counter */
    readerPointer->numReaderErrors = 0;  // No errors encountered yet

    /* ▼ STEP 5: Connect the content array to our Reader structure */
    readerPointer->content = content;  // Point to our character array
    readerPointer->size = size;        // Remember how much memory we allocated

    /* ▼ STEP 6: Initialize all position indicators to zero */
    readerPointer->position.read = 0;  // Haven't read any characters yet
    readerPointer->position.wrte = 0;  // Haven't written any characters yet  
    readerPointer->position.mark = 0;  // No position is marked yet

    /* ▼ STEP 7: Initialize all status flags to their starting values */
    readerPointer->flags.isEmpty = MPLUSPLUS_TRUE;   // Buffer starts empty
    readerPointer->flags.isFull = MPLUSPLUS_FALSE;   // Buffer starts not full
    readerPointer->flags.isRead = MPLUSPLUS_FALSE;   // Haven't reached end yet
    readerPointer->flags.isMoved = MPLUSPLUS_FALSE;  // Memory hasn't moved yet

    /* ▼ The created Reader is ready to use - return pointer to it */
    return readerPointer;
}

/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a single character to the Reader/Buffer
*          This is the most complex function - it handles dynamic growth
*
* How it works:
* 1. Validates the Reader pointer and character
* 2. Checks if there's space in current buffer
* 3. If no space, doubles the buffer size (dynamic growth)
* 4. Adds the character at the write position
* 5. Updates statistics and positions
*
* Why it's complex:
* - Must handle memory reallocation when buffer gets full
* - Must detect if memory moved during reallocation
* - Must update multiple tracking variables
* - Must maintain data integrity throughout the process
*
* Parameters:
*   readerPointer = pointer to the Reader to add character to
*   ch = the character to add
*
* Return value:
*   BufferPointer = pointer to the Reader (may be different after realloc)
*   NULL = if addition failed
*
* Growth Strategy:
*   250 → 500 → 1000 → 2000 → 4000 (doubles each time)
*************************************************************
*/
BufferPointer readerAddChar(BufferPointer const readerPointer, mplusplus_char ch) {
    // Temporary pointer for memory reallocation - helps prevent data loss
    mplusplus_strg tempReader = NULL;
    // Variable to hold the new size if we need to grow the buffer
    mplusplus_intg newSize = 0;

    /* ▼ DEFENSIVE PROGRAMMING: Validate the Reader pointer */
    if (readerPointer == NULL) {
        // Caller passed a NULL pointer - cannot proceed
        return NULL;
    }

    /* ▼ DEFENSIVE PROGRAMMING: Validate the character */
    // Verify that ch is a valid ASCII character (0..127)
    // We cast to unsigned char to handle negative chars correctly
    if ((unsigned char)ch >= (unsigned char)NCHAR) {
        // Character is outside valid ASCII range
        readerPointer->numReaderErrors++;  // Increment error counter
        return NULL;  // Return failure
    }

    /* ▼ STEP 1: Check if we have space for the new character */
    // Calculate how many bytes are currently used in the buffer
    // wrte position * size of each character = total bytes used
    mplusplus_intg usedBytes = readerPointer->position.wrte * (mplusplus_intg)sizeof(mplusplus_char);

    if (usedBytes < readerPointer->size) {
        /* ▼ WE HAVE SPACE: Buffer is not full yet */
        readerPointer->flags.isFull = MPLUSPLUS_FALSE;  // Clear the full flag
        // We can skip to adding the character (goto step 3)
    }
    else {
        /* ▼ BUFFER IS FULL: Need to grow it before adding character */

        /* ▼ STEP 2A: Set the full flag since we're at capacity */
        readerPointer->flags.isFull = MPLUSPLUS_TRUE;

        /* ▼ STEP 2B: Calculate the new size (double current size) */
        newSize = readerPointer->size * 2;  // Exponential growth strategy

        /* ▼ DEFENSIVE PROGRAMMING: Validate the new size */
        // Check for integer overflow or exceeding maximum allowed size
        if (newSize <= 0 || newSize > READER_MAX_SIZE) {
            // New size is invalid (overflow or too large)
            readerPointer->numReaderErrors++;  // Count this as an error
            return NULL;  // Cannot grow buffer - return failure
        }

        /* ▼ STEP 2C: Attempt to reallocate memory to the new size */
        // realloc() tries to grow the existing memory block
        // If it can't grow in place, it allocates new memory and copies data
        tempReader = (mplusplus_strg)realloc(readerPointer->content,
            (size_t)newSize * sizeof(mplusplus_char));

        /* ▼ DEFENSIVE PROGRAMMING: Check if reallocation succeeded */
        if (tempReader == NULL) {
            /* realloc failed - leave original buffer intact */
            readerPointer->numReaderErrors++;  // Count this as an error
            return NULL;  // Return failure without destroying original data
        }

        /* ▼ STEP 2D: Check if memory moved during reallocation */
        // realloc() might move our data to a new memory location
        // We need to detect this for pointer safety
        if (tempReader != readerPointer->content) {
            // Memory was moved to a new location
            readerPointer->flags.isMoved = MPLUSPLUS_TRUE;  // Set the moved flag
        }

        /* ▼ STEP 2E: Update Reader structure with new memory information */
        readerPointer->content = tempReader;  // Point to new/expanded memory
        readerPointer->size = newSize;        // Update size to new capacity
        readerPointer->flags.isFull = MPLUSPLUS_FALSE;  // No longer full
    }

    /* ▼ STEP 3: Add the character to the buffer */
    // Place character at the current write position, then advance write position
    readerPointer->content[readerPointer->position.wrte++] = ch;

    /* ▼ STEP 4: Update character frequency statistics */
    // Increment the count for this character in our histogram
    // Cast to unsigned char to use as array index (0-127)
    readerPointer->histogram[(unsigned char)ch]++;

    /* ▼ STEP 5: Update checksum for data integrity */
    // Add this character's ASCII value to running checksum
    // Keep only the low 8 bits to prevent overflow
    readerPointer->checkSum = (mplusplus_byte)
        (((mplusplus_intg)readerPointer->checkSum + (mplusplus_intg)(unsigned char)ch) & 0xFF);

    /* ▼ STEP 6: Update buffer status */
    // Since we've added a character, buffer is no longer empty
    readerPointer->flags.isEmpty = MPLUSPLUS_FALSE;

    /* ▼ Return the Reader pointer (may have changed due to realloc) */
    return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Resets the Reader to its initial empty state
*          Keeps the allocated memory but clears all data and positions
*
* How it works:
* 1. Validates the Reader pointer
* 2. Resets all position indicators to zero
* 3. Resets all flags to initial state
* 4. Clears the character frequency histogram
* 5. Resets error counter and checksum
*
* Use case: When you want to reuse a Reader for new input
*           without deallocating and reallocating memory
*
* Parameters:
*   readerPointer = pointer to the Reader to clear
*
* Return value:
*   MPLUSPLUS_TRUE = clearing succeeded
*   MPLUSPLUS_FALSE = clearing failed (NULL pointer)
*************************************************************
*/
mplusplus_boln readerClear(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        // Cannot clear a NULL Reader
        return MPLUSPLUS_FALSE;
    }

    /* ▼ STEP 1: Reset all position indicators to starting values */
    readerPointer->position.read = 0;  // Reset read position to beginning
    readerPointer->position.wrte = 0;  // Reset write position to beginning
    readerPointer->position.mark = 0;  // Reset mark position to beginning

    /* ▼ STEP 2: Reset all flags to initial state */
    readerPointer->flags.isEmpty = MPLUSPLUS_TRUE;   // Buffer is now empty
    readerPointer->flags.isFull = MPLUSPLUS_FALSE;   // Buffer is not full
    readerPointer->flags.isRead = MPLUSPLUS_FALSE;   // Haven't reached end
    readerPointer->flags.isMoved = MPLUSPLUS_FALSE;  // Reset movement flag

    /* ▼ STEP 3: Clear the character frequency histogram */
    // Reset all character counts to zero
    for (mplusplus_intg i = 0; i < NCHAR; i++) {
        readerPointer->histogram[i] = 0;  // No characters counted
    }

    /* ▼ STEP 4: Reset error tracking and integrity checking */
    readerPointer->numReaderErrors = 0;  // Clear error count
    readerPointer->checkSum = 0;         // Reset checksum

    /* ▼ Reader is now in clean initial state */
    return MPLUSPLUS_TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Completely deallocates a Reader and all its memory
*          This is the cleanup function - prevents memory leaks
*
* How it works:
* 1. Validates the Reader pointer
* 2. Frees the character content array
* 3. Frees the Reader structure itself
*
* IMPORTANT: After calling this function, the Reader pointer
*           becomes invalid and should not be used again
*
* Parameters:
*   readerPointer = pointer to the Reader to deallocate
*
* Return value:
*   MPLUSPLUS_TRUE = deallocation succeeded
*   MPLUSPLUS_FALSE = deallocation failed (NULL pointer)
*
* Memory Management:
*   Before: [Reader Structure] ──► [Character Array]
*   After:  [Both freed - pointers invalid]
*************************************************************
*/
mplusplus_boln readerFree(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        // Cannot free a NULL pointer
        return MPLUSPLUS_FALSE;
    }

    /* ▼ STEP 1: Free the character content array */
    // This was allocated separately in readerCreate()
    free(readerPointer->content);

    /* ▼ STEP 2: Free the Reader structure itself */
    // This frees the main structure containing all metadata
    free(readerPointer);

    /* ▼ Memory has been returned to the system */
    return MPLUSPLUS_TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if the Reader buffer is at capacity
*          Used to determine if we need to grow before adding more characters
*
* How it works:
* Simply returns the value of the isFull flag
*
* Parameters:
*   readerPointer = pointer to the Reader to check
*
* Return value:
*   MPLUSPLUS_TRUE = buffer is full
*   MPLUSPLUS_FALSE = buffer has space available or error
*************************************************************
*/
mplusplus_boln readerIsFull(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        // NULL Reader is considered "not full" (error condition)
        return MPLUSPLUS_FALSE;
    }

    /* ▼ Return the full status flag */
    return readerPointer->flags.isFull;
}

/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if the Reader buffer contains no characters
*          Used to determine if there's any content to process
*
* How it works:
* Simply returns the value of the isEmpty flag
*
* Parameters:
*   readerPointer = pointer to the Reader to check
*
* Return value:
*   MPLUSPLUS_TRUE = buffer is empty
*   MPLUSPLUS_FALSE = buffer contains data or error
*************************************************************
*/
mplusplus_boln readerIsEmpty(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        // NULL Reader is considered "not empty" (error condition)
        return MPLUSPLUS_FALSE;
    }

    /* ▼ Return the empty status flag */
    return readerPointer->flags.isEmpty;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Sets a "bookmark" at a specific position in the buffer
*          This allows returning to this position later with readerRestore()
*
* How it works:
* 1. Validates the Reader pointer and mark position
* 2. Sets the mark field to the specified position
*
* Use case: During parsing, when you need to try reading ahead
*           and potentially backtrack if the parse fails
*
* Example:
*   Input: "123abc"
*   Position 0: Set mark here
*   Read: "123" - looks like a number
*   Read: "a" - not a digit, this isn't a number!
*   Restore to mark - back to position 0
*
* Parameters:
*   readerPointer = pointer to the Reader
*   mark = position to mark (must be valid position in buffer)
*
* Return value:
*   MPLUSPLUS_TRUE = mark set successfully
*   MPLUSPLUS_FALSE = mark setting failed (invalid position)
*************************************************************
*/
mplusplus_boln readerSetMark(BufferPointer const readerPointer, mplusplus_intg mark) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return MPLUSPLUS_FALSE;
    }

    /* ▼ DEFENSIVE PROGRAMMING: Validate mark position */
    // Mark must be between 0 and wrte position (inclusive)
    // We can't mark beyond what we've written
    if (mark < 0 || mark > readerPointer->position.wrte) {
        // Invalid mark position
        readerPointer->numReaderErrors++;  // Count as an error
        return MPLUSPLUS_FALSE;
    }

    /* ▼ Set the mark to the specified position */
    readerPointer->position.mark = mark;
    return MPLUSPLUS_TRUE;
}

/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints all characters in the buffer to standard output
*          This function demonstrates how to read through the entire buffer
*
* How it works:
* 1. Validates the Reader pointer
* 2. Resets to beginning of buffer
* 3. Uses readerGetChar() to read each character
* 4. Prints each character until end is reached
*
* Important: This function modifies the read position!
*           After calling this, the read position will be at the end
*
* Parameters:
*   readerPointer = pointer to the Reader to print
*
* Return value:
*   Number of characters printed (0 if empty or error)
*************************************************************
*/
mplusplus_intg readerPrint(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return 0;  // Cannot print from NULL Reader
    }

    /* ▼ Check if buffer has any content */
    if (readerPointer->flags.isEmpty) {
        return 0;  // Nothing to print
    }

    /* ▼ Reset to beginning of buffer for printing */
    // This ensures we print from the start, regardless of current read position
    readerRecover(readerPointer);

    /* ▼ Read and print each character */
    mplusplus_intg count = 0;  // Count characters printed
    while (1) {  // Infinite loop - we'll break when we reach the end
        // Get the next character from the buffer
        mplusplus_char c = readerGetChar(readerPointer);

        // Check if we've reached the end of the buffer
        if (readerPointer->flags.isRead) {
            break;  // Exit the loop - no more characters
        }

        // Print the character to standard output
        putchar(c);
        count++;  // Increment our counter
    }

    /* ▼ Return the number of characters we printed */
    return count;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads an entire file into the Reader buffer
*          This is a high-level function that handles file reading and decryption
*
* How it works:
* 1. Validates parameters
* 2. Opens the file to verify it exists
* 3. Uses vigenereMem() to decrypt the file content
* 4. Adds each decrypted character to the buffer
* 5. Cleans up and returns character count
*
* Why decryption? The input files are encrypted using the Vigenère cipher
* from Assignment 1. We must decrypt them before processing.
*
* Parameters:
*   readerPointer = pointer to the Reader to load into
*   fileName = name of the file to load
*
* Return value:
*   Number of characters loaded (positive number)
*   READER_ERROR (-1) if loading failed
*
* File Processing Flow:
*   [Encrypted File] → vigenereMem() → [Decrypted String] → readerAddChar() → [Buffer]
*************************************************************
*/
mplusplus_intg readerLoad(BufferPointer const readerPointer, mplusplus_strg fileName) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate parameters */
    if (readerPointer == NULL || fileName == NULL) {
        return READER_ERROR;  // Invalid parameters
    }

    /* ▼ STEP 1: Try to open the file to verify it exists */
    FILE* fp = fopen(fileName, "r");  // Open for reading
    if (fp == NULL) {
        // File doesn't exist or can't be opened
        readerPointer->numReaderErrors++;  // Count as an error
        return READER_ERROR;
    }

    /* ▼ STEP 2: Decrypt the file content using Vigenère cipher */
    // vigenereMem() reads the file, decrypts it, and returns decrypted string
    // STR_LANGNAME is our decryption key ("mplusplus")
    // DECYPHER constant indicates we want to decrypt (not encrypt)
    mplusplus_strg decoded = vigenereMem(fileName, STR_LANGNAME, DECYPHER);

    if (decoded == NULL) {
        // Decryption failed
        fclose(fp);  // Clean up the file handle
        readerPointer->numReaderErrors++;  // Count as an error
        return READER_ERROR;
    }

    /* ▼ STEP 3: Close the file (we have the decrypted content now) */
    fclose(fp);

    /* ▼ STEP 4: Add each decrypted character to the Reader buffer */
    mplusplus_intg count = 0;  // Count characters successfully added

    // Loop through each character in the decrypted string
    for (mplusplus_intg i = 0; decoded[i] != '\0'; i++) {
        // Try to add this character to the buffer
        if (readerAddChar(readerPointer, decoded[i]) == NULL) {
            /* Character addition failed */
            // Note: The original spec mentions ungetc() here, but since we've
            // already closed the file, we'll skip it and just clean up
            free(decoded);  // Free the decrypted string
            return READER_ERROR;  // Return failure
        }
        count++;  // Successfully added one more character
    }

    /* ▼ STEP 5: Clean up and return success */
    free(decoded);  // Free the decrypted string (we've copied it to buffer)
    return count;   // Return number of characters loaded
}

/*
***********************************************************
* Function name: readerRecover
* Purpose: Resets the read position to the beginning of the buffer
*          This allows re-reading the buffer content from the start
*
* How it works:
* 1. Validates the Reader pointer
* 2. Sets read position to 0
* 3. Sets mark position to 0
* 4. Clears the end-of-read flag
*
* Use case: After processing the buffer, if you want to process it again
*
* Parameters:
*   readerPointer = pointer to the Reader to recover
*
* Return value:
*   MPLUSPLUS_TRUE = recovery succeeded
*   MPLUSPLUS_FALSE = recovery failed (NULL pointer)
*************************************************************
*/
mplusplus_boln readerRecover(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return MPLUSPLUS_FALSE;
    }

    /* ▼ Reset positions to beginning of buffer */
    readerPointer->position.read = 0;  // Start reading from beginning
    readerPointer->position.mark = 0;  // Reset mark to beginning

    /* ▼ Clear the end-of-read flag */
    readerPointer->flags.isRead = MPLUSPLUS_FALSE;  // Not at end anymore

    return MPLUSPLUS_TRUE;
}

/*
***********************************************************
* Function name: readerRetract
* Purpose: Moves the read position back by one character
*          This "un-reads" the last character that was read
*
* How it works:
* 1. Validates the Reader pointer
* 2. Checks if we can move back (read position > 0)
* 3. Decrements the read position
* 4. Clears the end-of-read flag
*
* Use case: When parsing, if you read one character too many
*           and need to put it back
*
* Example:
*   Read 'a', 'b', 'c' → read position = 3
*   Retract once → read position = 2
*   Next readerGetChar() will return 'c' again
*
* Parameters:
*   readerPointer = pointer to the Reader
*
* Return value:
*   MPLUSPLUS_TRUE = retraction succeeded
*   MPLUSPLUS_FALSE = retraction failed (already at beginning or error)
*************************************************************
*/
mplusplus_boln readerRetract(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return MPLUSPLUS_FALSE;
    }

    /* ▼ Check if we can retract (not already at beginning) */
    if (readerPointer->position.read > 0) {
        /* We can move back */
        readerPointer->position.read--;  // Move read position back by 1
        readerPointer->flags.isRead = MPLUSPLUS_FALSE;  // Clear end flag
        return MPLUSPLUS_TRUE;
    }

    /* ▼ Cannot retract - already at beginning */
    readerPointer->numReaderErrors++;  // Count as an error
    return MPLUSPLUS_FALSE;
}

/*
***********************************************************
* Function name: readerRestore
* Purpose: Sets the read position to the previously marked position
*          This implements the "bookmark and return" functionality
*
* How it works:
* 1. Validates the Reader pointer
* 2. Sets read position to the marked position
* 3. Clears the end-of-read flag
*
* Use case: During parsing lookahead operations
*
* Example:
*   Content: "123abc"
*   Mark at position 0
*   Read: '1', '2', '3', 'a' → read position = 4
*   Restore → read position = 0
*   Next read will return '1' again
*
* Parameters:
*   readerPointer = pointer to the Reader
*
* Return value:
*   MPLUSPLUS_TRUE = restore succeeded
*   MPLUSPLUS_FALSE = restore failed (NULL pointer)
*************************************************************
*/
mplusplus_boln readerRestore(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return MPLUSPLUS_FALSE;
    }

    /* ▼ Restore read position to the marked position */
    readerPointer->position.read = readerPointer->position.mark;

    /* ▼ Clear the end-of-read flag since we're repositioning */
    readerPointer->flags.isRead = MPLUSPLUS_FALSE;

    return MPLUSPLUS_TRUE;
}

/*
***********************************************************
* Function name: readerGetChar
* Purpose: Reads and returns the next character from the buffer
*          This is the primary function for sequential character access
*
* How it works:
* 1. Validates the Reader pointer
* 2. Checks if we've reached the end of written content
* 3. If at end, sets the end flag and returns terminator
* 4. If not at end, returns character and advances read position
*
* End-of-buffer detection:
*   When read position equals write position, we've read everything
*
* State changes:
*   - Advances read position by 1
*   - May set the isRead flag if reaching end
*
* Parameters:
*   readerPointer = pointer to the Reader
*
* Return value:
*   The character at current read position
*   READER_TERMINATOR ('\0') if at end or error
*
* Reading Process:
*   Content: "Hello"
*   Position: 01234
*
*   Call 1: read=0, returns 'H', read becomes 1
*   Call 2: read=1, returns 'e', read becomes 2
*   Call 3: read=2, returns 'l', read becomes 3
*   Call 4: read=3, returns 'l', read becomes 4
*   Call 5: read=4, returns 'o', read becomes 5
*   Call 6: read=5, read==wrte, returns '\0', sets isRead flag
*************************************************************
*/
mplusplus_char readerGetChar(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return READER_TERMINATOR;  // Return end marker for NULL Reader
    }

    /* ▼ Check if we've reached the end of the buffer content */
    // When read position equals write position, we've read all available data
    if (readerPointer->position.read >= readerPointer->position.wrte) {
        /* ▼ We're at the end - no more characters to read */
        readerPointer->flags.isRead = MPLUSPLUS_TRUE;  // Set end-of-buffer flag
        return READER_TERMINATOR;  // Return terminator character ('\0')
    }

    /* ▼ We have a character to return */
    // Get character at current read position
    mplusplus_char ch = readerPointer->content[readerPointer->position.read];

    /* ▼ Advance read position for next call */
    readerPointer->position.read++;

    /* ▼ Clear end flag since we're still reading valid content */
    readerPointer->flags.isRead = MPLUSPLUS_FALSE;

    /* ▼ Return the character we just read */
    return ch;
}

/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns a pointer to a specific position in the buffer
*          This allows direct access to buffer content at any valid position
*
* How it works:
* 1. Validates the Reader pointer and position
* 2. Checks if position is within valid range
* 3. Returns pointer to the character at that position
*
* Important: This returns a pointer INTO the buffer, not a copy
*           The returned pointer becomes invalid if the buffer is reallocated
*
* Use case: When you need to examine content at a specific position
*           without changing the current read position
*
* Parameters:
*   readerPointer = pointer to the Reader
*   pos = position to get pointer for (0 to wrte-1)
*
* Return value:
*   Pointer to character at specified position
*   NULL if position is invalid or error
*
* Memory Safety:
*   The returned pointer is only valid as long as:
*   - The Reader exists (not freed)
*   - The buffer isn't reallocated (no new characters added that cause growth)
*   - The position remains within written content
*************************************************************
*/
mplusplus_strg readerGetContent(BufferPointer const readerPointer, mplusplus_intg pos) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return NULL;
    }

    /* ▼ DEFENSIVE PROGRAMMING: Validate position parameter */
    // Position must be between 0 and wrte-1 (inclusive)
    // We can't access beyond what we've written
    if (pos < 0 || pos >= readerPointer->position.wrte) {
        /* Invalid position requested */
        readerPointer->numReaderErrors++;  // Count as an error
        return NULL;
    }

    /* ▼ Return pointer to character at specified position */
    // This uses pointer arithmetic: content + pos
    return readerPointer->content + pos;
}

/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the current read position
*          This shows where the next readerGetChar() will read from
*
* Parameters:
*   readerPointer = pointer to the Reader
*
* Return value:
*   Current read position (0 to wrte)
*   -1 if error (NULL pointer)
*************************************************************
*/
mplusplus_intg readerGetPosRead(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return -1;  // Error indicator
    }

    /* ▼ Return current read position */
    return readerPointer->position.read;
}

/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the current write position
*          This shows how many characters are in the buffer
*
* Parameters:
*   readerPointer = pointer to the Reader
*
* Return value:
*   Current write position (number of characters in buffer)
*   -1 if error (NULL pointer)
*************************************************************
*/
mplusplus_intg readerGetPosWrte(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return -1;  // Error indicator
    }

    /* ▼ Return current write position (also the character count) */
    return readerPointer->position.wrte;
}

/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the current mark position
*          This shows where readerRestore() will return to
*
* Parameters:
*   readerPointer = pointer to the Reader
*
* Return value:
*   Current mark position
*   -1 if error (NULL pointer)
*************************************************************
*/
mplusplus_intg readerGetPosMark(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return -1;  // Error indicator
    }

    /* ▼ Return current mark position */
    return readerPointer->position.mark;
}

/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the total allocated size of the buffer
*          This shows the maximum capacity before reallocation is needed
*
* Parameters:
*   readerPointer = pointer to the Reader
*
* Return value:
*   Total allocated size in characters
*   -1 if error (NULL pointer)
*************************************************************
*/
mplusplus_intg readerGetSize(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return -1;  // Error indicator
    }

    /* ▼ Return total allocated size */
    return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the current status flags as a single byte
*          This encodes all boolean flags into one value for display
*
* How it works:
* 1. Validates the Reader pointer
* 2. Builds a byte with each flag as a bit:
*    - Bit 3 (0x08): isEmpty flag
*    - Bit 2 (0x04): isFull flag
*    - Bit 1 (0x02): isRead flag
*    - Bit 0 (0x01): isMoved flag
*
* Example flag combinations:
*   0x08 = 00001000 = empty only
*   0x0C = 00001100 = empty and full (impossible, but shows format)
*   0x00 = 00000000 = no flags set
*
* Parameters:
*   readerPointer = pointer to the Reader
*
* Return value:
*   Byte containing encoded flags
*   0 if error (NULL pointer)
*************************************************************
*/
mplusplus_byte readerGetFlags(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return 0;  // No flags set for NULL Reader
    }

    /* ▼ Build flag byte by combining individual flags */
    mplusplus_byte f = 0;  // Start with no flags set

    // Set bit 3 if buffer is empty
    if (readerPointer->flags.isEmpty) f |= 0x08;   // Binary: 00001000

    // Set bit 2 if buffer is full
    if (readerPointer->flags.isFull) f |= 0x04;    // Binary: 00000100

    // Set bit 1 if we've reached end during reading
    if (readerPointer->flags.isRead) f |= 0x02;    // Binary: 00000010

    // Set bit 0 if memory was moved during reallocation
    if (readerPointer->flags.isMoved) f |= 0x01;   // Binary: 00000001

    /* ▼ Return the combined flag byte */
    return f;
}

/*
***********************************************************
* Function name: readerPrintStat
* Purpose: Prints character frequency statistics from the histogram
*          This shows how many times each character appears in the buffer
*
* How it works:
* 1. Validates the Reader pointer
* 2. Loops through all possible ASCII characters (0-127)
* 3. For each character that appears in the buffer (count > 0):
*    - If printable, shows the character itself
*    - If non-printable, shows the ASCII code number
*    - Shows the count of occurrences
*
* Output format examples:
*   B[H]=1, B[e]=2, B[l]=3, B[o]=1, B[ ]=1, B[10]=1,
*
*   Where:
*   - B[H]=1 means character 'H' appears 1 time
*   - B[ ]=1 means space character appears 1 time
*   - B[10]=1 means ASCII code 10 (newline) appears 1 time
*
* Use case: Analyzing the content of the buffer for debugging
*           or compiler optimization
*
* Parameters:
*   readerPointer = pointer to the Reader
*
* Return value: None (void function)
*************************************************************
*/
mplusplus_void readerPrintStat(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return;  // Cannot print stats for NULL Reader
    }

    /* ▼ Loop through all possible ASCII characters */
    for (mplusplus_intg i = 0; i < NCHAR; i++) {
        /* ▼ Only print characters that actually appear in the buffer */
        if (readerPointer->histogram[i] > 0) {
            /* ▼ Check if character is printable (visible) */
            if (isprint(i)) {
                /* Character is printable - show the actual character */
                printf("B[%c]=%d, ", (mplusplus_char)i, readerPointer->histogram[i]);
            }
            else {
                /* Character is non-printable - show the ASCII code number */
                printf("B[%d]=%d, ", i, readerPointer->histogram[i]);
            }
        }
    }
    /* ▼ End with a newline for better formatting */
    printf("\n");
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors encountered during Reader operations
*          This helps with debugging and error reporting
*
* Error counting:
*   Errors are incremented when:
*   - Invalid characters are added (outside ASCII 0-127)
*   - Memory allocation fails
*   - Invalid parameters are passed to functions
*   - Boundary conditions are violated
*
* Parameters:
*   readerPointer = pointer to the Reader
*
* Return value:
*   Number of errors encountered
*   -1 if error (NULL pointer)
*************************************************************
*/
mplusplus_intg readerNumErrors(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return -1;  // Error indicator
    }

    /* ▼ Return the error count */
    return readerPointer->numReaderErrors;
}

/*
***********************************************************
* Function name: readerChecksum
* Purpose: Calculates and updates the checksum of all buffer content
*          This provides a way to verify data integrity
*
* How it works:
* 1. Validates the Reader pointer
* 2. Sums the ASCII values of all characters in the buffer
* 3. Keeps only the low 8 bits to prevent overflow
* 4. Stores the result in the checkSum field
*
* Checksum calculation:
*   For content "ABC":
*   - 'A' = 65, 'B' = 66, 'C' = 67
*   - Sum = 65 + 66 + 67 = 198
*   - Checksum = 198 & 0xFF = 198 (fits in 8 bits)
*
* Use case: Detecting if buffer content has been corrupted
*           Two identical buffers should have identical checksums
*
* Parameters:
*   readerPointer = pointer to the Reader
*
* Return value:
*   The calculated checksum value (0-255)
*   READER_ERROR (-1) if calculation failed
*************************************************************
*/
mplusplus_intg readerChecksum(BufferPointer const readerPointer) {
    /* ▼ DEFENSIVE PROGRAMMING: Validate Reader pointer */
    if (readerPointer == NULL) {
        return READER_ERROR;
    }

    /* ▼ Calculate checksum by summing all character values */
    unsigned int sum = 0;  // Use unsigned to prevent overflow issues

    // Loop through all characters currently in the buffer
    for (mplusplus_intg i = 0; i < readerPointer->position.wrte; i++) {
        // Add each character's ASCII value to the sum
        // Cast to unsigned char to handle negative chars correctly
        sum += (unsigned char)readerPointer->content[i];
    }

    /* ▼ Keep only low 8 bits and store in Reader structure */
    // & 0xFF masks off all but the lowest 8 bits
    readerPointer->checkSum = (mplusplus_byte)(sum & 0xFF);

    /* ▼ Return the checksum value */
    return (mplusplus_intg)readerPointer->checkSum;
}