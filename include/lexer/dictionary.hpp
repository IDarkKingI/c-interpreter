#pragma once

#include "lexer.hpp"


class Lexer::Dictionary {
private:
    static const vector<string> keywords;
    static const vector<string> datatypes;
    static const vector<string> math_functions;
    static const vector<string> builtin_functions;

public:
    static const unordered_map<string, Token::TokenType> MetaSequenceAssossiations;

    static bool iskeyword(const string&);
    static bool isdatatype(const string&);
	static bool is_math_function(const string&);
	static bool is_builtin_function(const string&);
    static bool is_valid_metasequence(const string&);
};
