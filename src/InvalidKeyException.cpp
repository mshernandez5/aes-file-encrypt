#include <string>
#include <sstream>

#include "InvalidKeyException.h"

InvalidKeyException::InvalidKeyException(const int & bytesRead, const int & bytesExpected)
            : std::runtime_error(getErrorMessage(bytesRead, bytesExpected)) {}

InvalidKeyException::InvalidKeyException(const int & bytesExpected)
            : std::runtime_error(getErrorMessage(bytesExpected)) {}

std::string InvalidKeyException::getErrorMessage(const int & bytesRead, const int & bytesExpected)
{
    std::ostringstream stringStream;
    stringStream << "Error, only " << bytesRead << "/"
        << bytesExpected << " bytes have been provided for the key!";
    return stringStream.str();
}

std::string InvalidKeyException::getErrorMessage(const int & bytesExpected)
{
    std::ostringstream stringStream;
    stringStream << "Error, over " << bytesExpected
        << " bytes have been provided for the key!";
    return stringStream.str();
}