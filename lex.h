/*
 * lex.h
 *
 * CS280
 * Spring 2020
 */

#ifndef LEX_H_
#define LEX_H_

#include <string>
#include <iostream>
using std::string;
using std::istream;
using std::ostream;

enum Token {
		// keywords
	PRINT, PRINTLN, REPEAT, BEGIN, END,

		// an identifier
	IDENT,

		// an integer and string constant
	ICONST, SCONST,

		// the operators, parens, semicolon
	PLUS, MINUS, STAR, SLASH, EQ, LPAREN, RPAREN, SC,
		// any error returns this token
	ERR,

		// when completed (EOF), return this token
	DONE
};

class Tok {
	Token	token;
	string	lexeme;
	int	lnum;

public:
	Tok() {
		token = ERR;
		lnum = -1;
	}
	Tok(Token token, string lexeme, int line) {
		this->token = token;
		this->lexeme = lexeme;
		this->lnum = line;
	}

	bool operator==(const Token token) const { return this->token == token; }
	bool operator!=(const Token token) const { return this->token != token; }

	Token	GetToken() const { return token; }
	string	GetLexeme() const { return lexeme; }
	int	GetLinenum() const { return lnum; }
};

extern ostream& operator<<(ostream& out, const Tok& tok);

extern Tok getNextToken(istream& in, int& linenum);


#endif /* LEX_H_ */

