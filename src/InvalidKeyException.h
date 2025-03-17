#ifndef INVALID_KEY_EXCEPTION_H
#define INVALID_KEY_EXCEPTION_H

#include <stdexcept>

class InvalidKeyException : public std::runtime_error
{
    private:
        std::string getErrorMessage(const int& bytesRead);
        std::string getErrorMessage(const int& bytesRead, const int& bytesExpected);

    public:
        InvalidKeyException(const int& bytesRead, const int& bytesExpected);
        InvalidKeyException(const int& bytesExpected);
};

#endif // INVALID_KEY_EXCEPTION_H