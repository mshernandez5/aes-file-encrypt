#ifndef AES_H
#define AES_H

#include <cstdint>

#include "AESConstants.h"
#include "BlockCipher.h"

/**
 * Given a key and key size, an AES instance can encrypt
 * or decrypt 16-byte blocks of data.
 */
class AES : public BlockCipher
{
    public:
        AES(uint8_t * keyBytes, const int & keyByteSize);
        ~AES();
        void encrypt(uint8_t * bytes);
        void decrypt(uint8_t * bytes);

    private:
        uint8_t * keySchedule;
        int keyByteSize;
        int numberRounds;

        uint8_t * generateKeySchedule(uint8_t * keyBytes);
        void addWords(uint8_t * a, uint8_t * b);
        void addKey(uint8_t * bytes, uint8_t * key);
        void leftCyclicByteShiftWord(uint8_t * word);
        uint8_t * aesG(uint8_t * word, int round);
        uint8_t * aesH(uint8_t * word);
        void byteSubstitution(uint8_t * bytes);
        void inverseByteSubstitution(uint8_t * bytes);
        void shiftRows(uint8_t * bytes);
        void inverseShiftRows(uint8_t * bytes);
        void mixColumns(uint8_t * input);
        void inverseMixColumns(uint8_t * input);
};

#endif // AES_H
