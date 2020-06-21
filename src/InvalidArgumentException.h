#ifndef INVALID_ARGUMENT_EXCEPTION_H
#define INVALID_ARGUMENT_EXCEPTION_H

#include <stdexcept>

class InvalidArgumentException : public std::runtime_error
{
    private:
        std::string getErrorMessage(const std::string & details);

    public:
        InvalidArgumentException(const std::string & details);
};

#endif // INVALID_ARGUMENT_EXCEPTION_H