#ifndef ECB_MODE_H
#define ECB_MODE_H

#include "OperationMode.h"

class ECBMode : public OperationMode
{
    public:
        void encrypt(std::istream & inStream, std::ostream & outStream, BlockCipher & algorithm);
        void decrypt(std::istream & inStream, std::ostream & outStream, BlockCipher & algorithm);
};

#endif // ECB_MODE_H