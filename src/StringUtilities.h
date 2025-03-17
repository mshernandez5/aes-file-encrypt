#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

#include <string>

namespace StringUtilities
{
    /**
     * @brief Checks whether two characters are equal, ignoring case.
     * 
     * @param a The first character.
     * @param b The second character.
     * @return true If the characters are equal when ignoring case.
     * @return false If the characters are not equal.
     */
    bool equalsIgnoreCase(const char& a, const char& b);

    /**
     * @brief Checks whether two strings are equal, ignoring character case.
     * 
     * Returns true if the provided strings have the same length
     * and contain equivalent characters (ignoring case) in every position.
     * 
     * @param a The first string.
     * @param b The second string.
     * @return true If the strings are equal when ignoring case.
     * @return false If the strings are not equal.
     */
    bool equalsIgnoreCase(const std::string& a, const std::string& b);
}

#endif // STRING_UTILITIES_H