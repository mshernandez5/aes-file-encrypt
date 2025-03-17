#include "CBCMode.h"

CBCMode::CBCMode(BlockCipher& algorithm, uint8_t* initializationVector) : algorithm(algorithm)
{
    const int& blockSize = algorithm.getBlockSize();
    this->initializationVector = new uint8_t[blockSize];
    for (int byte = 0; byte < blockSize; ++byte)
    {
        this->initializationVector[byte] = initializationVector[byte];
    }
}

CBCMode::~CBCMode()
{
    delete[] initializationVector;
}

void CBCMode::encrypt(std::istream& inStream, std::ostream& outStream)
{
    const int& blockSize = algorithm.getBlockSize();
    bool addedPadding = false;
    int bytesRead;
    uint8_t* fileBuffer = new uint8_t[blockSize];
    uint8_t* lastCipherBlock = new uint8_t[blockSize];
    for (int byte = 0; byte < blockSize; ++byte)
    {
        lastCipherBlock[byte] = initializationVector[byte];
    }
    do
    {
        inStream.read((char *) fileBuffer, blockSize);
        bytesRead = inStream.gcount();
        if (bytesRead < blockSize)
        {
            addedPadding = true;
            padBuffer(fileBuffer, bytesRead, blockSize);
        }
        addBlock(fileBuffer, lastCipherBlock, blockSize);
        algorithm.encrypt(fileBuffer);
        for (int byte = 0; byte < blockSize; ++byte)
        {
            lastCipherBlock[byte] = fileBuffer[byte];
        }
        outStream.write((char *) fileBuffer, blockSize);
    } while (!addedPadding);
    delete[] lastCipherBlock;
    delete[] fileBuffer;
}

void CBCMode::decrypt(std::istream& inStream, std::ostream& outStream)
{
    const int& blockSize = algorithm.getBlockSize();
    bool moreBlocksAvailable = false;
    uint8_t* lastCipherBlock = new uint8_t[blockSize];
    uint8_t* currentCipherBlock = new uint8_t[blockSize];
    uint8_t* fileBuffer = new uint8_t[blockSize];
    for (int byte = 0; byte < blockSize; ++byte)
    {
        lastCipherBlock[byte] = initializationVector[byte];
    }
    do
    {
        inStream.read((char *) fileBuffer, blockSize);
        for (int byte = 0; byte < blockSize; ++byte)
        {
            currentCipherBlock[byte] = fileBuffer[byte];
        }
        algorithm.decrypt(fileBuffer);
        addBlock(fileBuffer, lastCipherBlock, blockSize);
        moreBlocksAvailable = inStream.peek() != EOF;
        // If this is the last block, then following the PKCS7 padding
        // standard the last byte of this block will indicate how many
        // bytes were used to pad the block; we will discard these bytes
        // as they are not part of the original data.
        if (!moreBlocksAvailable)
        {
            outStream.write((char *) fileBuffer, blockSize - fileBuffer[blockSize - 1]);
        }
        else
        {
            outStream.write((char *) fileBuffer, blockSize);
        }
        for (int byte = 0; byte < blockSize; ++byte)
        {
            lastCipherBlock[byte] = currentCipherBlock[byte];
        }
    } while (moreBlocksAvailable);
    delete[] currentCipherBlock;
    delete[] lastCipherBlock;
    delete[] fileBuffer;
}

void CBCMode::addBlock(uint8_t* a, uint8_t* b, const int& blockSize)
{
    for (int byte = 0; byte < blockSize; ++byte)
    {
        a[byte] ^= b[byte];
    }
}