// Markus Hernandez
// AES
// 3.22.2020

#include <iostream>
#include "KeyInput.h"

InvalidKeyException::InvalidKeyException(int bytesRead, int bytesExpected)
{
    message = new char[75];
    snprintf(message, 75, "Error, only %d/%d bytes have been provided for the key!",
                bytesRead, bytesExpected);
}

InvalidKeyException::InvalidKeyException(int bytesExpected)
{
    message = new char[75];
    snprintf(message, 75, "Error, over %d bytes have been provided for the key!",
                bytesExpected);
}

InvalidKeyException::~InvalidKeyException()
{
    delete[] message;
}

const char * InvalidKeyException::what()
{
    return message;
}

/**
 * A byte structure used to conserve memory by storing pairs of octals in the same memory spaces.
 */
struct Byte
{
    uint8_t octal1 : 4;
    uint8_t octal2 : 4;

    uint8_t getByte()
    {
        return (octal1 << 4) + octal2;
    }
};

/**
 * Checks whether or not a given char could represent a hexidecimal value.
 */
bool KeyInput::representsValidOctalValue(char & c)
{
    if ((c >= 48 && c <= 57) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102))
        return true;
    return false;
}

/**
 * Returns a single-byte integer reflecting the hex value represented by a character.
 */
uint8_t KeyInput::hexCharToOctal(char & c)
{
    if (c >= 48 && c <= 57)
        return (c - '0');
    else if (c >= 65 && c <= 70)
        return (c - 'A') + 10;
    else if (c >= 97 && c <= 102)
        return (c - 'a') + 10;
    return 0;
}

KeyInput::KeyInput(int numBytes)
{
    keyBytes = new uint8_t[numBytes];
    keySize = numBytes;
    inputBufferSize = numBytes * 4;
}

KeyInput::~KeyInput()
{
    delete[] keyBytes;
}

/**
 * Prompts the user for a hex key and returns the bytes extracted from the input.
 * An InvalidKeyException is thrown if the complete key is not provided.
 */
uint8_t * KeyInput::keyRead()
{
    // Take Key Hex Value Input
    char inputBuffer[inputBufferSize];
    std::cout << "Hex Key: ";
    std::cin.getline(inputBuffer, inputBufferSize);

    // Extract Key Bytes From Buffer
    Byte tempByte;
    int validOctals = 0;
    for (int i = 0; i < inputBufferSize; i++)
    {
        if (validOctals > (keySize * 2))
            throw InvalidKeyException(keySize);
        char & c = inputBuffer[i];
        if (c == 0)
            break;
        if (representsValidOctalValue(c))
        {
            if (validOctals % 2 == 0)
                tempByte.octal1 = hexCharToOctal(c);
            else
            {
                tempByte.octal2 = hexCharToOctal(c);
                keyBytes[(validOctals / 2)] = tempByte.getByte();
            }
            validOctals++;
        }
    }
    if (validOctals < keySize * 2)
        throw InvalidKeyException((validOctals / 2), keySize);
    return keyBytes;
}