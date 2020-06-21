#ifndef ENCRYPTION_MODE_H
#define ENCRYPTION_MODE_H

#include <fstream>

#include "BlockCipher.h"

class OperationMode
{
    public:
        virtual void encrypt(std::istream & inStream, std::ostream & outStream, BlockCipher & algorithm) = 0;
        virtual void decrypt(std::istream & inStream, std::ostream & outStream, BlockCipher & algorithm) = 0;
};

#endif // ENCRYPTION_MODE_H