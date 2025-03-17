#ifndef CBC_MODE_H
#define CBC_MODE_H

#include "OperationMode.h"

#include <cstdint>

/**
 * CBC (cipher block chaining) is a mode of operation that
 * "adds" (XOR) the previous cipher block (already encrypted)
 * to the current block of data before encrypting. For the
 * first block, an initialization vector is used in place of
 * a previous cipher block (which does not exist).
 */
class CBCMode : public OperationMode
{
    private:
        BlockCipher& algorithm;
        uint8_t* initializationVector;

    public:
        CBCMode(BlockCipher& algorithm, uint8_t* initializationVector);
        ~CBCMode();

        void encrypt(std::istream& inStream, std::ostream& outStream);
        void decrypt(std::istream& inStream, std::ostream& outStream);

        void addBlock(uint8_t* a, uint8_t* b, const int& blockSize);
};

#endif // CBC_MODE_H