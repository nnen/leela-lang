/**
 * \file   leela/Parser.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-29
 *
 * \brief  Implementation of the Parser class.
 */

#include <exception>

#include "Parser.h"
#include "grammar.h"

/* SYMBOLS ********************************************************************/

ostream& operator << (ostream& output, const Symbol& symbol)
{
	symbol.print(output);
	return output;
}

/* RULES **********************************************************************/

Ref<Rule> operator + (Ref<Rule> a, Ref<Rule> b)
{
	return new ChainRule(a, b);
}

Ref<Rule> operator | (Ref<Rule> a, Ref<Rule> b)
{
	return new ChoiceRule(a, b);
}

/* PARSER *********************************************************************/

Parser::Parser()
{
}

void Parser::parse(istream& input, ostream& output)
{
	_output = &output;
	_lexer = new Lexer(&input);

	while (!_stack.empty()) _stack.pop();
	while (!_nonterminals.empty()) _nonterminals.pop();
	
	_current = _lexer->getToken();
	
	// _stack.push(NonterminalRule<Program>::rule);
	_stack.push(new NonterminalRule<Program>());
	
	while (!_stack.empty()) {
		// dumpState(output);

		Ref<Symbol> top = _stack.top();
		_stack.pop();
		
		top->onPop(*this);
	}
}

void Parser::push(Ref<Symbol> symbol)
{
	_stack.push(symbol);
	symbol->onPush(*this);
}

void Parser::append(Ref<InputSymbol> symbol)
{
	if (!_nonterminals.empty())
		getNonterminal()->append(symbol);
}

void Parser::accept()
{
	append(new Terminal(_current));
	_current = _lexer->getToken();
}

void Parser::startNonterminal(Ref<Nonterminal> nonterminal)
{
	_nonterminals.push(nonterminal);
	// _auxiliary.push(vector<Ref<Symbol> >());
}

Ref<Nonterminal> Parser::getNonterminal()
{
	return _nonterminals.top();
}

/*
vector<Ref<Symbol> >& Parser::getMatched()
{
	return _auxiliary.top();
}
*/

void Parser::endNonterminal()
{
	if (_nonterminals.empty())
		throw std::exception();
	
	Ref<Nonterminal> nonterminal = getNonterminal();
	_nonterminals.pop();
	nonterminal->onFinished(*this);
	append(nonterminal);
}

void Parser::dumpState(ostream& output)
{
	output << "PARSER STATE:" << endl;
	output << "\tTOKEN: " << _current << endl;
	output << "\tSTACK SIZE: " << _stack.size() << endl;
	if (_stack.empty())
		output << "\tTOP OF STACK: - " << endl;
	else
		output << "\tTOP OF STACK: " << *(_stack.top()) << endl;
	if (_nonterminals.empty())
		output << "\tNONTERMINAL: - " << endl;
	else
		output << "\tNONTERMINAL: " << *(_nonterminals.top()) << endl;
}
