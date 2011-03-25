/**
 * \file   leela/Char.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-25
 *
 * \brief  Implementation of the Char class.
 */

#include "Char.h"

Char::Char()
{
	value = -1;
	column = -1;
	line = -1;
}

Char::Char(int value)
{
	this->value = value;
	column = -1;
	line = -1;
}

Char::Char(Char previous, int value)
{
	this->value = value;
	column = previous.column + 1;
	line = previous.line;
	
	if (isNewLine()) {
		column = 0;
		line++;
	}
}

Char::operator char() const
{
	if (!isChar()) return '\0';
	return (char)value;
}

bool Char::operator == (char other) const
{
	if (!isChar()) return false;
	return ((char)value == other);
}

bool Char::isChar() const
{
	return (value >= 0);
}

bool Char::isLetter() const
{
	return false;
}

bool Char::isDigit() const
{
	return false;
}

bool Char::isWhitespace() const
{
	switch (value) {
	case ' ':
	case '\n':
	case '\t':
		return true;
	}
	return false;
}

bool Char::isNewLine() const
{
	return (value == '\n');
}

