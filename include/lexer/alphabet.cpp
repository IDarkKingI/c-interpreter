#include "alphabet.hpp"


const string Lexer::Alphabet::literals = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
const string Lexer::Alphabet::digits = "0123456789";
const string Lexer::Alphabet::spaces = " \t\n";
const string Lexer::Alphabet::metachars = "+-*/=<>%!(){}[]&|,;.";


bool Lexer::Alphabet::isspace(const char& c) {
	return spaces.find(c) != string::npos;
}


bool Lexer::Alphabet:: isliteral(const char& c) {
	return literals.find(c) != string::npos;
}


bool Lexer::Alphabet:: isdigit(const char& c) {
	return digits.find(c) != string::npos;
}


bool Lexer::Alphabet:: ismetachar(const char& c) {
	return metachars.find(c) != string::npos;
}


bool Lexer::Alphabet::isdot(const char& c){
	return c == '.';
}


bool Lexer::Alphabet::isquote(const char& c){
	return c == '\"';
}


bool Lexer::Alphabet::isalphanumeric(const char& c) {
	return isliteral(c) || isdigit(c);
}
