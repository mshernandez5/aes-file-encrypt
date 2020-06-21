#include "AES.h"

AES::AES(uint8_t * keyBytes, const int & numBytes) : BlockCipher(16)
{
    keySchedule = generateKeySchedule(keyBytes, numBytes);
}

AES::~AES()
{
    delete[] keySchedule;
}

uint8_t * AES::generateKeySchedule(uint8_t * keyBytes, const int & numBytes)
{
    // 16 Bytes / Key * (Initial Key + 10 Round Keys) = 176 Bytes
    uint8_t * keySchedule = new uint8_t[176];
    // Full Key Size / Key * (Initial Key + 10 Round Keys)
    uint8_t * keyGeneration = new uint8_t[numBytes * 11];
    int numWords = numBytes / 4;
    // Copy Input Key to "Round 0" Initial Key
    for (int byte = 0; byte < 16; byte++)
        keySchedule[byte] = keyBytes[byte];
    for (int byte = 0; byte < numBytes; byte++)
        keyGeneration[byte] = keyBytes[byte];
    // Generate Round Keys
    for (int round = 1; round < 11; round++)
    {
        for (int byte = 0; byte < numBytes; byte++)
            keyGeneration[round*numBytes + byte] = keyGeneration[(round - 1)*16 + byte];
        for (int word = 0; word < numWords; word++)
        {
            if (word == 0)
            {
                uint8_t * gResult = aesG((keyGeneration + round*numBytes + 4*(numWords - 1)), round);
                addWords((keyGeneration + round*numBytes + 4*word), gResult);
                delete gResult;
            }
            // 256-bit Key Generation Involves an Additional "h-function"
            else if (numBytes == 32 && word == 4)
            {
                uint8_t * hResult = aesH(keyGeneration + round*numBytes + 12);
                addWords((keyGeneration + round*numBytes + 16), hResult);
                delete hResult;
            }
            else
                addWords((keyGeneration + round*numBytes + 4*word), (keyGeneration + round*numBytes + 4*(word - 1)));
        }
        // AES Only Uses The First 16 Bytes Of Each Round Key
        for (int byte = 0; byte < 16; byte++)
            keySchedule[round * 16 + byte] = keyGeneration[round*numBytes + byte];
    }
    delete[] keyGeneration;
    return keySchedule;
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

void AES::decrypt(uint8_t * bytes)
{
    // Round 10 Inverse
    addKey(bytes, (keySchedule + 160));
    inverseShiftRows(bytes);
    inverseByteSubstitution(bytes);

    // Rounds 9-1 Inverse
    for (int round = 9; round > 0; round--)
    {
        addKey(bytes, (keySchedule + 16 * round));
        inverseMixColumns(bytes);
        inverseShiftRows(bytes);
        inverseByteSubstitution(bytes);
    }
    
    // Key Whitening Inverse
    addKey(bytes, keySchedule);
}

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

uint8_t * AES::aesH(uint8_t * word)
{
    uint8_t * bytes = new uint8_t[4];
    for (int byte = 0; byte < 4; byte++)
        bytes[byte] = word[byte];
    for (int byte = 0; byte < 4; byte++)
        bytes[byte] = sbox[bytes[byte]];
    return bytes;
}

void AES::byteSubstitution(uint8_t * bytes)
{
    for (int byte = 0; byte < 16; byte++)
        bytes[byte] = sbox[bytes[byte]];
}

void AES::inverseByteSubstitution(uint8_t * bytes)
{
    for (int byte = 0; byte < 16; byte++)
        bytes[byte] = inv_sbox[bytes[byte]];
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

void AES::inverseShiftRows(uint8_t * bytes)
{
    uint8_t mappedBytes[16];
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
            mappedBytes[4*column + row] = bytes[4*((column + (4 - row)) % 4) + row];
    }
    for (int byte = 0; byte < 16; byte++)
        bytes[byte] = mappedBytes[byte];
}

void AES::mixColumns(uint8_t * bytes)
{
    uint8_t tmp[16];
    for (int i = 0; i <= 12; i = i + 4)
    {
        tmp[i]     = (uint8_t) (mul_2[bytes[i]] ^ mul_3[bytes[i + 1]] ^ bytes[i + 2]        ^ bytes[i + 3]);
        tmp[i + 1] = (uint8_t) (bytes[i]        ^ mul_2[bytes[i + 1]] ^ mul_3[bytes[i + 2]] ^ bytes[i + 3]);
        tmp[i + 2] = (uint8_t) (bytes[i]        ^ bytes[i + 1]        ^ mul_2[bytes[i + 2]] ^ mul_3[bytes[i + 3]]);
        tmp[i + 3] = (uint8_t) (mul_3[bytes[i]] ^ bytes[i + 1]        ^ bytes[i + 2]        ^ mul_2[bytes[i + 3]]);
    }
    for (int i = 0; i < 16; ++i)
        bytes[i] = tmp[i];
}

void AES::inverseMixColumns(uint8_t * bytes)
{
    uint8_t tmp[16];
    for (int i = 0; i <= 12; i = i + 4)
    {
        tmp[i]     = (uint8_t) (mul_e[bytes[i]] ^ mul_b[bytes[i + 1]] ^ mul_d[bytes[i + 2]] ^ mul_9[bytes[i + 3]]);
        tmp[i + 1] = (uint8_t) (mul_9[bytes[i]] ^ mul_e[bytes[i + 1]] ^ mul_b[bytes[i + 2]] ^ mul_d[bytes[i + 3]]);
        tmp[i + 2] = (uint8_t) (mul_d[bytes[i]] ^ mul_9[bytes[i + 1]] ^ mul_e[bytes[i + 2]] ^ mul_b[bytes[i + 3]]);
        tmp[i + 3] = (uint8_t) (mul_b[bytes[i]] ^ mul_d[bytes[i + 1]] ^ mul_9[bytes[i + 2]] ^ mul_e[bytes[i + 3]]);
    }
    for (int i = 0; i < 16; ++i)
        bytes[i] = tmp[i];
}