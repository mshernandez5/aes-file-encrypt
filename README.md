# aes-file-encrypt

A basic implementation of AES 128 in ECB mode to encrypt files.
TODO: Larger Key Sizes (192/256), More Secure Modes
  
# Compiling

Compiling requires g++ and GNU Make

Clone the project and run `make` to generate an executable named `aesf`.

Remove the compiled classes and executable with `make clean`.

# Using

`./aesf <enc | dec> <input_file> <output_file> <options>` will prompt for an AES 128 key in hex format, ex. `00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F`

Alternatively, a key may be specified as a command line option with the use of `-k`, ex. `./aesf <enc | dec> <input_file> <output_file> -k 000102030405060708090A0B0C0D0E0F`
