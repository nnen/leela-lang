/**
 * \file   leela/grammar.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-29
 *
 * \brief  Defines classes for nonterminal symbols and grammar rules.
 */

#include "grammar.h"

/* NONTERMINALS ***************************************************************/

void IdentList::onFinished(Parser& parser)
{
	if (matched.size() > 0) {
		Ref<Terminal>      identifier = getMatched<Terminal>(0);
		Ref<IdentListRest> rest       = getMatched<IdentListRest>(1);

		identifiers.push_front(identifier->getValue<String>()->getValue());
		identifiers.insert(identifiers.end(), rest->identifiers.begin(), rest->identifiers.end());

		parser.getOutput() << "IDENTLIST: ";
		foreach(i, identifiers)
			parser.getOutput() << *i << ", ";
		parser.getOutput() << std::endl;

	}
	Nonterminal::onFinished(parser);
}

void IdentListRest::onFinished(Parser& parser)
{
	if (matched.size() > 0) {
		Ref<Terminal>      terminal = getMatched<Terminal>(1);
		Ref<IdentListRest> rest     = getMatched<IdentListRest>(2);
		
		identifiers.push_front(terminal->getValue<String>()->getValue());
		identifiers.insert(identifiers.end(), rest->identifiers.begin(), rest->identifiers.end());
	}
	Nonterminal::onFinished(parser);
}

/* GRAMMAR ********************************************************************/

void initGrammar()
{
	#define DEF(t) NonterminalRule<t>::rule
	#define N(t) Ref<Rule>(new NonterminalRule<t>())
	#define T(t) Ref<Rule>(new TerminalRule(Token::t))
	#define A(type, action) Ref<Rule>(new SemanticAction<type>(&type::action, #action))
	#define STR(str) Ref<Rule>(new StringOutput(str))
	#define epsilon Ref<Rule>(new EpsilonRule())
	
	DEF(Program)         = N(Preamble) + N(CompoundCommand) + STR("STOP");
	
	DEF(Preamble)        = (N(VarDecl) + N(Preamble)) | epsilon;

	// DEF(VarDecl)         = T(KW_VAR) + T(IDENTIFIER) + A(VarDecl, doSomething) + N(VarDeclRest) + T(SEMICOLON);

	// DEF(VarDeclRest)     = (T(COMMA) + T(IDENTIFIER) + N(VarDeclRest)) | epsilon;
	
	DEF(VarDecl)         = T(KW_VAR) + N(IdentList) + T(SEMICOLON);
	
	DEF(CompoundCommand) = T(KW_BEGIN) + N(Command) + N(MoreCommands) + T(KW_END);
	
	DEF(MoreCommands)    = (T(SEMICOLON) + N(Command) + N(MoreCommands)) | epsilon;

	DEF(Command)         = N(Assignment) | epsilon;

	DEF(Assignment)      = T(IDENTIFIER) + T(ASSIGN) + N(Expression);

	DEF(Expression)      = T(NUMBER_LITERAL) | T(STRING_LITERAL) | ( T(LEFT_PAR) + N(Expression) + T(RIGHT_PAR) );
	
	
	DEF(Lambda)          = T(LAMBDA) + N(IdentList) + T(COLON) + N(Expression);
	
	DEF(IdentList)       = T(IDENTIFIER) + N(IdentListRest) | epsilon;
	
	DEF(IdentListRest)   = T(COMMA) + T(IDENTIFIER) + N(IdentListRest) | epsilon;
	
	#undef DEF
	#undef N
	#undef T
	#undef A
	#undef STR
	#undef epsilon

	#define NT(name) NonterminalRule<name>::rule->getFirsts();
	NONTERMINALS
	#undef NT

	#define NT(name) NonterminalRule<name>::calculateFollow();
	NONTERMINALS
	NONTERMINALS
	#undef NT
}

void dumpGrammar(ostream& output)
{
	#define NT(name) { NonterminalRule<name>::dumpRule(output); output << endl << endl; }

	NONTERMINALS
	
	/*
	DUMP(Program);
	DUMP(Preamble);
	DUMP(VarDecl);
	DUMP(VarDeclRest);
	DUMP(CompoundCommand);
	DUMP(MoreCommands);
	DUMP(Command);
	DUMP(Assignment);
	DUMP(Expression);
	*/

	#undef NT
}

