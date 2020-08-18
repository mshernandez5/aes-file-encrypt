#include "ECBMode.h"

ECBMode::ECBMode(BlockCipher & algorithm) : algorithm(algorithm) {}

void ECBMode::encrypt(std::istream & inStream, std::ostream & outStream)
{
    const int & blockSize = algorithm.getBlockSize();
    bool addedPadding = false;
    int bytesRead;
    uint8_t * fileBuffer = new uint8_t[blockSize];
    do
    {
        inStream.read((char *) fileBuffer, blockSize);
        bytesRead = inStream.gcount();
        if (bytesRead < blockSize)
        {
            addedPadding = true;
            padBuffer(fileBuffer, bytesRead, blockSize);
        }
        algorithm.encrypt(fileBuffer);
        outStream.write((char *) fileBuffer, blockSize);
    } while (!addedPadding);
    delete[] fileBuffer;
}

void ECBMode::decrypt(std::istream & inStream, std::ostream & outStream)
{
    const int & blockSize = algorithm.getBlockSize();
    bool moreBlocksAvailable = false;
    uint8_t * fileBuffer = new uint8_t[blockSize];
    do
    {
        inStream.read((char *) fileBuffer, blockSize);
        algorithm.decrypt(fileBuffer);
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
    } while (moreBlocksAvailable);
    delete[] fileBuffer;
}