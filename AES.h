// Markus Hernandez
// AES
// 3.22.2020

#include <iostream>
#include <string.h>
#include "aes128_constants.h"

class AES
{
    private:
        uint8_t * keySchedule;

        void addWords(uint8_t * a, uint8_t * b);

        void addKey(uint8_t * bytes, uint8_t * key);

        void leftCyclicByteShiftWord(uint8_t * word);

        uint8_t * aesG(uint8_t * word, int round);

        uint8_t * generateKeySchedule(uint8_t * keyBytes, int numBytes);

        void byteSubstitution(uint8_t * bytes);

        void shiftRows(uint8_t * bytes);

        void mixColumns(uint8_t * input);

    public:
        AES(uint8_t * keyBytes, int numBytes);

        ~AES();

        void encrypt(uint8_t * bytes);
};