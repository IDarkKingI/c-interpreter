#include "syntaxer.hpp"


int Syntaxer::level = 1;


unique_ptr<AST> Syntaxer::parse(const vector<Token>& tokens) {
    unique_ptr<AST> root = make_unique<AST>();
    size_t pos = 0;
    while (pos < tokens.size()) {
        auto declaration_list = parse_declaration(tokens, pos);
        for (auto& it : declaration_list)
            root->declarations.push_back(move(it));
    }
    
    return root;
}


vector<unique_ptr<Declaration>> Syntaxer::parse_declaration(const vector<Token>& tokens, size_t& pos) {
    vector<unique_ptr<Declaration>> declarations;
    
    if (tokens[pos].type == Token::DATATYPE && (tokens[pos + 1].type == Token::IDENTIFIER || (tokens[pos + 1].type == Token::KEYWORD && tokens[pos + 1].value == "main")) && tokens[pos + 2].type == Token::LPAREN) {
        declarations.push_back(parse_func_declaration(tokens, pos));
        return declarations;
    } else if (tokens[pos].type == Token::DATATYPE && tokens[pos + 1].type == Token::IDENTIFIER) {
        auto vardecl = parse_var_declaration(tokens, pos);
        for (auto& it : vardecl)
            declarations.push_back(unique_ptr<VarDeclaration>(move(it)));
        return declarations;
    } else {
        throw runtime_error("Syntaxer::parse_declaration(): Invalid declaration");
    }
}


vector<unique_ptr<VarDeclaration>> Syntaxer::parse_var_declaration(const vector<Token>& tokens, size_t& pos) {
    vector<unique_ptr<VarDeclaration>> declarations;
    Token datatype = tokens[pos++];

    while (tokens[pos].type != Token::SEMICOLON) {
        if (tokens[pos].type == Token::COMMA) {
            pos++;
            continue;
        }
        
        if (tokens[pos].type == Token::IDENTIFIER &&
            tokens[pos + 1].type == Token::ASSIGNMENT) {
                auto var_name = tokens[pos].value;
                pos += 2;
                declarations.push_back(make_unique<VarDeclaration>(datatype.value, var_name, parse_binary_expression(tokens, pos)));
        } else if (tokens[pos].type == Token::IDENTIFIER) {
            declarations.push_back(make_unique<VarDeclaration>(datatype.value, tokens[pos++].value));
        }
        else
            throw runtime_error("Syntaxer::parse_var_declaration(): Invalid variable declaration");
    }
    expect_token(Token::SEMICOLON, tokens, pos);
    return declarations;
}


vector<unique_ptr<VarDeclaration>> Syntaxer::parse_func_args(const vector<Token>& tokens, size_t& pos) {
    vector<unique_ptr<VarDeclaration>> args;
    while (tokens[pos].type != Token::RPAREN) {
        if (tokens[pos].type == Token::COMMA)
            pos++;
        if (tokens[pos].type == Token::DATATYPE && tokens[pos + 1].type == Token::IDENTIFIER) {
            string type = tokens[pos++].value, name = tokens[pos++].value;
            args.push_back(make_unique<VarDeclaration>(type, name));
        }
    }

    return args;
}


unique_ptr<FuncDeclaration> Syntaxer::parse_func_declaration(const vector<Token>& tokens, size_t& pos) {
    string return_type = tokens[pos++].value;
    string func_name = tokens[pos++].value;
    
    expect_token(Token::LPAREN, tokens, pos);
    auto args = parse_func_args(tokens, pos);
    expect_token(Token::RPAREN, tokens, pos);
    unique_ptr<FuncProt> prot = make_unique<FuncProt>(return_type, func_name, move(args));

    if (tokens[pos].type == Token::LCURLYBRACKET) {
        pos++;
        return make_unique<FuncDef>(move(prot), parse_block(tokens, pos));
    } else if (tokens[pos].type == Token::SEMICOLON) {
        pos++;
        return prot;
    } else
        throw runtime_error("Syntaxer::parse_func_declaration(): Invalid function declaration");
}


