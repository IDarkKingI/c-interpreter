#pragma once


#include "../lexer/lexer.hpp"
#include "tree.hpp"


class Syntaxer {
public:
    static unique_ptr<AST> parse(const vector<Token>&);

private:
    static int level;
	static const unordered_map<string, int> precedences;

    static vector<unique_ptr<Declaration>> parse_declaration(const vector<Token>&, size_t&);
    static vector<unique_ptr<VarDeclaration>> parse_var_declaration(const vector<Token>&, size_t&);
    static vector<unique_ptr<VarDeclaration>> parse_func_args(const vector<Token>&, size_t&);
    static unique_ptr<FuncDeclaration> parse_func_declaration(const vector<Token>&, size_t&);

    static unique_ptr<Block> parse_block(const vector<Token>&, size_t&);
    static unique_ptr<Conditional> parse_conditional_statement(const vector<Token>&, size_t&);
    static unique_ptr<Loop> parse_loop_statement(const vector<Token>&, size_t&);
    static unique_ptr<Return> parse_return_statement(const vector<Token>&, size_t&);
    static unique_ptr<Jump> parse_jump_statement(const vector<Token>&, size_t&);

    static unique_ptr<Expr> parse_binary_expression(const vector<Token>&, size_t&, const int& = 0);
	static unique_ptr<Expr> parse_simple_expression(const vector<Token>&, size_t&);
	static vector<unique_ptr<Expr>> parse_interior(const vector<Token>&, size_t&);
	static void expect_token(const Token::TokenType&, const vector<Token>&, size_t&);

};

