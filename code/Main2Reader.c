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
************************************************************
* File name: Main2Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file contains the main driver program for testing
*          the Reader/Buffer implementation. It demonstrates how
*          to create, load, and display a Reader buffer.
*
*          This is the entry point when the user runs the compiler
*          with option "2" (Reader mode).
*
* Function list:
*   - main2Reader(): Main entry point for Reader testing
*   - startReader(): Core Reader demonstration function
*   - displayBuffer(): Shows all Reader properties and content
*   - getFileSize(): Utility to get file size for diagnostics
*   - isNumber(): Utility to check if string represents a number
*************************************************************/

/*
 *.............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: Preprocessor directives
 * The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard string library functions defined in string.h.
 * The define directive does not have any effect on other compiler projects
 * (Gcc, VSCode, Codeblocks, etc.).
 *
 * PROGRAM FLOW OVERVIEW:
 * ======================
 * 1. main2Reader() - Parse command line arguments
 * 2. startReader() - Create and test Reader
 * 3. readerLoad() - Load file content into Reader
 * 4. displayBuffer() - Show Reader statistics and content
 * 5. Cleanup and exit
 *.............................................................................
 */

 // Suppress Visual Studio warnings about "unsafe" functions
#define _CRT_SECURE_NO_WARNINGS 

// Include standard C library headers
#include <stdio.h>   // For printf, FILE operations, input/output functions
#include <stdlib.h>  // For malloc, free, exit, EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>  // For strlen, strchr string manipulation functions
#include <stdarg.h>  // For variable argument lists (used in errorPrint)
#include <ctype.h>   // For character classification (isdigit, isprint)

// Include our custom header files
#ifndef COMPILERS_H_
#include "Compilers.h"    // Contains our language-specific data types
#endif

#ifndef CODER_H_
#include "Step1Coder.h"   // Contains encryption/decryption functions
#endif

#ifndef READER_H_
#include "Step2Reader.h"  // Contains Reader structure and function declarations
#endif

 /* Check for ANSI C compliancy - used for compatibility across different compilers */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/*
 * TODO .......................................................................
 * Basically, change all datatypes to your language definitions
 * (see "Compilers.h")
 *
 * All the mplusplus_* types are defined in Compilers.h:
 * - mplusplus_intg  = int (integer numbers)
 * - mplusplus_strg  = char* (strings/text)
 * - mplusplus_void  = void (no return value)
 * - mplusplus_long  = long (large integer numbers)
 * - mplusplus_boln  = unsigned char (boolean true/false)
 * - mplusplus_char  = char (single characters)
 */

 /*
  * -------------------------------------------------------------
  *  Function declarations (prototypes)
  *  These tell the compiler what functions exist before we define them
  * -------------------------------------------------------------
  */
mplusplus_void displayBuffer(BufferPointer ptr_Buffer);
mplusplus_long getFileSize(mplusplus_strg fname);
mplusplus_intg isNumber(const mplusplus_strg ns);
mplusplus_void startReader(mplusplus_strg, mplusplus_strg);

