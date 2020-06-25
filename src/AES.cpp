#include "AES.h"

AES::AES(uint8_t * keyBytes, const int & numBytes) : BlockCipher(16)
{
    keyByteSize = numBytes;
    switch (keyByteSize)
    {
        case 16:
            numberRounds = 10;
            break;
        case 24:
            numberRounds = 12;
            break;
        default:
            numberRounds = 14;
    }
    keySchedule = generateKeySchedule(keyBytes);
}

AES::~AES()
{
    delete[] keySchedule;
}

void AES::encrypt(uint8_t * bytes)
{
    // Key Whitening
    addKey(bytes, keySchedule);

    // General Rounds
    for (int round = 1; round < numberRounds; ++round)
    {
        byteSubstitution(bytes);
        shiftRows(bytes);
        mixColumns(bytes);
        addKey(bytes, (keySchedule + 16 * round));
    }

    // Last Round
    byteSubstitution(bytes);
    shiftRows(bytes);
    addKey(bytes, (keySchedule + 16 * numberRounds));
}

void AES::decrypt(uint8_t * bytes)
{
    // Last Round Inverse
    addKey(bytes, (keySchedule + 16 * numberRounds));
    inverseShiftRows(bytes);
    inverseByteSubstitution(bytes);

    // General Rounds Inverse
    for (int round = (numberRounds - 1); round > 0; --round)
    {
        addKey(bytes, (keySchedule + 16 * round));
        inverseMixColumns(bytes);
        inverseShiftRows(bytes);
        inverseByteSubstitution(bytes);
    }
    
    // Key Whitening Inverse
    addKey(bytes, keySchedule);
}

uint8_t * AES::generateKeySchedule(uint8_t * keyBytes)
{
    int numberKeyRounds = (numberRounds * 16) / keyByteSize;
    int numberWordsPerKeyRound = keyByteSize / 4;
    uint8_t * keyExpansion = new uint8_t[keyByteSize * (numberKeyRounds + 1)];
    // Copy Input Key to "Round 0" Initial Key
    for (int byte = 0; byte < keyByteSize; ++byte)
    {
        keyExpansion[byte] = keyBytes[byte];
    }
    // Rounds Of Key Expansion
    for (int round = 1; round <= numberKeyRounds; round++)
    {
        uint8_t * roundKeyPtr = keyExpansion + round * keyByteSize;
        for (int byte = 0; byte < keyByteSize; ++byte)
        {
            roundKeyPtr[byte] = keyExpansion[keyByteSize * (round - 1) + byte];
        }
        for (int word = 0; word < numberWordsPerKeyRound; ++word)
        {
            uint8_t * wordPtr = roundKeyPtr + word * 4;
            if (word == 0)
            {
                uint8_t * gResult = aesG((roundKeyPtr + (numberWordsPerKeyRound - 1) * 4), round);
                addWords(wordPtr, gResult);
                delete gResult;
            }
            else if (keyByteSize == 32 && word == 4)
            {
                uint8_t * hResult = aesH(wordPtr - 4);
                addWords(wordPtr, hResult);
                delete hResult;
            }
            else
            {
                addWords(wordPtr, wordPtr - 4);
            }
        }
    }
    return keyExpansion;
}

uint8_t * AES::aesG(uint8_t * word, int round)
{
    uint8_t * bytes = new uint8_t[4];
    for (int byte = 0; byte < 4; ++byte)
    {
        bytes[byte] = word[byte];
    }
    leftCyclicByteShiftWord(bytes);
    for (int byte = 0; byte < 4; ++byte)
    {
        bytes[byte] = sbox[bytes[byte]];
    }
    bytes[0] ^= rcon[round];
    return bytes;
}

uint8_t * AES::aesH(uint8_t * word)
{
    uint8_t * bytes = new uint8_t[4];
    for (int byte = 0; byte < 4; ++byte)
    {
        bytes[byte] = word[byte];
    }
    for (int byte = 0; byte < 4; ++byte)
    {
        bytes[byte] = sbox[bytes[byte]];
    }
    return bytes;
}

