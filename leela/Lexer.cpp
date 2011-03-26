/**
 * \file   leela/Lexer.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-25
 *
 * \brief  Implementation of the Lexer class.
 */

#include "leela.h"
#include "Lexer.h"

void Lexer::init()
{
	_factories.push_back(new TokenFactory<Identifier>());
	_factories.push_back(new TokenFactory<StringLiteral>());
}

Char Lexer::getChar()
{
	assert(_input != NULL);
	_current = Char(_current, _input->get());
	return _current;
}

Token Lexer::getToken()
{
	while (getChar().isChar()) {
		foreach (it, _factories) {
			if ((*it)->advance()) {
				Token result = (*it)->getToken();
				resetToken();
				return result;
			}
		}
	}
	return EndToken();
}

void Lexer::resetToken()
{
	foreach(it, _factories)
		(*it)->reset();
}


Lexer::Lexer(istream *input)
{
	if (input == NULL)
		throw exception();
	
	this->init();
	this->_input = input;
}