/*
************************************************************
* Function name: main2Reader
* Purpose: Main entry point for Reader testing mode
*          This function is called when user runs: program.exe 2 filename.txt
*
* How it works:
* 1. Validates command line arguments
* 2. Extracts program name and input filename
* 3. Calls startReader() to perform the actual Reader testing
*
* Command Line Format:
*   program.exe 2 input_file.txt
*   argv[0] = "program.exe"     (program name)
*   argv[1] = "2"               (option - Reader mode)
*   argv[2] = "input_file.txt"  (input filename)
*
* Parameters:
*   argc = argument count (number of command line arguments)
*   argv = argument vector (array of command line argument strings)
*
* Return value:
*   EXIT_SUCCESS (0) = program completed successfully
*   EXIT_FAILURE (1) = program failed due to errors
*
* Error conditions:
*   - Not enough command line arguments (argc <= 2)
*   - Missing input filename
*
* Example usage:
*   Compiler.exe 2 hello.mplusplus
*   This will test the Reader with the file "hello.mplusplus"
************************************************************
*/
mplusplus_intg main2Reader(mplusplus_intg argc, mplusplus_strg* argv) {

    /* ▼ STEP 1: Extract command line arguments into meaningful variables */

    // Get the program name from argv[0] (first argument is always program name)
    mplusplus_strg program = argv[0];

    // Get the input filename from argv[2] (third argument)
    // Note: argv[1] is the option "2" which brought us to this function
    mplusplus_strg input = argv[2];

    // Variable for potential future use (currently unused)
    mplusplus_intg size = 0;

    /* ▼ COMMENTED OUT: Alternative approach for creating output files */
    // The following lines show how to create an output filename by appending a suffix
    // This is not currently used but shows the pattern for file processing
    ///mplusplus_strg output = (mplusplus_strg)malloc(strlen(input) + strlen(SUFFIX_READER) + 1);
    ///if (output == NULL) {  
    ///   errorPrint("Memory allocation failed for 'output'.");  
    ///   exit(EXIT_FAILURE);  
    ///}
    ///strcpy(output, input);                    // Copy original filename
    ///strcat(output, SUFFIX_READER);            // Append suffix (like ".out")

    /* ▼ STEP 2: DEFENSIVE PROGRAMMING - Validate command line arguments */

    // Check if we have enough arguments
    // We need at least 3: program_name, option, input_file
    if (argc <= 2) {
        /* ▼ Not enough arguments - print error information and usage */

        // Print timestamp information for debugging
        errorPrint("\nDate: %s  Time: %s", __DATE__, __TIME__);

        // Print where the error occurred (line number and file name)
        errorPrint("\nRuntime error at line %d in file %s\n", __LINE__, __FILE__);

        // Print what went wrong
        errorPrint("%s\b\b\b\b%s%s", argv[0], ": ", "Missing parameters.");

        // Print usage instructions
        errorPrint("Usage: <Option=2> <SourceFile>");

        // Exit with failure status
        exit(EXIT_FAILURE);
    }

    /* ▼ COMMENTED OUT: Alternative approach using decryption pipeline */
    // This shows how we could decrypt the file first, then process it
    // Currently we do decryption inside readerLoad() instead
    ///decypher(input, output, STR_LANGNAME);    // Decrypt input to output
    ///startReader(program, output);             // Process decrypted file

    /* ▼ STEP 3: Start the Reader testing process */
    // Pass program name and input filename to the core testing function
    startReader(program, input);

    /* ▼ STEP 4: Return success status */
    return (EXIT_SUCCESS);
}

/*
************************************************************
* Function name: startReader
* Purpose: Core Reader testing function - demonstrates all Reader capabilities
*          This is where the actual Reader testing happens
*
* How it works:
* 1. Creates a new Reader with default size
* 2. Loads file content into the Reader (with decryption)
* 3. Calculates data integrity checksum
* 4. Handles any loading errors
* 5. Adds terminator character
* 6. Displays all Reader properties and content
* 7. Cleans up memory
*
* This function demonstrates the complete Reader lifecycle:
* Create → Load → Process → Display → Cleanup
*
* Parameters:
*   program = name of the program (for error messages)
*   input = filename to load into the Reader
*
* Return value: None (void function)
*
* Reader Testing Process:
*   [File] → readerLoad() → [Reader Buffer] → displayBuffer() → [Output]
*      ↓
*   Decryption happens automatically inside readerLoad()
************************************************************
*/
mplusplus_void startReader(mplusplus_strg program, mplusplus_strg input) {

    /* ▼ STEP 1: Declare variables for Reader testing */

    // Pointer to our Reader structure - this will hold all our data
    BufferPointer bufferp;

    // Variable to track how many characters were loaded from the file
    mplusplus_intg loadSize = 0;

    /* ▼ STEP 2: Create a new Reader with default size */

    // Create Reader with READER_DEFAULT_SIZE (250 characters initially)
    // This allocates memory for both the Reader structure and character content
    bufferp = readerCreate(READER_DEFAULT_SIZE);

    /* ▼ DEFENSIVE PROGRAMMING: Check if Reader creation succeeded */
    if (bufferp == NULL) {
        /* Reader creation failed - probably out of memory */

        // Print error message with program name and context
        errorPrint("%s%s", program, ": Cannot allocate buffer - Use: buffer <input>.");
        errorPrint("Filename: %s \n", input);

        // Exit with error status - can't continue without a Reader
        exit(1);
    }

    /* ▼ STEP 3: Load the input file into the Reader */

    // Inform user that file loading is starting
    printf("Reading file %s ....Please wait\n", input);

    // Load file content into Reader
    // This function:
    // 1. Opens the file
    // 2. Decrypts the content using Vigenère cipher
    // 3. Adds each character to the Reader buffer
    // 4. Returns the number of characters loaded
    loadSize = readerLoad(bufferp, input);

    /* ▼ STEP 4: Calculate data integrity checksum */

    // Calculate checksum of all loaded data for integrity verification
    // This helps detect if data was corrupted during loading or processing
    readerChecksum(bufferp);

    /* ▼ STEP 5: Handle potential loading errors */

    // Check if file loading failed completely
    if (loadSize == READER_ERROR) {
        /* File loading failed - print diagnostic information */

        printf("The input file %s %s\n", input, "has not been completely loaded.");

        // Show how much of the Reader buffer is actually being used
        printf("Current size of buffer: %d.\n", readerGetSize(bufferp));

        // Show the actual file size for comparison
        printf("Input file size: %ld\n", getFileSize(input));
    }

    /* ▼ STEP 6: Add terminator character to mark end of content */

    // Check if we successfully loaded some content (not error, not empty)
    if ((loadSize != READER_ERROR) && (loadSize != 0)) {
        /* We have content - add terminator character */

        // Add null terminator ('\0') to mark the end of valid content
        // This is important for string processing and prevents reading beyond valid data
        if (!readerAddChar(bufferp, READER_TERMINATOR)) {
            /* Adding terminator failed - this is unusual but handle it */
            errorPrint("%s%s%s", program, ": ", "Error in compacting buffer.");
        }
    }

    /* ▼ STEP 7: Display all Reader information */

    // This function shows:
    // - Reader size and usage statistics
    // - Character frequency histogram
    // - Error count
    // - Flag status
    // - Complete buffer content
    displayBuffer(bufferp);

    /* ▼ STEP 8: Cleanup - free all allocated memory */

    // Free all memory allocated for the Reader
    // This prevents memory leaks
    readerFree(bufferp);

    // Set pointer to NULL to prevent accidental reuse
    // (This is a good practice but not strictly necessary since variable goes out of scope)
    bufferp = NULL;
}

