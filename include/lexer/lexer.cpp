#include "lexer.hpp"
#include "alphabet.hpp"
#include "dictionary.hpp"


bool Lexer::blockcomment = false;
Token::Token(const TokenType& type, const string& value) : type(type), value(value) {}


Token Lexer::extract_number(const string& input, size_t& pos) {
	string number;
	auto len = input.length();
	for (auto c = input[pos]; pos < len && Alphabet::isdigit(c); number.push_back(c), c = input[++pos]);
	if (Alphabet::isdot(input[pos])) {
		number.push_back(input[pos++]);
		for (auto c = input[pos]; pos < len && Alphabet::isdigit(c); number.push_back(c), c = input[++pos]);
	}
	return Token(Token::NUMBER, number);
}


Token Lexer::extract_identifier(const string& input, size_t& pos) {
    string identifier;
	auto len = input.length();
	for (; pos < len; pos++) {
		auto c = input[pos];
		if (Alphabet::isliteral(c))
			identifier.push_back(c);
		else break;
	}

	if (Dictionary::iskeyword(identifier))
		return Token(Token::KEYWORD, identifier);
	else if (Dictionary::isdatatype(identifier))
		return Token(Token::DATATYPE, identifier);
	else if (Dictionary::is_math_function(identifier))
		return Token(Token::MATH_FUNC, identifier);
	else if (Dictionary::is_builtin_function(identifier))
		return Token(Token::BUILTIN_FUNC, identifier);
	return Token(Token::IDENTIFIER, identifier);
}


Token Lexer::extract_string(const string& input, size_t& pos) {
	pos++;
	string str;
	auto len = input.length();
	for(; pos < len; pos++) {
		auto c = input[pos];
		if (Alphabet::isquote(c)) {
			pos++;
			break;
		}
		str.push_back(c);
	}
	return Token(Token::STRING, str);
}


Token Lexer::extract_metasequence(const string& input, size_t& pos){
	string metasequence;
	auto len = input.length();
	for (auto c = input[pos]; pos < len && Alphabet::ismetachar(c); c = input[pos]) {
		metasequence.push_back(c);
		if (!Dictionary::is_valid_metasequence(metasequence)) {
			metasequence.pop_back();
			break;
		}
		pos++;
	}
	return Token(Dictionary::MetaSequenceAssossiations.at(metasequence), metasequence);
}



vector<Token> Lexer::parse(const string& input) {
    vector<Token> tokens;
    size_t pos = 0, len = input.length();

	while (pos < len) {
        auto c = input[pos];
        if (Alphabet::isspace(c))
            pos++;
		else if (c == '\'') {
			pos++;
			tokens.push_back(Token(Token::CHAR, string(1, input[pos])));
			pos += 2;
		} else if (Alphabet::isdigit(c)) {
			auto number = extract_number(input, pos);
			tokens.push_back(number);
		} else if (Alphabet::isliteral(c)) {
			auto identifier = extract_identifier(input, pos);
			tokens.push_back(identifier);
		} else if (Alphabet::ismetachar(c)) {
            auto metasequence = extract_metasequence(input, pos);
			tokens.push_back(metasequence);
		} else if (Alphabet::isquote(c)) {
			tokens.push_back(extract_string(input, pos));
		 } else
            throw runtime_error("Invalid char: " + to_string(c));
    }

    return tokens;
}