void AES::addWords(uint8_t * a, uint8_t * b)
{
    for (int byte = 0; byte < 4; ++byte)
    {
        a[byte] ^= b[byte];
    }
}

void AES::addKey(uint8_t * bytes, uint8_t * key)
{
    for (int byte = 0; byte < 16; ++byte)
    {
        bytes[byte] ^= key[byte];
    }
}

void AES::leftCyclicByteShiftWord(uint8_t * word)
{
    for (int byte = 1; byte < 4; ++byte)
    {
        word[byte] ^= word[byte - 1];
        word[byte - 1] ^= word[byte];
        word[byte] ^= word[byte - 1];
    }
}

void AES::byteSubstitution(uint8_t * bytes)
{
    for (int byte = 0; byte < 16; ++byte)
    {
        bytes[byte] = sbox[bytes[byte]];
    }
}

void AES::inverseByteSubstitution(uint8_t * bytes)
{
    for (int byte = 0; byte < 16; ++byte)
    {
        bytes[byte] = inv_sbox[bytes[byte]];
    }
}

void AES::shiftRows(uint8_t * bytes)
{
    uint8_t mappedBytes[16];
    for (int row = 0; row < 4; ++row)
    {
        for (int column = 0; column < 4; ++column)
        {
            mappedBytes[4*column + row] = bytes[4*((column + row) % 4) + row];
        }
    }
    for (int byte = 0; byte < 16; ++byte)
    {
        bytes[byte] = mappedBytes[byte];
    }
}

void AES::inverseShiftRows(uint8_t * bytes)
{
    uint8_t mappedBytes[16];
    for (int row = 0; row < 4; ++row)
    {
        for (int column = 0; column < 4; ++column)
        {
            mappedBytes[4*column + row] = bytes[4*((column + (4 - row)) % 4) + row];
        }
    }
    for (int byte = 0; byte < 16; ++byte)
    {
        bytes[byte] = mappedBytes[byte];
    }
}

void AES::mixColumns(uint8_t * bytes)
{
    uint8_t tmp[16];
    for (int i = 0; i <= 12; i += 4)
    {
        tmp[i]     = (uint8_t) (mul_2[bytes[i]] ^ mul_3[bytes[i + 1]] ^ bytes[i + 2]        ^ bytes[i + 3]);
        tmp[i + 1] = (uint8_t) (bytes[i]        ^ mul_2[bytes[i + 1]] ^ mul_3[bytes[i + 2]] ^ bytes[i + 3]);
        tmp[i + 2] = (uint8_t) (bytes[i]        ^ bytes[i + 1]        ^ mul_2[bytes[i + 2]] ^ mul_3[bytes[i + 3]]);
        tmp[i + 3] = (uint8_t) (mul_3[bytes[i]] ^ bytes[i + 1]        ^ bytes[i + 2]        ^ mul_2[bytes[i + 3]]);
    }
    for (int i = 0; i < 16; ++i)
    {
        bytes[i] = tmp[i];
    }
}

void AES::inverseMixColumns(uint8_t * bytes)
{
    uint8_t tmp[16];
    for (int i = 0; i <= 12; i += 4)
    {
        tmp[i]     = (uint8_t) (mul_e[bytes[i]] ^ mul_b[bytes[i + 1]] ^ mul_d[bytes[i + 2]] ^ mul_9[bytes[i + 3]]);
        tmp[i + 1] = (uint8_t) (mul_9[bytes[i]] ^ mul_e[bytes[i + 1]] ^ mul_b[bytes[i + 2]] ^ mul_d[bytes[i + 3]]);
        tmp[i + 2] = (uint8_t) (mul_d[bytes[i]] ^ mul_9[bytes[i + 1]] ^ mul_e[bytes[i + 2]] ^ mul_b[bytes[i + 3]]);
        tmp[i + 3] = (uint8_t) (mul_b[bytes[i]] ^ mul_d[bytes[i + 1]] ^ mul_9[bytes[i + 2]] ^ mul_e[bytes[i + 3]]);
    }
    for (int i = 0; i < 16; ++i)
    {
        bytes[i] = tmp[i];
    }
}