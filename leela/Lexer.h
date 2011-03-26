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

using namespace std;

class ITokenFactory {
protected:
	Token _token;

public:
	virtual      ~ITokenFactory() {}
	virtual bool  advance(Char c) { return _token.advance(c); }
	virtual void  reset() = 0;
	Token         getToken() { return _token; }
};

template<class T>
class TokenFactory : public ITokenFactory {
public:
	TokenFactory()
	{
		_token = T();
	}
	
	virtual ~TokenFactory() {}
	
	virtual void reset() { _token = T(); }
};

/**
 * Represents the lexical analyzer.
 */
class Lexer {
private:
	istream                * _input;
	Char                     _current;
	vector<Char>             _buffer;
	vector<ITokenFactory*>   _factories;
	
	void                     init();
	Char                     peekChar();
	Char                     getChar();
	Token                    getToken();
	void                     resetToken();

public:
	Lexer(istream *input);
	virtual ~Lexer();
	
	class Input {
	private:
		Lexer * _lexer;
	
	public:
		Input(Lexer &lexer);
		~Input();
	};
};

#endif /* end of include guard: LEXER_H_34VDFVER34FVA */

