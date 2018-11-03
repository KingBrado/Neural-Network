#ifndef UTILITY_H
#define UTILITY_H
#include <string>
#include <vector>
#include <cctype>

/**
 * @file utility.h
 * @brief Contains class @ref Utility
 * @author B. M. Manzi
 * @date 20/11/2017
 */

 /**
 * @brief Class containing static functions for useful operations
 */
class Utility
{
public:
    /**
    * @brief Static function splitting a string in words, using the a single space 
    * character as divider.
    * @param word std::string to split.
    * @return std::vector of words in \p word.
    */
    static std::vector<std::string> split(const std::string& word)
    {
        std::vector<std::string> splittedLine;
        size_t oldPos = 0;
        size_t pos = word.find(' ',oldPos);
        while(pos < word.size())
        {
            splittedLine.push_back(word.substr(oldPos,pos-oldPos));
            oldPos = pos + 1;
            pos = word.find(' ',oldPos);
        }
        splittedLine.push_back(word.substr(oldPos,pos-oldPos));
        return splittedLine;
    }
    /**
    * @brief Utility function for changing all characters in a std::string to lower case.
    * @param word std::string to modify.
    */
    static void tolower(std::string& word)
    {
        for(unsigned i = 0; i < word.length(); ++i)
        {
            word[i] = std::tolower(word[i]);
        }
    }
    /**
    * @brief Absolute value for doubles.
    * @param x Double to apply the funtion on.
    * @return @f$ \vert x \vert @f$ .
    */
    static double abs(double x)
    {
        return (x >= 0) ? x : -x;
    }
};
#endif // UTILITY_H            