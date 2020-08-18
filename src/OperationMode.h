#ifndef ENCRYPTION_MODE_H
#define ENCRYPTION_MODE_H

#include <fstream>

#include "BlockCipher.h"

/**
 * OperationMode is an abstract class for modes of operation,
 * which are standard ways data can be encrypted or decrypted
 * using a block cipher. This is required since data comes in
 * varying lengths while these ciphers only accept a fixed
 * number of bytes as input.
 */
class OperationMode
{
    public:
        virtual void encrypt(std::istream & inStream, std::ostream & outStream) = 0;
        virtual void decrypt(std::istream & inStream, std::ostream & outStream) = 0;

        void padBuffer(uint8_t * buffer, const int & bytesRead, const int & blockSize);
};

#endif // ENCRYPTION_MODE_H