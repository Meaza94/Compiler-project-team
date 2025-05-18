mplusplus Compiler Overview MPlusPlus is a compiler project developed as part of the Compilers course (CST 8152) at Algonquin College. The project implements a custom programming language called "mplusplus" through a series of modular components that together form a complete compiler system. Project Structure The compiler is built in five progressive components:

Coder Current - Implements file encryption/decryption using the Vigenère cipher Reader - Will handle file reading and buffer management Scanner - Will perform lexical analysis (tokenization) Parser - Will perform syntax analysis and build parse trees Writer - Will generate output code

Current Implementation: Coder Component The first component of the project is the Coder, which provides file encryption and decryption capabilities using the Vigenère cipher algorithm with "MPlusPlus" as the encryption key. Features

File-based encryption/decryption Memory-based encryption/decryption Support for all visible ASCII characters (32-126) Command-line interface Robust error handling

How It Works The Vigenère cipher implemented in this project:

Uses "MPlusPlus" as the encryption key Encrypts each character by shifting it based on the corresponding character in the key Only modifies visible ASCII characters (32-126), leaving other characters unchanged Wraps around the ASCII range to ensure all encrypted characters remain readable

Building the Project The project uses CMake for building: bash# Create build directory mkdir build cd build

Generate build files
cmake ..

Build the project
cmake --build . Usage To use the MPlusPlus Coder: bash# To encrypt a file MPlusPlus.exe 1 1 <input_file> <output_file>

To decrypt a file
MPlusPlus.exe 1 0 <input_file> <output_file> Where:

First argument (1): Selects the Coder component Second argument (1 or 0): 1 for encryption, 0 for decryption Third argument: Input file path Fourth argument: Output file path

Example bash# Create a test file echo "Hello, this is a test!" > test.txt

Encrypt the file
MPlusPlus.exe 1 1 test.txt encrypted.txt

Decrypt the file
MPlusPlus.exe 1 0 encrypted.txt decrypted.txt Future Development This project will continue to evolve through the implementation of the remaining compiler components. The next phase will be the Reader component, which will handle file reading and buffer management for parsing source code in the MPlusPlus language. Technical Details

Written in C Compatible with MS Visual Studio 2022 Uses defensive programming techniques Implements modular design for extensibility Custom data types for language-specific operations

Course Information

Course: CST 8152 – Compilers Professor: Paulo Sousa Algonquin College, Summer 2025
