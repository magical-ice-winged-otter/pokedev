#include "text_parser.hpp"

std::string TextParser::readToInclusive(std::istream& stream, char endCharacter)
{
    std::string result{};
    char cur;

    while (stream.get(cur))
    {
        result.push_back(cur);

        if (cur == endCharacter)
            return result;
    }

    return result;
}

std::string TextParser::readToExclusive(std::istream& stream, char endCharacter)
{
    std::string result{};
    char cur;

    while (stream.get(cur))
    {
        if (cur == endCharacter)
            return result;

        result.push_back(cur);
    }

    return result;
}

bool TextParser::tryFind(std::istream& stream, char character, char stopCharacter)
{
    char cur{};

    while (stream.get(cur))
    {
        if (cur == stopCharacter)
            return false;

        if (cur == character)
        {
            stream.putback(character);
            return true;
        }
    }

    return false;
}

bool TextParser::tryFind(std::istream& stream, char character)
{
    char cur{};

    while (stream.get(cur))
    {
        if (cur == character)
        {
            stream.putback(character);
            return true;
        }
    }

    return false;
}

bool TextParser::tryFind(std::istream& stream, const std::string& string, char stopCharacter)
{
    size_t matches{0};

    while (stream)
    {
        tryFind(stream, string[0]);

        for (char i : string)
        {
            char cur;

            if (stream.get(cur))
            {
                if (i == stopCharacter)
                    return false;

                if (i == cur)
                {
                    matches++;
                    continue;
                }
            }

            matches = 0;
            break;
        }

        if (matches == string.size())
        {
            for (int i = static_cast<int>(string.size()) - 1; i >= 0; --i)
                stream.putback(string[i]);

            return true;
        }
    }

    return false;
}

bool TextParser::tryFind(std::istream& stream, const std::string& string)
{
    size_t matches{0};

    while (stream)
    {
        tryFind(stream, string[0]);

        for (char i : string)
        {
            char cur;

            if (stream.get(cur) && i == cur)
            {
                matches++;
                continue;
            }

            matches = 0;
            break;
        }

        if (matches == string.size())
        {
            for (int i = static_cast<int>(string.size()) - 1; i >= 0; --i)
                stream.putback(string[i]);

            return true;
        }
    }

    return false;
}
