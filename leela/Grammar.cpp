/**
 * \file   Grammar.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-20
 *
 * \brief  
 */

#include <queue>
#include <sstream>
#include <iostream>

#include "Grammar.h"

NonterminalDef& Grammar::definition(int line, string name)
{
	if (_nonterminals.count(name) > 0)
		return *_nonterminals[name];
	
	Ref<NonterminalDef> def = new NonterminalDef(this, line, name);
	_nonterminals[name] = def;
		
	if (_startingSymbol.isNull())
		_startingSymbol = def;
	
	return *def;
}

Ref<GSymbol> Grammar::terminal(int line, Token::Type terminal)
{
	return new Terminal(this, line, terminal);
}

Ref<GSymbol> Grammar::epsilon(int line)
{
	return new Terminal(this, line);
}

Ref<GSymbol> Grammar::nonterminal(int line, string name)
{
	return new NonterminalRef(this, line, name);
}

void Grammar::init()
{
	#define DEF(name) definition(__LINE__, #name)
	#define T(name)   terminal(__LINE__, Token::name)
	#define E         epsilon(__LINE__)
	#define N(name)   nonterminal(__LINE__, #name)
	
	DEF(Program)           = N(Preamble) + N(CompoundStatement);
	DEF(Preamble)          = T(KW_VAR) + N(IdentList) + T(SEMICOLON);
	DEF(Statement)         = N(CompoundStatement) | N(IfStatement);
	DEF(CompoundStatement) = T(KW_BEGIN) + N(StatementList) + T(KW_END);
	DEF(StatementList)     = (N(Statement) + N(StatementListRest)) | E;
	DEF(StatementListRest) = (T(SEMICOLON) + N(Statement) + N(StatementListRest)) | E;
	DEF(IfStatement)       = T(KW_IF) + N(Expression) + T(KW_THEN) + N(Statement) + T(KW_ELSE) + N(Statement);
	DEF(IdentList)         = T(IDENTIFIER) + N(IdentListRest);
	DEF(IdentListRest)     = (T(COMMA) + T(IDENTIFIER) + N(IdentListRest)) | E;
	
	DEF(Expression)        = T(NUMBER_LITERAL);
	
	#undef DEF
	#undef T
	#undef N
}

void Grammar::discover()
{
	queue<Ref<GSymbol> > toAdd;
	
	toAdd.push(_startingSymbol->getSymbol());
	
	_allSymbols.clear();
	_sortedSymbols.clear();
	
	while (!toAdd.empty()) {
		Ref<GSymbol> symbol = toAdd.front();
		toAdd.pop();

		cerr << symbol << " ";
		
		if (_allSymbols.count(symbol) > 0)
			continue;
		
		_allSymbols.insert(symbol);
		_sortedSymbols.push_back(symbol);
		GSymbol::Set children = symbol->getChildren();
		cerr << "children(" << children.size() << ") ";
		foreach (child, children) {
			cerr << "child(" << *child << ") ";
			toAdd.push(*child);
		}
	}
	
	cerr << endl;
}

void Grammar::check()
{
	foreach (item, _nonterminals)
		item->second->getSymbol()->checkRecursion();
	
	foreach (item, _nonterminals)
		if (item->second->getSymbol()->isRecursive())
			item->second->hasLeftRecursion();
	
	foreach(item, _nonterminals)
		item->second->getSymbol()->calculateFirst();
	
	foreach(item, _nonterminals)
		item->second->getSymbol()->calculateFollow();
}

vector<GSymbol::Set> Grammar::getComponents()
{
	vector<GSymbol::Set> components;
	stack<Ref<GSymbol> > stack;
	int                  nextIndex = 0;
	
	foreach (def, _nonterminals)
		def->second->getSymbol()->getStrongComponents(components, stack, nextIndex);
	
	return components;
}

Grammar::Grammar()
{
	init();
	check();
	discover();
}

Ref<NonterminalDef> Grammar::getNonterminal(string name)
{
	if (_nonterminals.count(name) < 1)
		return NULL;
	return _nonterminals[name];
}

void Grammar::addConflict(Ref<Conflict> conflict)
{
	_conflicts.push_back(conflict);
}

void Grammar::output(string message)
{
	/* code */
}

void Grammar::warning(string message)
{
	stringstream s;
	s << "GRAMMAR WARNING: " << message << endl;
	output(s.str());
}

void Grammar::warning(Ref<NonterminalDef> nonterminal, string message)
{
	stringstream s;
	s << "(Nonterminal \"" << nonterminal->getName() << "\" on line " << nonterminal->getLine() << ") ";
	s << message;
	warning(s.str());
}

void Grammar::warning(string nonterminal, string message)
{
	warning(_nonterminals[nonterminal], message);
}

void Grammar::error(string message)
{
	stringstream s;
	s << "GRAMMAR ERROR: " << message << endl;
	output(s.str());
}

void Grammar::error(Ref<NonterminalDef> nonterminal, string message)
{
	stringstream s;
	s << "(Nonterminal \"" << nonterminal->getName() << "\" on line " << nonterminal->getLine() << ") ";
	s << message;
	warning(s.str());
}

void Grammar::error(string nonterminal, string message)
{
	error(_nonterminals[nonterminal], message);
}

void Grammar::dump(ostream& output)
{
	output << "***** GRAMMAR *****" << endl;
	
	output << "Nonterminals:" << endl;
	foreach (def, _nonterminals) {
		output << "   ";
		output << def->second->getName();
		output << " -> " << def->second->getSymbol();
		output << endl;
	}
	
	output << endl << "Topological order:" << endl;
	foreach (symbol, _sortedSymbols) {
		output << "   ";
		output << (*symbol)->getNonterminal()->getName() << ":";
		output << (*symbol)->getLine() << ": ";
		output << (*symbol);
		output << endl;
	}

	output << endl << "Strong components:" << endl;
	vector<GSymbol::Set> components = getComponents();
	foreach (component, components) {
		output << "   =====================" << endl;
		foreach (symbol, *component)
			output << "   " << *symbol << endl;
	}
}

