/**
 * \file   leela/grammar.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-29
 *
 * \brief  Defines classes for nonterminal symbols and grammar rules.
 */

#include "grammar.h"

/* NONTERMINALS ***************************************************************/

/*
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
*/

void IdentList::onFinished(Parser& parser)
{
	if (matched.size() > 0) {
		for (unsigned int i = 0; i < matched.size(); i += 2) {
			Ref<Terminal> identifier = getMatched<Terminal>(i);
			identifiers.push_back(identifier->getValue<String>()->getValue());
		}

		parser.getOutput() << "IDENTLIST: ";
		foreach(i, identifiers)
			parser.getOutput() << *i << ", ";
		parser.getOutput() << std::endl;
	}
	Nonterminal::onFinished(parser);
}

/*
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
*/

/* GRAMMAR ********************************************************************/

void initGrammar()
{
	#define DEF(t) NonterminalRule<t>::rule
	#define N(t) Ref<Rule>(new NonterminalRule<t>())
	#define T(t) Ref<Rule>(new TerminalRule(Token::t))
	#define A(type, action) Ref<Rule>(new SemanticAction<type>(&type::action, #action))
	#define STR(str) Ref<Rule>(new StringOutput(str))
	#define REPEAT(rule) Ref<Rule>(new RepeatRule((rule)))
	#define epsilon Ref<Rule>(new EpsilonRule())
	
	DEF(Program)           = N(Preamble) + N(CompoundStatement) + STR("\tSTOP\n");
	
	DEF(Preamble)          = (N(VarDecl) + N(Preamble)) | epsilon;

	DEF(VarDecl)           = T(KW_VAR) + N(IdentList) + T(SEMICOLON);
	
	DEF(CompoundStatement) = T(KW_BEGIN) + N(Statement) + REPEAT(T(SEMICOLON) + N(Statement)) + T(KW_END);
	
	DEF(Statement)         = N(Assignment) | N(IfStatement) | N(ReturnStatement) | N(CompoundStatement) | epsilon;
	
	DEF(Assignment)        = T(IDENTIFIER) + T(ASSIGN) + N(Expression) + STR("\tSTR\n");
	
	DEF(IfStatement)       = T(KW_IF) + N(Expression) + T(KW_THEN) + N(Statement) + N(ElseStatement);
	
	DEF(ElseStatement)     = (T(KW_ELSE) + N(Statement)) | epsilon;
	
	// ReturnStatement    -> 'return' Expression (RETURN)
	DEF(ReturnStatement)   = T(KW_RETURN) + N(Expression) + STR("\tRETURN\n");
	
	// Expression         -> ['-'] Term { '+' Term (ADD) | '-' Term (SUB) }
	DEF(Expression)        = (T(MINUS) | epsilon) + N(Term) + REPEAT(
	                             (T(PLUS)  + N(Term) + STR("\tADD\n")) |
	                             (T(MINUS) + N(Term) + STR("\tSUB\n"))
                            );
	
	// Term               -> Factor { '*' Factor (MULT) | '/' Factor (DIV) }
	DEF(Term)              = N(Factor) + REPEAT(
	                             (T(ASTERIX) + N(Factor) + STR("\tMULT\n")) |
	                             (T(SLASH)   + N(Factor) + STR("\tDIV\n"))
	                         );
	
	// Factor             -> identifier | number_literal | string_literal | Lambda | '(' Expression ')'
	DEF(Factor)            = T(IDENTIFIER) | T(NUMBER_LITERAL) | T(STRING_LITERAL) | N(Lambda) | ( T(LEFT_PAR) + N(Expression) + T(RIGHT_PAR) );
	
	// Lambda             -> 'lambda' IdentList ':' Expression
	DEF(Lambda)            = T(KW_LAMBDA) + N(IdentList) + T(COLON) + N(Expression);
	
	// IdentList          -> identifier { ',' identifier } | e
	DEF(IdentList)         = T(IDENTIFIER) + REPEAT(T(COMMA) + T(IDENTIFIER)) | epsilon;
	
	#undef DEF
	#undef N
	#undef T
	#undef A
	#undef STR
	#undef epsilon

	#define NT(name) NonterminalRule<name>::rule->init();
	NONTERMINALS
	#undef NT

	#define NT(name) NonterminalRule<name>::rule->getFirsts();
	NONTERMINALS
	NONTERMINALS
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
	#undef NT
}

