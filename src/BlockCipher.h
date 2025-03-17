#ifndef BLOCK_CIPHER_H
#define BLOCK_CIPHER_H

#include <cstdint>

/**
 * BlockCipher is an abstract class for algorithms that
 * encrypt or decrypt a fixed amount of data (a block).
 */
class BlockCipher
{
    public:
        virtual void encrypt(uint8_t* bytes) = 0;
        virtual void decrypt(uint8_t* bytes) = 0;
        int getBlockSize() { return blockByteSize; };
        BlockCipher(const int& blockSize) { blockByteSize = blockSize; };

    private:
        int blockByteSize;
};

#endif // BLOCK_CIPHER_H