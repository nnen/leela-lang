/**
 * \file   leela/Parser.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-29
 *
 * \brief  Header file for the Parser class.
 */

#ifndef PARSER_H_23VERTBYU8N
#define PARSER_H_23VERTBYU8N

#include "Lexer.h"

class SyntaxSymbol {
};

class Rule {
public:
	virtual void advance(Parser& parser) = 0;
	
	Rule& operator + (const Rule& other) const;
	Rule& operator | (const Rule& other) const;
};

class Terminal : Rule {
public:
	virtual void advance(Parser& parser);
};

class Chain : Rule {
};

class Choice : Rule {
};

class Parser {
private:

public:
	Parser();
	virtual ~Parser();
};

#endif /* end of include guard: PARSER_H_23VERTBYU8N */

