#ifndef POKEDEV_TEXT_PARSER_HPP
#define POKEDEV_TEXT_PARSER_HPP

#include <fstream>

namespace TextParser
{
    std::string readToExclusive(std::istream& stream, char endCharacter);
    std::string readToInclusive(std::istream& stream, char endCharacter);

    bool tryFind(std::istream& stream, char character, char stopCharacter);
    bool tryFind(std::istream& stream, char character);
    bool tryFind(std::istream& stream, const std::string& string, char stopCharacter);
    bool tryFind(std::istream& stream, const std::string& string);

} // namespace TextParser

#endif // POKEDEV_TEXT_PARSER_HPP
