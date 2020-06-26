# aes-file-encrypt

A basic implementation of AES 128/192/256 in ECB/CBC mode using PKCS7 padding to encrypt files.\
TODO: More Modes, Multithreaded ECB

# Compiling

Compiling requires g++ and GNU Make

Clone the project and run `make` to generate an executable named `aesf`.

Remove the compiled classes and executable with `make clean`.

# Using

`./aesf <enc | dec> <input_file> <output_file> <options>`\
will prompt for an AES key in hex format, ex. `00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F`

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
