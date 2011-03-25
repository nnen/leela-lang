/**
 * \file   leela/Token.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-24
 *
 * \brief  Implementation of the Token class and derived classes.
 */

#include "Token.h"

/* ************************************************************
 * Token
 **************************************************************/

bool Token::advance(Char c)
{
	switch (_state) {
	case STATE_FINISHED:
		return true;
	case STATE_ERROR:
	default:
		return false;
	}
}

void Token::reset()
{
	_state = STATE_INIT;
}

void StringToken::append(Char c)
{
	if (!c.isChar()) return;
	_string += (char)c;
}

bool Identifier::advance(Char c)
{
	switch (_state) {
	case STATE_INIT:
		if (c.isLetter()) {
			append(c);
			_state = STATE_STARTED;
		} else if (!c.isWhitespace()) {
			_state = STATE_ERROR;
		}
		break;
	case STATE_STARTED:
		if (c.isLetter() || c.isDigit()) {
			append(c);
		} else if (c.isWhitespace()) {
			return true;
		} else {
			_state = STATE_ERROR;
		}
		break;
	default:
		return Token::advance(c);
	}
	return false;
}

const char StringLiteral::QUOTE = '"';
const char StringLiteral::ESCAPE = '\\';

bool StringLiteral::advance(Char c)
{
	switch (_state) {
	case STATE_INIT:
		if (c == QUOTE) {
			_state = STATE_INSIDE;
		} else if (!c.isWhitespace()) {
			_state = STATE_ERROR;
		}
		break;
	case STATE_INSIDE:
		if (c == ESCAPE) {
			_state = STATE_ESCAPED;
		} else if (c == QUOTE) {
			return true;
		} else {
			append(c);
		}
		break;
	case STATE_ESCAPED:
		switch (c.value) {
		case 'n': append(Char('\n')); break;
		case 'r': append(Char('\r')); break;
		case 't': append(Char('\t')); break;
		default:  append(c);         break;
		}
		_state = STATE_INSIDE;
		break;
	default:
		return Token::advance(c);
	}
	return false;
}



