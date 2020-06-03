// Markus Hernandez
// AES
// 3.22.2020

#include <iostream>

/**
 * An exception class that is thrown when the complete size of the key is not provided.
 * Contains a message displaying the number of bytes read and expected.
 */
class InvalidKeyException
{
    private:
        char * message;

    public:
        InvalidKeyException(int bytesRead, int bytesExpected);

        InvalidKeyException(int bytesExpected);

        ~InvalidKeyException();

        const char * what();
};

class KeyInput
{
    private:
        uint8_t * keyBytes;
        int keySize, inputBufferSize;

        /**
         * Checks whether or not a given char could represent a hexidecimal value.
         */
        bool representsValidOctalValue(char & c);

        /**
         * Returns a single-byte integer reflecting the hex value represented by a character.
         */
        uint8_t hexCharToOctal(char & c);

    public:
        KeyInput(int numBytes);

        ~KeyInput();

        /**
         * Prompts the user for a hex key and returns the bytes extracted from the input.
         * An InvalidKeyException is thrown if the complete key is not provided.
         */
        uint8_t * keyRead();
};