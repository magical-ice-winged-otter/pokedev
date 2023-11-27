#include <fstream>
#include <regex>
#include "string_parsing_util.hpp"

std::vector<std::string> parseDefines(const std::filesystem::path& filePath, const char* regex) {
    std::ifstream stream {filePath};
    std::string line {};
    std::vector<std::string> results {};
    std::smatch match {};

    while (std::getline(stream, line)) {
        bool hasDefine = line.find("#define") != std::string::npos;

        if (hasDefine && std::regex_search(line, match, std::regex(regex))) {
            size_t start = line.find("#define") + 8;
            size_t end = line.find(' ', start);
            results.emplace_back(line.substr(start, end - start));
        }
    }

    return results;
}

bool tryGetCsvColumn(const std::string& line, int index, std::string& outResult) {
    size_t argStart {};

    for (size_t i = 0; i < index; ++i) {
        // Find the first space between args
        argStart = line.find(',', argStart);

        // Often, we have multiple spaces between args, so skip them all.
        argStart = line.find_first_not_of(' ', argStart + 1);
    }

    // Make sure we actually found our target: otherwise we failed.
    if (argStart == std::string::npos) {
        return false;
    }

    // Now we skip over to the end of the word, *assuming no spaces within*.
    size_t argEnd {line.find(',', argStart + 1)};

    // If we were given a valid buffer, write the result into it.
    outResult = line.substr(argStart, argEnd - argStart);
    trim(outResult);
    return true;
}

std::string getBetween(const std::string& value, char delimiter) {
    size_t start {value.find_first_of(delimiter) + 1};
    size_t end {value.find_first_of(delimiter, start)};
    return value.substr(start, end - start);
}

// for string delimiter
std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        trim(token);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    token = s.substr (pos_start);
    trim(token);
    res.push_back (token);
    return res;
}