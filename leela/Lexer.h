/**
 * \file   leela/Lexer.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-23
 *
 * \brief  Header file for the Lexer class.
 */

#ifndef LEXER_H_34VDFVER34FVA
#define LEXER_H_34VDFVER34FVA

#include <iostream>
#include <vector>

#include "Token.h"

#define LEXER_STATES \
	S( INIT )       \
	S( IDENT )      \
	S( NUMBER )     \
	S( STRING )     \
	S( ESCAPED )    \
	S( UNKNOWN )

#include "Object.h"

using namespace std;

/**
 * Represents the lexical analyzer.
 */
class Lexer : public Object {
private:
	enum LexerState {
		#define S(name) STATE_##name,
		LEXER_STATES
		#undef S
		
		STATE_COUNT
	};
	
	istream                * _input;
	LexerState               _state;
	Char                     _current;
	Char                     _next;
	string                   _tokenString;
	
	Char                     current();
	Char                     next();
	Char                     advance();
	
	void                     append(char c) { _tokenString += c; }
	void                     append() { _tokenString += (char) _current; advance(); }
	Token                    endToken(Token::Type type)
	{
		_state = STATE_INIT;
		
		Token token(type, _tokenString);
		_tokenString = "";
		return token;
	}
	
	static const char *      getStateName(LexerState state);

public:
	Lexer(istream *input);
	virtual ~Lexer() { }
	
	Token                    getToken();
	
	void                     dumpState(ostream& output);
};

#endif /* end of include guard: LEXER_H_34VDFVER34FVA */

