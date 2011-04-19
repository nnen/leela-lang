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

#define NONTERMINALS     \
	NT(Program)           \
	NT(Preamble)          \
	NT(VarDecl)           \
	NT(CompoundStatement) \
	NT(Statement)         \
	/*                    \
	NT(MoreStatements)    \
	*/                    \
	NT(IfStatement)       \
	NT(ElseStatement)     \
	NT(WhileStatement)    \
	NT(ReturnStatement)   \
	NT(Assignment)        \
	\
	NT(Expression)        \
	NT(Term)              \
	NT(Factor)            \
   \
	NT(Lambda)            \
	NT(IdentList)         \
	// NT(IdentListRest)

using namespace std;

/* NONTERMINALS ***************************************************************/

#define NONTERM(name) class name : public Nonterminal { \
	public: \
		name() : Nonterminal() {} \
		virtual ~name() {} \
		static const char * getName() { return #name; } \
		virtual void print(ostream& output) const { output << getName(); }

NONTERM(Program)           };
NONTERM(Preamble)          };
NONTERM(VarDecl)           };

NONTERM(CompoundStatement) };
NONTERM(Statement)         };
NONTERM(IfStatement)       };
NONTERM(ElseStatement)     };
NONTERM(WhileStatement)    };
NONTERM(ReturnStatement)   };
NONTERM(Assignment)        };

NONTERM(Expression)        };
NONTERM(Term)              };
NONTERM(Factor)            };

NONTERM(Lambda)            };

NONTERM(IdentList)
	virtual void onFinished(Parser& parser);
};

/* NONTERMINALS ***************************************************************/

void initGrammar();
void dumpGrammar(ostream& output);

#endif // end of include guard GRAMMAR_H_32098SDVFD349

