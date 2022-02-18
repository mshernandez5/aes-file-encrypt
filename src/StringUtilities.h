#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

#include <string>

namespace StringUtilities
{
    // String Utilities
    bool equalsIgnoreCase(const char & a, const char & b);
    bool equalsIgnoreCase(const std::string & a, const std::string & b);
}

#endif // STRING_UTILITIES_H