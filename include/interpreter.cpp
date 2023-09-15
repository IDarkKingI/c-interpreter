#include "interpreter.hpp"


Interpreter::Interpreter(const string& file_name) : file_name(file_name) {}


void Interpreter::parse_source() {
    ifstream fin(file_name, ios::in);
    if (fin.is_open()) {
        string line;
        while (getline(fin, line)) {
            for (const auto& it : Lexer::parse(line))
                tokens.push_back(it);
        }
    }
    fin.close();
}


void Interpreter::parse_syntax() {
    ast_root = Syntaxer::parse(tokens);
}


void Interpreter::print_ast() const {
    Printer printer;
    ast_root->accept(printer);
}