/*
************************************************************
* Function name: getFileSize
* Purpose: Utility function to determine the size of a file in bytes
*          Used for diagnostics and error reporting
*
* How it works:
* 1. Opens the file for reading
* 2. Seeks to the end of the file
* 3. Uses ftell() to get current position (which is the file size)
* 4. Closes the file and returns the size
*
* This is useful for:
* - Comparing expected vs actual loaded characters
* - Debugging file loading problems
* - Memory allocation planning
*
* Parameters:
*   fname = filename to check
*
* Return value:
*   File size in bytes (positive number)
*   0 if file cannot be opened or other error
*
* File Size Detection Process:
*   [Open File] → [Seek to End] → [Get Position] → [Close] → [Return Size]
************************************************************
*/
mplusplus_long getFileSize(mplusplus_strg fname) {

    /* ▼ Declare variables for file operations */
    FILE* input;           // File pointer for reading
    mplusplus_long flength; // Variable to store file length

    /* ▼ STEP 1: Open the file for reading */
    input = fopen(fname, "r");  // Open in text mode for reading

    /* ▼ DEFENSIVE PROGRAMMING: Check if file opened successfully */
    if (input == NULL) {
        // File doesn't exist, no permission, or other error
        errorPrint("%s%s", "Cannot open file: ", fname);
        return 0;  // Return 0 to indicate error
    }

    /* ▼ STEP 2: Move file pointer to the end */
    // fseek(file, offset, whence)
    // SEEK_END means "from end of file"
    // 0L means "0 bytes offset"
    // So this moves to exactly the end of the file
    fseek(input, 0L, SEEK_END);

    /* ▼ STEP 3: Get current file position (which is the file size) */
    // ftell() returns current position in bytes from beginning
    // Since we're at the end, this gives us the total file size
    flength = ftell(input);

    /* ▼ STEP 4: Close the file */
    fclose(input);

    /* ▼ STEP 5: Return the file size */
    return flength;
}

