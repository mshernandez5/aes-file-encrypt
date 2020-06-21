#ifndef KEY_INPUT_H
#define KEY_INPUT_H

class KeyInput
{
    private:
        uint8_t * keyBytes;
        int keyByteSize;
        bool isValidHexValue(const char & c);
        uint8_t getHexValue(const char & c);

    public:
        KeyInput(const int & numBytes);
        ~KeyInput();
        uint8_t * keyRead(); //throws InvalidKeyException
        uint8_t * keyRead(const std::string & keyString); //throws InvalidKeyException
};

#endif // KEY_INPUT_H