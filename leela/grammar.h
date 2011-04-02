/**
 * \file   leela/grammar.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-29
 *
 * \brief  Defines classes for nonterminal symbols and grammar rules.
 */

#ifndef GRAMMAR_H_32098SDVFD349
#define GRAMMAR_H_32098SDVFD349

#include <ostream>

#include "Parser.h"

using namespace std;

/* NONTERMINALS ***************************************************************/

#define NONTERM(name) class name : public Nonterminal { \
	public: \
		name() : Nonterminal() {} \
		virtual ~name() {} \
		static const char * getName() { return #name; }

NONTERM(Program) };
NONTERM(Preamble) };
NONTERM(VarDecl) 
	void doSomething(Parser& parser) {}
};
NONTERM(VarDeclRest) };
NONTERM(CompoundCommand) };
NONTERM(Command) };
NONTERM(MoreCommands) };
NONTERM(Assignment) };
NONTERM(Expression) };

/* NONTERMINALS ***************************************************************/

void initGrammar();
void dumpGrammar(ostream& output);

#endif // end of include guard GRAMMAR_H_32098SDVFD349

