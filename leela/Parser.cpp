/**
 * \file   leela/Parser.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-29
 *
 * \brief  Implementation of the Parser class.
 */

#include "Parser.h"

/* RULE ***********************************************************************/

void Rule::printWithName(ostream& output) const
{
	output << _name << " -> ";
	print(output);
}

Ref<Rule> operator + (Ref<Rule> a, Ref<Rule> b)
{
	return new Chain(a, b);
}

Ref<Rule> operator + (Token::Type a, Ref<Rule> b)
{
	return new Chain(new Terminal(a), b);
}

Ref<Rule> operator | (Ref<Rule> a, Ref<Rule> b)
{
	return new Choice(a, b);
}

Ref<Rule> operator | (Token::Type a, Ref<Rule> b)
{
	return new Choice(new Terminal(a), b);
}

/* RULE PROXY *****************************************************************/

RuleProxy& RuleProxy::operator = (Ref<Rule> rule)
{
	_rule = rule;
	return *this;
}

/* TERMINAL *******************************************************************/

vector<Token::Type> Terminal::getFirst() const
{
	vector<Token::Type> v;
	v.push_back(_type);
	return v;
}

void Terminal::advance(Parser& parser)
{
	if (parser.current().type != _type)
		// TODO: Handle syntax error
	parser.accept();
}

void Terminal::print(ostream& output) const
{
	output << Token::getTypeName(_type);
}

/* CHAIN **********************************************************************/

void Chain::build()
{
	_a->build();
	_b->build();
}

vector<Token::Type> Chain::getFirst() const
{
	return _a->getFirst();
}

void Chain::advance(Parser& parser)
{
	parser.push(_b); parser.push(_a);
}

void Chain::print(ostream& output) const
{
	_a->print(output);
	output << " ";
	_b->print(output);
}

/* CHOICE *********************************************************************/

void Choice::build()
{
	foreach(it, _a->getFirst())
		_table[*it] = _a;
	
	foreach(it, _b->getFirst())
		_table[*it] = _b;
}

vector<Token::Type> Choice::getFirst() const
{
	vector<Token::Type> v = _a->getFirst();

	foreach(it, _b->getFirst())
		v.push_back(*it);

	return v;
}

void Choice::advance(Parser& parser)
{
	if (_table.count(parser.current().type) < 1)
		// TODO: Handle syntax error
	
	parser.push(_table[parser.current().type]);
}

void Choice::print(ostream& output) const
{
	_a->print(output);
	output << " | ";
	_b->print(output);
}

/* PARSER *********************************************************************/

void Parser::initGrammar()
{
	Ref<RuleProxy> S;

	S = Token::KW_BEGIN + Token::KW_END;

	S->build();
	
	_startingSymbol = S;
}

Parser::Parser()
{
	initGrammar();
}

void Parser::parse(istream& input)
{
	_lexer = new Lexer(&input);
}


