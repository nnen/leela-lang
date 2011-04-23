/**
 * \file   Grammar.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-20
 *
 * \brief  
 */

#ifndef GRAMMAR_H_23498SDS203GFQZGTD
#define GRAMMAR_H_23498SDS203GFQZGTD

#include <ostream>
#include <vector>
#include <map>
#include <string>

#include "Object.h"
#include "GSymbol.h"
#include "Conflict.h"

using namespace std;

class Grammar : public Object {
private:
	GSymbol::Set                      _allSymbols;
	vector<Ref<GSymbol> >             _sortedSymbols;
	
	map<string, Ref<NonterminalDef> > _nonterminals;
	vector<Ref<Conflict> >            _conflicts;
	
	Ref<NonterminalDef>               _startingSymbol;
	
	NonterminalDef& definition(int line, string name);
	Ref<GSymbol>    terminal(int line, Token::Type terminal);
	Ref<GSymbol>    nonterminal(int line, string name);
	
	void            init();
	void            discover();
	void            check();

public:
	Grammar();
	virtual ~Grammar() {}
	
	Ref<NonterminalDef> getNonterminal(string name);
	
	void                addConflict(Ref<Conflict> conflict);
	
	void output(string message);
	void warning(string message);
	void warning(Ref<NonterminalDef> nonterminal, string message);
	void warning(string nonterminal, string message);
	void error(string message);
	void error(Ref<NonterminalDef> nonterminal, string message);
	void error(string nonterminal, string message);
	
	void dump(ostream& output);
};

#endif /* end of include guard: GRAMMAR_H_23498SDS203GFQZGTD */

