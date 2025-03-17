#ifndef HEX_INPUT_H
#define HEX_INPUT_H

#include <cstdint>

/**
 * @brief Interprets hex strings into byte arrays.
 * 
 * Takes ASCII strings representing hexadecimal values (ex. "0A")
 * and stores an array of bytes holding the interpreted values.
 */
class HexInput
{
    private:
        uint8_t * keyBytes;
        int keyByteSize;
        std::string prompt;
        bool isValidHexValue(const char & c);
        uint8_t getHexValue(const char & c);

    public:
        HexInput(const int & numBytes);
        HexInput(const int & numBytes, const std::string & prompt);
        ~HexInput();
        uint8_t * keyRead(); //throws InvalidKeyException
        uint8_t * keyRead(const std::string & keyString); //throws InvalidKeyException
};

#endif // HEX_INPUT_H