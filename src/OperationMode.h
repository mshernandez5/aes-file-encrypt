#ifndef ENCRYPTION_MODE_H
#define ENCRYPTION_MODE_H

#include "BlockCipher.h"

#include <cstdint>
#include <fstream>

/**
 * @brief An abstract representation for modes of encryption.
 * 
 * OperationMode is an abstract class for modes of operation,
 * which are standard ways data can be encrypted or decrypted
 * using a block cipher. This is required since data comes in
 * varying lengths while these ciphers only accept a fixed
 * number of bytes as input.
 */
class OperationMode
{
    public:
        /**
         * @brief Encrypt data from a stream, writing encrypted data to an output stream.
         * 
         * @param inStream A stream of data to encrypt.
         * @param outStream The stream to write encrypted results to.
         */
        virtual void encrypt(std::istream& inStream, std::ostream& outStream) = 0;

        /**
         * @brief Decrypt data from a stream, writing decrypted data to an output stream.
         * 
         * @param inStream A stream of data to decrypt.
         * @param outStream The stream to write decrypted results to.
         */
        virtual void decrypt(std::istream& inStream, std::ostream& outStream) = 0;

        /**
         * @brief Pads a buffer to indicate unused space in the final block.
         * 
         * @param buffer The buffer to pad.
         * @param bytesRead The last number of bytes read.
         * @param blockSize The cipher block size.
         */
        void padBuffer(uint8_t* buffer, const int& bytesRead, const int& blockSize);
};

#endif // ENCRYPTION_MODE_H