#pragma once


using namespace std;


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>


class Token {
public:
    enum TokenType {
        NUMBER, 
        CHAR, 
        IDENTIFIER, // name of functions, vars, structs etc.
        DATATYPE, // void, bool, int double, char
        STRING, // string
        KEYWORD, // if, elif, else, while, struct, main, return, continue, break
        MATH_FUNC, // pow, log, exp, sin, cos
        BUILTIN_FUNC, // read, print, abs, sgn
        PLUS, // +
        MINUS, // -
        ASTERISK, // *
        SLASH, // /
        INCREMENT, // ++
        DECREMENT, // --
        EQUAL, // ==
        NOTEQUAL, // !=
        COMPARISON, // <, >, <=, >=
        NOT, // !
        ASSIGNMENT, // =
        MODULO, // %
        POWER, // **
        LOGIC_OR, // ||
        LOGIC_AND, // &&
        AMPERSAND, // &
        VERTICALBAR, // |
        QUOTE, // "
        LPAREN, // (
        RPAREN, // )
        LSQUAREBRACKET, // [
        RSQUAREBRACKET, // ]
        LCURLYBRACKET, // {
        RCURLYBRACKET, // }
        COMMA, // ,
        SEMICOLON, // ;
        DOT, // .
        INLINECOMMENT,
        BLOCKCOMMENTSTART,
        BLOCKCOMMENTEND
    };

    TokenType type;
    string value;

    Token(const TokenType&, const string&);
    string get_type();
};

class Lexer {
private:
    class Alphabet;
    class Dictionary;

    static Token extract_number(const string&, size_t&);
    static Token extract_identifier(const string&, size_t&);
    static Token extract_string(const string&, size_t&);
    static Token extract_metasequence(const string&, size_t&);

public:
    static bool blockcomment;
    static vector<Token> parse(const string&);
};
