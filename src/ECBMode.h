#ifndef ECB_MODE_H
#define ECB_MODE_H

#include "OperationMode.h"

/**
 * ECB (electronic code book) is a basic mode of operation
 * that simply splits data into blocks and encrypts each block
 * independently. Unfortunately, this means encrypting identical
 * blocks of data always produces identical results, which can
 * leave encrypted data open to analysis when blocks repeat.
 */
class ECBMode : public OperationMode
{
    private:
        BlockCipher& algorithm;

    public:
        ECBMode(BlockCipher& algorithm);

        void encrypt(std::istream& inStream, std::ostream& outStream);
        void decrypt(std::istream& inStream, std::ostream& outStream);
};

#endif // ECB_MODE_H