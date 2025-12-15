# The Cipher Vault

A simple, secure password manager built from scratch in C++. It encrypts your passwords using the [**TEA (Tiny Encryption Algorithm)**](https://en.wikipedia.org/wiki/Tiny_Encryption_Algorithm) and stores them in a binary file.

I built this to understand how low-level encryption, bitwise operations, and memory management work without relying on external libraries like OpenSSL.

## How It Works
* **Encryption:** Uses the Tiny Encryption Algorithm (TEA) with 32 rounds of mixing.
* **Padding:** Implements padding to align data to 64-bit blocks.
* **Storage:** Saves data to a custom binary format (`vault.bin`) using direct memory writing.

## Project Structure
* `main.cpp` - Handles the user menu (Add, View, Unlock).
* `Site.cpp/h` - Manages the site data and file loading/saving in binary file.
* `Crypto.cpp/h` - Contains the raw TEA logic, pointer casting, and padding rules.

## How to Build and Run

```bash
Clang++ main.cpp Site.cpp Crypto.cpp -o vault