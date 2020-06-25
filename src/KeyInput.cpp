#include <iostream>
#include <string>
#include <stdint.h>

#include "KeyInput.h"
#include "InvalidKeyException.h"

KeyInput::KeyInput(const int & numBytes)
{
    keyBytes = new uint8_t[numBytes];
    keyByteSize = numBytes;
}

KeyInput::~KeyInput()
{
    delete[] keyBytes;
}

struct Byte
{
    uint8_t nibble1 : 4;
    uint8_t nibble2 : 4;
    uint8_t getByte()
    {
        return (nibble1 << 4) + nibble2;
    }
};

bool KeyInput::isValidHexValue(const char & c)
{
    return (c >= 48 && c <= 57) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102);
}

uint8_t KeyInput::getHexValue(const char & c)
{
    if (c >= 48 && c <= 57)
    {
        return (c - '0');
    }
    else if (c >= 65 && c <= 70)
    {
        return (c - 'A') + 10;
    }
    else if (c >= 97 && c <= 102)
    {
        return (c - 'a') + 10;
    }
    return 0;
}

uint8_t * KeyInput::keyRead()
{
    std::string keyString;
    std::cout << "Hex Key: ";
    std::getline(std::cin, keyString);
    return keyRead(keyString);
}

uint8_t * KeyInput::keyRead(const std::string & keyString)
{
    Byte tempByte;
    int numberNibblesRead = 0;
    for (const char & c : keyString)
    {
        if (numberNibblesRead > (keyByteSize * 2))
        {
            throw InvalidKeyException(keyByteSize);
        }
        if (isValidHexValue(c))
        {
            if (numberNibblesRead % 2 == 0)
            {
                tempByte.nibble1 = getHexValue(c);
            }
            else
            {
                tempByte.nibble2 = getHexValue(c);
                keyBytes[(numberNibblesRead / 2)] = tempByte.getByte();
            }
            numberNibblesRead++;
        }
    }
    if (numberNibblesRead < keyByteSize * 2)
    {
        throw InvalidKeyException((numberNibblesRead / 2), keyByteSize);
    }
    return keyBytes;
}