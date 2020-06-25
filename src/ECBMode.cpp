#include "ECBMode.h"

void padBuffer(uint8_t * buffer, const int & bytesRead, const int & blockSize)
{
    int paddingByte = blockSize - bytesRead;
    for (int byte = bytesRead; byte < blockSize; ++byte)
    {
        buffer[byte] = paddingByte;
    }
}

void ECBMode::encrypt(std::istream & inStream, std::ostream & outStream, BlockCipher & algorithm)
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

void ECBMode::decrypt(std::istream & inStream, std::ostream & outStream, BlockCipher & algorithm)
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