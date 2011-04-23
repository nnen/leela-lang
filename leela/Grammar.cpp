/**
 * \file   Grammar.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-20
 *
 * \brief  
 */

#include <queue>
#include <sstream>

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
	return new Terminal(this, terminal);
}

Ref<GSymbol> Grammar::nonterminal(int line, string name)
{
	return new NonterminalRef(this, line, name);
}

void Grammar::init()
{
	#define DEF(name) definition(__LINE__, #name)
	#define T(name)   terminal(__LINE__, Token::name)
	#define N(name)   nonterminal(__LINE__, #name)
	
	DEF(Program) = N(Preamble);
	DEF(Preamble) = T(KW_VAR);
	
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
		
		if (_allSymbols.count(symbol) > 0)
			continue;
		
		_allSymbols.insert(symbol);
		_sortedSymbols.push_back(symbol);
		foreach (child, symbol->getChildren())
			toAdd.push(*child);
	}
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
	
	output << "Topological order:" << endl;
	foreach (symbol, _sortedSymbols) {
		output << "   ";
		output << (*symbol)->getNonterminal()->getName() << ":";
		output << (*symbol)->getLine() << ": ";
		output << endl;
	}
}

