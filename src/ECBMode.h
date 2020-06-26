#ifndef ECB_MODE_H
#define ECB_MODE_H

#include "OperationMode.h"

class ECBMode : public OperationMode
{
    private:
        BlockCipher & algorithm;

    public:
        ECBMode(BlockCipher & algorithm);

        void encrypt(std::istream & inStream, std::ostream & outStream);
        void decrypt(std::istream & inStream, std::ostream & outStream);
};

#endif // ECB_MODE_H