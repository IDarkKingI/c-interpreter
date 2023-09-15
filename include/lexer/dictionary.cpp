#include "dictionary.hpp"


const vector<string> Lexer::Dictionary::keywords = {
    "if",
    "elif",
    "else",
    "while",
    "main",
    "return",
    "continue",
    "break"
};

const vector<string> Lexer::Dictionary::datatypes = {
    "void",
    "bool",
    "int",
    "double",
    "char",
    "string"
};

const vector<string> Lexer::Dictionary::math_functions = {
	"pow",
	"exp",
	"log",
	"sin",
	"cos"
};

const vector<string> Lexer::Dictionary::builtin_functions = {
    "read",
    "print",
    "abs",
    "sgn"
};

const unordered_map<string, Token::TokenType> Lexer::Dictionary::MetaSequenceAssossiations = {
    {"+", Token::PLUS},
    {"-", Token::MINUS},
    {"*", Token::ASTERISK},
    {"/", Token::SLASH},
    {"++", Token::INCREMENT},
    {"--", Token::DECREMENT},
    {"==", Token::EQUAL},
    {"!=", Token::NOTEQUAL},
    {">", Token::COMPARISON},
    {">=", Token::COMPARISON},
    {"<", Token::COMPARISON},
    {"<=", Token::COMPARISON},
    {"=", Token::ASSIGNMENT},
    {"!", Token::NOT},
    {"%", Token::MODULO},
    {"**", Token::POWER},
    {"||", Token::LOGIC_OR},
    {"&&", Token::LOGIC_AND},
    {"&", Token::AMPERSAND},
    {"|", Token::VERTICALBAR},
    {"\"", Token::QUOTE},
    {"(", Token::LPAREN},
    {")", Token::RPAREN},
    {"[", Token::LSQUAREBRACKET},
    {"]", Token::RSQUAREBRACKET},
    {"{", Token::LCURLYBRACKET},
    {"}", Token::RCURLYBRACKET},
    {",", Token::COMMA},
    {".", Token::DOT},
    {";", Token::SEMICOLON}
};


bool Lexer::Dictionary::iskeyword(const string& token) {
    return find(keywords.begin(), keywords.end(), token) != keywords.end();
}


bool Lexer::Dictionary::isdatatype(const string& token) {
    return find(datatypes.begin(), datatypes.end(), token) != datatypes.end();
}


bool Lexer::Dictionary::is_math_function(const string& token) {
    return find(math_functions.begin(), math_functions.end(), token) != math_functions.end();
}


bool Lexer::Dictionary::is_builtin_function(const string& token) {
    return find(builtin_functions.begin(), builtin_functions.end(), token) != builtin_functions.end();
}


bool Lexer::Dictionary::is_valid_metasequence(const string& token) {
    return MetaSequenceAssossiations.find(token) != MetaSequenceAssossiations.end();
}
