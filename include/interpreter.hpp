#pragma once


#include "lexer/lexer.hpp"
#include "syntaxer/syntaxer.hpp"
#include "visitor.hpp"
#include <fstream>


using namespace std;


class Interpreter {
private:
    string file_name;
    string source_code;
    vector<Token> tokens;
    unique_ptr<AST> ast_root;

public:
    Interpreter(const string&);

    void parse_source();
    void parse_syntax();
    void print_ast() const;
};

