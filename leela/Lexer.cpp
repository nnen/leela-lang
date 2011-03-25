/**
 * \file   leela/Lexer.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-25
 *
 * \brief  Implementation of the Lexer class.
 */

#include "Lexer.h"

void Lexer::init()
{
	_factories.push_back(new TokenFactory<Identifier>());
}

Lexer::Lexer(istream *input)
{
	if (input == NULL)
		throw exception();
	
	this->init();
	this->_input = input;
}

