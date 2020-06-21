#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

bool equalsIgnoreCase(const char & a, const char & b)
{
    return tolower(a) == tolower(b);
}

bool equalsIgnoreCase(const std::string & a, const std::string & b)
{
    if (a.size() != b.size())
        return false;
    for (std::string::size_type i = 0; i < a.size(); ++i)
    { 
        if (!equalsIgnoreCase(a[i], b[i]))
            return false;
    }
    return true;
}

#endif // UTILITIES_H