#ifndef POKEDEV_STRING_PARSING_UTIL_HPP
#define POKEDEV_STRING_PARSING_UTIL_HPP

#ifdef _WIN32
#include <filesystem>
#else
#include <vector>
#include <filesystem>
#endif

#include <string>
#include <filesystem>

std::vector<std::string> parseDefines(const std::filesystem::path& filePath, const char* regex);
bool tryGetCsvColumn(const std::string& line, int index, std::string& outResult);
std::string getBetween(const std::string& value, char delimiter);
std::vector<std::string> split(const std::string& s, const std::string& delimiter);

// todo: move these to cpp file

// trim from start (in place)
inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
    {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
    {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
inline void trim(std::string &s)
{
    rtrim(s);
    ltrim(s);
}

// trim from start (copying)
[[nodiscard]]
inline std::string ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

// trim from end (copying)
[[nodiscard]]
inline std::string rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

// trim from both ends (copying)
[[nodiscard]]
inline std::string trim_copy(std::string s)
{
    trim(s);
    return s;
}

#endif