unique_ptr<Block> Syntaxer::parse_block(const vector<Token>& tokens, size_t& pos) {
    level++;
    vector<unique_ptr<Statement>> statements;

    while (tokens[pos].type != Token::RCURLYBRACKET) {
        if (tokens[pos].type == Token::IDENTIFIER && tokens[pos + 1].type == Token::ASSIGNMENT) {
            unique_ptr<Expr> expr = parse_binary_expression(tokens, pos);
            statements.push_back(move(expr));
            if (tokens[pos - 1].type != Token::RCURLYBRACKET)
                expect_token(Token::SEMICOLON, tokens, pos);
            continue;
        }
        if (tokens[pos].type == Token::DATATYPE && tokens[pos + 1].type == Token::IDENTIFIER) {
            auto declaration_list = parse_var_declaration(tokens, pos);
            for (auto& it : declaration_list)
                statements.push_back(unique_ptr<VarDeclaration>(move(it)));
        } else if (tokens[pos].type == Token::KEYWORD && (tokens[pos].value == "if" || tokens[pos].value == "elif" || tokens[pos].value == "else")) {
            statements.push_back(parse_conditional_statement(tokens, pos));
        } else if (tokens[pos].type == Token::KEYWORD && tokens[pos].value == "while") {
            statements.push_back(parse_loop_statement(tokens, pos));
        } else if (tokens[pos].type == Token::KEYWORD && tokens[pos].value == "return") {
            pos++;
            auto ret = parse_return_statement(tokens, pos);
            if (tokens[pos - 1].type != Token::RCURLYBRACKET)
                expect_token(Token::SEMICOLON, tokens, pos);
            statements.push_back(move(ret));
        } else if (tokens[pos].type == Token::KEYWORD && (tokens[pos].value == "break" || tokens[pos].value == "continue")) {
            auto jump = parse_jump_statement(tokens, pos);
            if (tokens[pos - 1].type != Token::RCURLYBRACKET)
                expect_token(Token::SEMICOLON, tokens, pos);
            statements.push_back(move(jump));
        } else {
            statements.push_back(parse_binary_expression(tokens, pos));
            if (tokens[pos - 1].type != Token::RCURLYBRACKET)
                expect_token(Token::SEMICOLON, tokens, pos);
        }
    }

    expect_token(Token::RCURLYBRACKET, tokens, pos);
    return make_unique<Block>(move(statements), --level);
}


unique_ptr<Conditional> Syntaxer::parse_conditional_statement(const vector<Token>& tokens, size_t& pos) {
    string keyword = tokens[pos++].value;
    vector<unique_ptr<Statement>> statement_list;

    if (keyword == "if" || keyword == "elif") {
        expect_token(Token::LPAREN, tokens, pos);
        unique_ptr<Expr> condition = parse_binary_expression(tokens, pos);
        expect_token(Token::RPAREN, tokens, pos);
        expect_token(Token::LCURLYBRACKET, tokens, pos);
        return make_unique<Conditional>(keyword == "if" ? Conditional::IF : Conditional::ELIF, move(condition), parse_block(tokens, pos));

    } else if (keyword == "else") {
        expect_token(Token::LCURLYBRACKET, tokens, pos);
        return make_unique<Conditional>(Conditional::ELSE, parse_block(tokens, pos));
    }
    
    throw runtime_error("Syntaxer::parse_conditional_statement(): Invalid conditional statement");
}


unique_ptr<Loop> Syntaxer::parse_loop_statement(const vector<Token>& tokens, size_t& pos) {
    string keyword = tokens[pos++].value;
    vector<unique_ptr<Statement>> statement_list;
    
    expect_token(Token::LPAREN, tokens, pos);
    unique_ptr<Expr> condition = parse_binary_expression(tokens, pos);
    expect_token(Token::RPAREN, tokens, pos);
    expect_token(Token::LCURLYBRACKET, tokens, pos);
    return make_unique<Loop>(move(condition), parse_block(tokens, pos));
}


unique_ptr<Return> Syntaxer::parse_return_statement(const vector<Token>& tokens, size_t& pos) {
    return make_unique<Return>(parse_binary_expression(tokens, pos));
}


unique_ptr<Jump> Syntaxer::parse_jump_statement(const vector<Token>& tokens, size_t& pos) {
    return make_unique<Jump>(tokens[pos++].value == "break" ? Jump::BREAK : Jump::CONTINUE);
}


