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
#include <iomanip>
#include <vector>

#include "Object.h"
#include "Token.h"
#include "Input.h"
#include "Output.h"

#define LEXER_STATES  \
	S( INIT )        \
	S( COMMENT )     \
	S( IDENT )       \
	S( NUMBER )      \
	S( STRING )      \
	S( ESCAPED )     \
	S( UNKNOWN )

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
	
	Ref<Input>               _input;
	LexerState               _state;
	Char                     _current;
	Char                     _next;
	CharLocation             _tokenLocation;
	string                   _tokenString;
	CharLocation             _location;
	Token                    _token;
	
	Char                     current();
	Char                     next();
	Char                     advance();
	
	void                     append(char c) { _tokenString += c; }
	void                     append() { _tokenString += (char) _current; advance(); }
	Token                    endToken(Token::Type type)
	{
		_state = STATE_INIT;
		
		Token token(type, _tokenString, _tokenLocation);
		_tokenString = "";
		return token;
	}
	Token                    getToken();
	
	static const char *      getStateName(LexerState state);

public:
	Lexer(Ref<Input> input);
	virtual ~Lexer() { }
	
	Token                    peek();
	Token                    get();
	
	void                     dumpTokens(ostream& output);
	void                     dumpTokens(Ref<Output> output);
	void                     dumpTokens() { dumpTokens(std::cout); }
	void                     dumpState(ostream& output);
};

#endif /* end of include guard: LEXER_H_34VDFVER34FVA */

