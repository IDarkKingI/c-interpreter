#pragma once


#include "lexer.hpp"
#include <stdexcept>


class Lexer::Alphabet {
private:
    static const string literals;
    static const string digits;
    static const string spaces;
    static const string metachars;

public:
    static bool isspace(const char&);
    static bool isliteral(const char&);
    static bool isdigit(const char&);
    static bool isalphanumeric(const char&);
    static bool ismetachar(const char&);
    static bool isdot(const char&);
    static bool isquote(const char&);
};
