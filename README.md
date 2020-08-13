# aes-file-encrypt

A basic implementation of AES 128/192/256 in ECB/CBC mode using PKCS7 padding to encrypt files.\
TODO: More Modes, Multithreaded ECB

# Executables (Compiled, Ready To Run)
Whenever there are significant changes to the program, I will create a tag and upload binaries.

Currently, I only compile binaries for Windows and Linux on x86-64.

# Compiling Your Own Binaries

Compiling requires g++ and GNU Make

The program compiles fine with MSVC on Windows and should compile fine elsewhere as well but I have not included any scripts or makefile equivalents to automate this process.

Clone the project and run `make` to generate an executable named `aesf`.

Remove the compiled objects and executable with `make clean`.

Take a look at the makefile for comments on other targets (remake, release).

# Using

`./aesf <enc | dec> <input_file> <output_file> <options>`\
will use default settings (see options) and prompt for an AES key in hex format, ex. `00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F`

Here is a basic example of the program in use (AES-128, ECB):
```
markus@rz-pc:~/Desktop/example$ echo "very secret message" > secret.txt
markus@rz-pc:~/Desktop/example$ cat secret.txt
very secret message
markus@rz-pc:~/Desktop/example$ ./aesf enc secret.txt encrypted.txt
Hex Key: 01020304050607080910111213141516
markus@rz-pc:~/Desktop/example$ cat encrypted.txt

��
  2bD�
7كh
E��]x�<2����markus@rz-pc:~/Desktop/example$ 
markus@rz-pc:~/Desktop/example$ ./aesf dec encrypted.txt decrypted.txt
Hex Key: 01020304050607080910111213141516
markus@rz-pc:~/Desktop/example$ cat decrypted.txt
very secret message
markus@rz-pc:~/Desktop/example$ 
```
(though using a predictable 128-bit key in ECB mode may be a poor choice for a secret message)

# Options

A key may be specified as a command line option with the use of `-k`,\
ex. `./aesf <enc | dec> <input_file> <output_file> -k 000102030405060708090A0B0C0D0E0F`\
(Default: Prompt For Key)

The key size (128/192/256) may be specified with the use of `-s`,\
ex. `./aesf <enc | dec> <input_file> <output_file> -s 256 -k 1122334455667788990011223344556611223344556677889900112233445566`\
(Default: 128)

The mode of operation (ecb/cbc) may be specified using `-m`,\
ex. `./aesf <enc | dec> <input_file> <output_file> -s 256 -m cbc -k 1122334455667788990011223344556611223344556677889900112233445566 -iv 11223344556677889900112233445566`\
(Default: ecb)

An initialization vector (16 bytes, to match AES block size) is required for modes other than ECB and may be specified using `-iv`,\
ex. `./aesf <enc | dec> <input_file> <output_file> -s 256 -m cbc -k 1122334455667788990011223344556611223344556677889900112233445566 -iv 11223344556677889900112233445566`\
(Default: Prompt For Initialization Vector)

# Program Structure Overview

This is not meant to be a comprehensive description of every process involved but if you'd like to take a quick look around the source code this should give you an idea of where to begin:

1. The program begins in AESFileUtility.cpp, where the main method handles command-line arguments and prepares the information necessary for encryption/decryption (keys, files, etc). Since keys and initialization vectors are provided through ASCII text representing hexadecimal values, an instance of the HexInput class is used to extract their true values into byte arrays. The HexInput class is also capable of prompting a user for the ASCII input if it was not provided as a command-line argument.
2. An instance of the AES class is created given a key and key size; round keys are immediately generated so the class is ready to encrypt/decrypt individual blocks of data.
3. An instance of an operation mode (ECB/CBC) is created given the instance of AES. The operation mode takes an input stream (from the input file), reads the contents into individual blocks that can be processed by the AES algorithm, and writes the output into the given output stream (to the output file).

AES Implementation Notes:
* Constants such as the sbox values and round constants (used for AES "g" function) are located in AESConstants.h.
* The implementation does not perform multiplication in GF(2^8) for the "mix columns" function, but rather uses pre-computed values that can also be found in the AESConstants.h header.
