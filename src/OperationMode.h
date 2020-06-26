#ifndef ENCRYPTION_MODE_H
#define ENCRYPTION_MODE_H

#include <fstream>

#include "BlockCipher.h"

class OperationMode
{
    public:
        virtual void encrypt(std::istream & inStream, std::ostream & outStream) = 0;
        virtual void decrypt(std::istream & inStream, std::ostream & outStream) = 0;

        void padBuffer(uint8_t * buffer, const int & bytesRead, const int & blockSize);
};

#endif // ENCRYPTION_MODE_H