/*
************************************************************
* Function name: isNumber
* Purpose: Utility function to check if a string represents a valid number
*          Tests if all characters in the string are digits
*
* How it works:
* 1. Validates the input string pointer
* 2. Loops through each character in the string
* 3. Uses isdigit() to check if each character is a digit (0-9)
* 4. Returns false if any non-digit is found
* 5. Returns true if all characters are digits
*
* Use cases:
* - Validating user input
* - Parsing configuration files
* - Input sanitization
*
* Parameters:
*   ns = null-terminated string to test
*
* Return value:
*   1 (true) = string represents a valid number (all digits)
*   0 (false) = string contains non-digit characters or is invalid
*
* Examples:
*   isNumber("123")   → returns 1 (true)
*   isNumber("12a3")  → returns 0 (false, contains 'a')
*   isNumber("")      → returns 1 (true, empty string is considered valid)
*   isNumber(NULL)    → returns 0 (false, invalid pointer)
************************************************************
*/
mplusplus_intg isNumber(const mplusplus_strg ns) {

    /* ▼ Declare variables for string processing */
    mplusplus_char c;  // Current character being examined
    mplusplus_intg i = 0;  // Index for looping through string

    /* ▼ DEFENSIVE PROGRAMMING: Validate input pointer */
    if (ns == NULL) {
        return 0;  // NULL string is not a number
    }

    /* ▼ Loop through each character in the string */
    // This loop continues until we hit the null terminator '\0'
    // FIXED: Changed from == 0 to != '\0' to fix infinite loop bug
    while ((c = ns[i++]) != '\0') {

        /* ▼ Check if current character is a digit */
        // isdigit() returns non-zero for '0'-'9', zero for anything else
        if (!isdigit(c)) {
            // Found a non-digit character
            return 0;  // String is not a pure number
        }
    }

    /* ▼ All characters were digits */
    return 1;  // String represents a valid number
}

/*
************************************************************
* Function name: displayBuffer
* Purpose: Comprehensive display function showing all Reader properties
*          This is the main output function that demonstrates what the Reader contains
*
* How it works:
* 1. Displays buffer size and usage statistics
* 2. Shows the first character in the buffer
* 3. Displays status flags as hexadecimal
* 4. Prints character frequency statistics
* 5. Shows error count
* 6. Displays complete buffer content
*
* This function demonstrates:
* - How to query Reader properties
* - How to access Reader content
* - How to interpret Reader statistics
*
* Output format:
*   Printing buffer parameters:
*
*   The size of the buffer is: 250
*   The current size of the buffer is: 12
*   The first symbol in the buffer is: H
*   The value of the flags field is: 28
*   Reader statistics:
*   B[H]=1, B[e]=1, B[l]=3, B[o]=2, ...
*   Number of errors: 0
*
*   Printing buffer contents:
*
*   Hello World!
*
* Parameters:
*   ptr_Buffer = pointer to the Reader to display
*
* Return value: None (void function)
************************************************************
*/
mplusplus_void displayBuffer(BufferPointer ptr_Buffer) {

    /* ▼ SECTION 1: Display buffer parameters and statistics */
    printf("\nPrinting buffer parameters:\n\n");

    /* ▼ Show total allocated buffer size */
    // This is how much memory was allocated (may be larger than content)
    printf("The size of the buffer is:  %d\n",
        readerGetSize(ptr_Buffer));

    /* ▼ Show current content size (number of characters actually stored) */
    // This is how many characters are actually in the buffer
    printf("The current size of the buffer is:  %d\n",
        readerGetPosWrte(ptr_Buffer));

    /* ▼ Show the first character in the buffer (if any) */
    // Uses ternary operator: condition ? value_if_true : value_if_false
    // If write position > 0, there's content, so show first character
    // If write position = 0, buffer is empty, so show space character
    printf("The first symbol in the buffer is:  %c\n",
        readerGetPosWrte(ptr_Buffer) ? *readerGetContent(ptr_Buffer, 0) : ' ');

    /* ▼ Show status flags as hexadecimal value */
    // Flags are encoded in a single byte:
    // Bit 3 (0x08): isEmpty
    // Bit 2 (0x04): isFull  
    // Bit 1 (0x02): isRead
    // Bit 0 (0x01): isMoved
    // %02hX formats as 2-digit uppercase hexadecimal with leading zero if needed
    printf("The value of the flags field is: %02hX\n",
        readerGetFlags(ptr_Buffer));

    /* ▼ SECTION 2: Display character frequency statistics */
    printf("%s", "Reader statistics : \n");

    // This function prints character counts like: B[a]=5, B[e]=3, B[ ]=2,
    // It shows how many times each character appears in the buffer
    readerPrintStat(ptr_Buffer);

    /* ▼ Show error count */
    // This shows how many errors occurred during Reader operations
    printf("Number of errors: %d\n",
        readerNumErrors(ptr_Buffer));

    /* ▼ SECTION 3: Display complete buffer content */
    printf("\nPrinting buffer contents:\n\n");

    /* ▼ Reset read position to beginning for content display */
    // This ensures we print from the start regardless of current read position
    readerRecover(ptr_Buffer);

    /* ▼ Print all buffer content */
    // readerPrint() reads through entire buffer and prints each character
    // It returns the number of characters printed
    if (!readerPrint(ptr_Buffer)) {
        // If no characters were printed, buffer is empty
        printf("Empty buffer\n");
    }
}