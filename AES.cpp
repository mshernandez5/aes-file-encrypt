// Markus Hernandez
// AES
// 3.22.2020

#include "AES.h"

void AES::addWords(uint8_t * a, uint8_t * b)
{
    for (int byte = 0; byte < 4; byte++)
        a[byte] ^= b[byte];
}

void AES::addKey(uint8_t * bytes, uint8_t * key)
{
    for (int byte = 0; byte < 16; byte++)
        bytes[byte] ^= key[byte];
}

void AES::leftCyclicByteShiftWord(uint8_t * word)
{
    for (int byte = 1; byte < 4; byte++)
    {
        word[byte] ^= word[byte - 1];
        word[byte - 1] ^= word[byte];
        word[byte] ^= word[byte - 1];
    }
}

uint8_t * AES::aesG(uint8_t * word, int round)
{
    uint8_t * bytes = new uint8_t[4];
    for (int byte = 0; byte < 4; byte++)
        bytes[byte] = word[byte];
    leftCyclicByteShiftWord(bytes);
    for (int byte = 0; byte < 4; byte++)
        bytes[byte] = sbox[bytes[byte]];
    bytes[0] ^= rcon[round];
    return bytes;
}

uint8_t * AES::generateKeySchedule(uint8_t * keyBytes, int numBytes)
{
    int numWords = numBytes / 4;
    uint8_t * keySchedule = new uint8_t[176];
    for (int byte = 0; byte < numBytes; byte++)
        keySchedule[byte] = keyBytes[byte];
    for (int round = 1; round < 11; round++)
    {
        for (int byte = 0; byte < numBytes; byte++)
            keySchedule[round*16 + byte] = keySchedule[(round - 1)*16 + byte];
        for (int word = 0; word < numWords; word++)
        {
            if (word == 0)
            {
                uint8_t * gResult = aesG((keySchedule + round*16 + 4*(numWords - 1)), round);
                addWords((keySchedule + round*16 + 4*word), gResult);
                delete gResult;
            }
            else
                addWords((keySchedule + round*16 + 4*word), (keySchedule + round*16 + 4*(word - 1)));
        }
    }
    return keySchedule;
}

void AES::byteSubstitution(uint8_t * bytes)
{
    for (int byte = 0; byte < 16; byte++)
        bytes[byte] = sbox[bytes[byte]];
}

void AES::shiftRows(uint8_t * bytes)
{
    uint8_t mappedBytes[16];
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
            mappedBytes[4*column + row] = bytes[4*((column + row) % 4) + row];
    }
    for (int byte = 0; byte < 16; byte++)
        bytes[byte] = mappedBytes[byte];
}

void AES::mixColumns(uint8_t * input)
{
    uint8_t tmp[16];
    int i;
    for (i = 0; i < 4; ++i) {
        tmp[(i << 2) + 0] = (uint8_t) (mul2[input[(i << 2) + 0]] ^ mul_3[input[(i << 2) + 1]] ^ input[(i << 2) + 2] ^ input[(i << 2) + 3]);
        tmp[(i << 2) + 1] = (uint8_t) (input[(i << 2) + 0] ^ mul2[input[(i << 2) + 1]] ^ mul_3[input[(i << 2) + 2]] ^ input[(i << 2) + 3]);
        tmp[(i << 2) + 2] = (uint8_t) (input[(i << 2) + 0] ^ input[(i << 2) + 1] ^ mul2[input[(i << 2) + 2]] ^ mul_3[input[(i << 2) + 3]]);
        tmp[(i << 2) + 3] = (uint8_t) (mul_3[input[(i << 2) + 0]] ^ input[(i << 2) + 1] ^ input[(i << 2) + 2] ^ mul2[input[(i << 2) + 3]]);
    }

    for (i = 0; i < 16; ++i)
        input[i] = tmp[i];
}

AES::AES(uint8_t * keyBytes, int numBytes)
{
    keySchedule = generateKeySchedule(keyBytes, numBytes);
}

AES::~AES()
{
    delete[] keySchedule;
}

void AES::encrypt(uint8_t * bytes)
{
    // Key Whitening
    addKey(bytes, keySchedule);
    // Rounds 1-9
    for (int round = 1; round < 10; round++)
    {
        byteSubstitution(bytes);
        shiftRows(bytes);
        mixColumns(bytes);
        addKey(bytes, (keySchedule + 16 * round));
    }
    // Round 10
    byteSubstitution(bytes);
    shiftRows(bytes);
    addKey(bytes, (keySchedule + 160));
}