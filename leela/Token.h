/**
 * \file   leela/Token.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-24
 *
 * \brief  Header file for the Token class and derived classes.
 */

#ifndef TOKEN_H_434634DFBDV
#define TOKEN_H_434634DFBDV

#include <string>

#include "Char.h"

#define FIRST_STATE (__LINE__ * 100)

using namespace std;

typedef int TokenState;

class Token {
protected:
	enum { STATE_INIT = FIRST_STATE, STATE_FINISHED, STATE_ERROR };
	
	TokenState _state;

public:
	virtual ~Token() { }
	
	virtual bool advance(Char c);
	virtual void reset();
};

class EndToken : public Token {
};

class StringToken : public Token {
protected:
	string _string;
	void append(Char c);

public:
	virtual ~StringToken() { }
};

class Identifier : public StringToken {
protected:
	enum { STATE_STARTED = FIRST_STATE };

public:
	virtual ~Identifier() { }
	
	virtual bool advance(Char c);
	virtual void reset();
};

class StringLiteral : public StringToken {
private:
	enum { STATE_INSIDE = FIRST_STATE, STATE_ESCAPED };

public:
	static const char QUOTE;
	static const char ESCAPE;
	
	virtual ~StringLiteral() { }

	virtual bool advance(Char c);
	virtual void reset();
};

#endif /* end of include guard: TOKEN_H_434634DFBDV */

