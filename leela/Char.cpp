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
	
	if (previous.hasPosition()) {
		column = previous.column + 1;
		line = previous.line;
	
		if (isNewLine()) {
			column = 0;
			line++;
		}
	} else {
		column = 0;
		line = 0;
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

bool Char::hasPosition() const
{
	return ((column >= 0) && (line >= 0));
}

bool Char::isChar() const
{
	return (value >= 0);
}

bool Char::isLetter() const
{
	return (
		(
			(value >= 'a') &&
			(value <= 'z')
		) ||
		(
			(value >= 'A') &&
			(value <= 'Z')
		)
	);
}

bool Char::isDigit() const
{
	return ((value >= '0') &&
	        (value <= '9'));
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

bool Char::isSingleDelimiter() const
{
	return (
		#define TD(name, ch) (value == (ch)) ||
		#include "token_types.h"
		false
	);
}

bool Char::isDoubleDelimiter() const
{
	return (
		#define T2(name, ch1, ch2) (value == (ch1)) ||
		#include "token_types.h"
		false
	);
}

bool Char::isDelimiter() const
{
	if (isSingleDelimiter() || isDoubleDelimiter())
		return true;
	
	return (
		isWhitespace() ||
		isNewLine()    ||
		(!isChar())
	);
}

std::ostream& operator << (std::ostream& output, const Char& c)
{
	if (c.isChar())
		output << (char)c;
	else
		output << "<EOF>";
	return output;
}

