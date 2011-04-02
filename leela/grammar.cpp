/**
 * \file   leela/grammar.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-29
 *
 * \brief  Defines classes for nonterminal symbols and grammar rules.
 */

#include "grammar.h"

/* NONTERMINALS ***************************************************************/

/* GRAMMAR ********************************************************************/

void initGrammar()
{
	#define DEF(t) NonterminalRule<t>::rule
	#define N(t) Ref<Rule>(new NonterminalRule<t>())
	#define T(t) Ref<Rule>(new TerminalRule(Token::t))
	#define A(type, action) Ref<Rule>(new SemanticAction<type>(&type::action, #action))
	#define STR(str) Ref<Rule>(new StringOutput(str))
	#define epsilon Ref<Rule>(new EpsilonRule())
	
	#define DUMP(t) { NonterminalRule<t>::dumpRule(std::cout); std::cout << endl; }

	Ref<Rule> rule       = N(Preamble) + N(CompoundCommand) + STR("STOP");
	NonterminalRule<Program>::rule = rule;

	// DEF(Program)         = N(Preamble) + N(CompoundCommand) + STR("STOP");
	// DUMP(Program)
	
	DEF(Preamble)        = (N(VarDecl) + N(Preamble)) | epsilon;

	DEF(VarDecl)         = T(KW_VAR) + T(IDENTIFIER) + A(VarDecl, doSomething) + N(VarDeclRest) + T(SEMICOLON);

	DEF(VarDeclRest)     = (T(COMMA) + T(IDENTIFIER) + N(VarDeclRest)) | epsilon;
	
	DEF(CompoundCommand) = T(KW_BEGIN) + N(Command) + N(MoreCommands) + T(KW_END);
	
	DEF(MoreCommands)    = (T(SEMICOLON) + N(Command) + N(MoreCommands)) | epsilon;

	DEF(Command)         = N(Assignment) | epsilon;

	DEF(Assignment)      = T(IDENTIFIER) + T(ASSIGN) + N(Expression);

	DEF(Expression)      = T(NUMBER_LITERAL) | T(STRING_LITERAL);

	#undef DUMP
	
	#undef DEF
	#undef N
	#undef T
	#undef A
	#undef STR
	#undef epsilon
}

void dumpGrammar(ostream& output)
{
	#define DUMP(t) { NonterminalRule<t>::dumpRule(output); output << endl; }

	DUMP(Program);
	DUMP(Preamble);
	DUMP(VarDecl);
	DUMP(VarDeclRest);
	DUMP(CompoundCommand);
	DUMP(MoreCommands);
	DUMP(Command);
	DUMP(Assignment);
	DUMP(Expression);

	#undef DUMP
}

