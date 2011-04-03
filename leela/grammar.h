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
#include <list>

#include "Parser.h"

#define NONTERMINALS   \
	NT(Program)         \
	NT(Preamble)        \
	/*                  \
	NT(VarDecl)         \
	NT(VarDeclRest)     \
	*/                  \
	NT(CompoundCommand) \
	NT(Command)         \
	NT(MoreCommands)    \
	NT(Assignment)      \
	NT(Expression)      \
   \
	NT(Lambda)          \
	NT(IdentList)       \
	NT(IdentListRest)

using namespace std;

/* NONTERMINALS ***************************************************************/

#define NONTERM(name) class name : public Nonterminal { \
	public: \
		name() : Nonterminal() {} \
		virtual ~name() {} \
		static const char * getName() { return #name; } \
		virtual void print(ostream& output) const { output << getName(); }

NONTERM(Program) };
NONTERM(Preamble) };
NONTERM(VarDecl) 
	virtual void onFinished(Parser& parser)
	{
		std::cerr << "VarDecl finished, name = " << *(matched[1]) << std::endl;
		Nonterminal::onFinished(parser);
	}
};
// NONTERM(VarDeclRest) };
NONTERM(CompoundCommand) };
NONTERM(Command) };
NONTERM(MoreCommands) };
NONTERM(Assignment) };
NONTERM(Expression) };

NONTERM(Lambda) };

NONTERM(IdentList)
	list<string> identifiers;
	
	virtual void onFinished(Parser& parser);
};

NONTERM(IdentListRest)
	list<string> identifiers;

	virtual void onFinished(Parser& parser);
};

/* NONTERMINALS ***************************************************************/

void initGrammar();
void dumpGrammar(ostream& output);

#endif // end of include guard GRAMMAR_H_32098SDVFD349

