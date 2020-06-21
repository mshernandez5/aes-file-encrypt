#ifndef BLOCK_CIPHER_H
#define BLOCK_CIPHER_H

class BlockCipher
{
    public:
        virtual void encrypt(uint8_t * bytes) = 0;
        virtual void decrypt(uint8_t * bytes) = 0;
        int getBlockSize() { return blockByteSize; };
        BlockCipher(const int & blockSize) { blockByteSize = blockSize; };

    private:
        int blockByteSize;
};

#endif // BLOCK_CIPHER_H