const unordered_map<string, int> Syntaxer::precedences = {
    {"=", 1},
    {"||", 2},
    {"&&", 3},
    {"==", 4}, {"!=", 4},
    {"<", 5}, {"<=", 5}, {">", 5}, {">=", 5},
    {"+", 6}, {"-", 6},
    {"*", 7}, {"/", 7}, {"%", 7},
    {"**", 8},
};


unique_ptr<Expr> Syntaxer::parse_binary_expression(const vector<Token>& tokens, size_t& pos, const int& min_precedence) {
	auto left = parse_simple_expression(tokens, pos);
	for (auto token = tokens[pos]; token.type != Token::COMMA || token.type != Token::SEMICOLON || token.type != Token::RPAREN; token = tokens[pos]) {
		auto it = precedences.find(token.value);
		if (it == precedences.end() || it -> second < min_precedence)
			break;
		auto right = parse_binary_expression(tokens, ++pos, it -> second);
        vector<unique_ptr<Expr>> branches;
        branches.push_back(move(left));
        branches.push_back(move(right));
        left = make_unique<Expr>(Expr::BINARY_OP, token.value, move(branches));

	}

	return left;
}


unique_ptr<Expr> Syntaxer::parse_simple_expression(const vector<Token>& tokens, size_t& pos) {
	auto token = tokens[pos++];
	if (token.type == Token::NUMBER) {
		return make_unique<Expr>(Expr::CONST, token.value);
    } else if (token.type == Token::STRING) {
        return make_unique<Expr>(Expr::STRING, token.value);
    } else if (token.type == Token::CHAR) {
        return make_unique<Expr>(Expr::CHAR, token.value);
    }  else if (token.type == Token::IDENTIFIER) {
		if (tokens[pos].type == Token::LPAREN) {
			vector<unique_ptr<Expr>> args = parse_interior(tokens, pos);
			return make_unique<Expr>(Expr::FUNC, token.value, move(args));
		} else
			return make_unique<Expr>(Expr::VAR, token.value);
	} else if (token.type == Token::BUILTIN_FUNC) {
		if (tokens[pos].type == Token::LPAREN) {
			vector<unique_ptr<Expr>> args = parse_interior(tokens, pos);
			return make_unique<Expr>(Expr::BUILTIN_FUNC, token.value, move(args));
		}
	} else if (token.type == Token::MATH_FUNC) {
		return make_unique<Expr>(Expr::MATH_FUNC, token.value, parse_interior(tokens, pos));
	} else if (token.type == Token::PLUS || token.type == Token::MINUS || token.type == Token::INCREMENT || token.type == Token::DECREMENT || token.type == Token::NOT) {
        vector<unique_ptr<Expr>> simple_expr;
        simple_expr.push_back(parse_simple_expression(tokens, pos));
		return make_unique<Expr>(Expr::PRE_UNARY_OP, token.value, move(simple_expr));
	} else if (token.type == Token::LPAREN) {
		auto node = parse_binary_expression(tokens, pos);
		expect_token(Token::RPAREN, tokens, pos);
		return node;
	} else
    	throw runtime_error("Syntaxer::parse_simple_expression(): Invalid token: " + token.value + " (pos: " + to_string(pos) + ")");
}


vector<unique_ptr<Expr>> Syntaxer::parse_interior(const vector<Token>& tokens, size_t& pos) {
	vector<unique_ptr<Expr>> args;
	expect_token(Token::LPAREN, tokens, pos);
    
    if (tokens[pos].type == Token::RPAREN) {
        expect_token(Token::RPAREN, tokens, pos);
        return args;
    }

	pos--;
	while (1) {
		pos++;
		args.push_back(parse_binary_expression(tokens, pos));
		if (tokens[pos].type == Token::COMMA)
			continue;
		expect_token(Token::RPAREN, tokens, pos);
		break;
	}

	return args;
}


void Syntaxer::expect_token(const Token::TokenType& expected_type, const vector<Token>& tokens, size_t& pos) {
	auto token = tokens[pos];
	if (token.type == expected_type)
		pos++;
	else
		throw runtime_error("Syntaxer::expect_token(): Unexpected token: " + tokens[pos].value);
}
