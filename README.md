# aes-file-encrypt

A basic implementation of AES 128 in ECB mode to encrypt files.
TODO: Support for larger key sizes, file decryption, alternate modes, better command line arguments
  
# Compiling

Compiling requires g++ and GNU Make

Clone the project and run `make` to generate an executable named `AES128Encrypt`.

Remove the compiled classes and executable with `make clean`.

# Using

`./AES128Encrypt <input_file>` will prompt for an AES 128 key in hex format, ex. `00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F`
The program will output a file of the same name except with the extention replaced with `.enc` or with the `.enc` extension added to the file name if none is present. Please be aware that files of the same name are overwritten without warning.
