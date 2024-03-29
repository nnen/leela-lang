/**
 * \file   leela/Char.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-25
 *
 * \brief  Header file for the Char class.
 */

#ifndef CHAR_H_SC3455VFFEVB
#define CHAR_H_SC3455VFFEVB

#include <ostream>

struct Char {
public:
	int value;
	int column;
	int line;
	
	Char();
	Char(int value);
	Char(Char previous, int value);
	
	operator char() const;
	bool operator == (char other) const;
	
	bool hasPosition() const;
	
	bool isChar() const;
	bool isLetter() const;
	bool isDigit() const;
	bool isWhitespace() const;
	bool isNewLine() const;
	bool isSingleDelimiter() const;
	bool isDoubleDelimiter() const;
	bool isDelimiter() const;
};

std::ostream& operator << (std::ostream& output, const Char& c);

#endif /* end of include guard: CHAR_H_SC3455VFFEVB */

