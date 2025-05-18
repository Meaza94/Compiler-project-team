# MPlusPlus Compiler

**MPlusPlus** is a compiler project developed for the **CST8152 – Compilers** course at **Algonquin College**. The project implements a custom programming language named `mplusplus` through a series of modular components that together form a complete compiler system.

---

## 📁 Project Structure

The compiler is built in five progressive components:

| Component | Description |
|----------|-------------|
| **Coder** (Current) | Implements file encryption/decryption using the Vigenère cipher |
| **Reader** *(Planned)* | Handles file reading and buffer management |
| **Scanner** *(Planned)* | Performs lexical analysis (tokenization) |
| **Parser** *(Planned)* | Performs syntax analysis and builds parse trees |
| **Writer** *(Planned)* | Generates output code |

---

## 🔐 Current Component: Coder (Vigenère Cipher)

The **Coder** component encrypts and decrypts files using the **Vigenère cipher**, with `"MPlusPlus"` as the encryption key.

### ✨ Features
- File-based encryption/decryption
- Memory-based encryption/decryption
- Supports all visible ASCII characters (32–126)
- Command-line interface
- Robust error handling

### ⚙️ How It Works
- Uses `"MPlusPlus"` as the encryption key
- Encrypts each visible ASCII character (32–126) by shifting it based on the corresponding key character
- Non-visible characters remain unchanged
- Wraps around the ASCII range to preserve readability

---

## 🛠️ Building the Project

This project uses **CMake**.

```bash
# Create and move into the build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the project
cmake --build .
