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
public:
	virtual ~ITokenFactory() {}
	virtual Token* advance(Char c) = 0;
	virtual void   reset() = 0;
};

template<class T>
class TokenFactory : public ITokenFactory {
private:
	Token * _token;

public:
	TokenFactory()
	{
		_token = (Token*) new T();
	}
	
	virtual ~TokenFactory()
	{
		delete _token;
		_token = NULL;
	}
	
	virtual Token* advance(Char c)
	{
		if (!_token->advance(c))
			return NULL;
		
		Token* result = _token;
		_token = (Token*) new T();
		return result;
	}
	
	virtual void reset()
	{
		_token->reset();
	}
};

/**
 * Represents the lexical analyzer.
 */
class Lexer {
private:
	istream               * _input;
	vector<ITokenFactory*>   _factories;
	
	Char             _current;
	
	void             init();

public:
	Lexer(istream *input);
	virtual ~Lexer();
	
	virtual Token& advance();
};

#endif /* end of include guard: LEXER_H_34VDFVER34FVA */

