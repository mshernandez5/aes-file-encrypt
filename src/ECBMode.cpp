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
    bool lastBlock = false;
    uint8_t * fileBuffer = new uint8_t[blockSize];
    do
    {
        inStream.read((char *) fileBuffer, blockSize);
        algorithm.decrypt(fileBuffer);
        lastBlock = inStream.peek() == EOF;
        if (lastBlock)
        {
            outStream.write((char *) fileBuffer, blockSize - fileBuffer[blockSize - 1]);
        }
        else
        {
            outStream.write((char *) fileBuffer, blockSize);
        }
    } while (!lastBlock);
    delete[] fileBuffer;
}