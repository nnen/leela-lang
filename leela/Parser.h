/**
 * \file   leela/Parser.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-29
 *
 * \brief  Header file for the Parser class.
 */

#ifndef PARSER_H_23VERTBYU8N
#define PARSER_H_23VERTBYU8N

#include <vector>
#include <stack>
#include <map>

#include "leela.h"
#include "Object.h"
#include "Lexer.h"
#include "Token.h"

using namespace std;

class Parser;

/* RULE ***********************************************************************/

class Rule : public Object {
protected:
	string _name;

public:
	Ref<Rule>                    setName(string name) { _name = name; return this; }
	
	virtual void                 build() { }
	virtual vector<Token::Type>  getFirst() const = 0;
	
	virtual void                 advance(Parser& parser) = 0;

	virtual void                 print(ostream& output) const = 0;
	virtual void                 printWithName(ostream& output) const;
};

Ref<Rule> operator + (Ref<Rule>   a, Ref<Rule>   b);
Ref<Rule> operator + (Token::Type a, Ref<Rule>   b);
Ref<Rule> operator + (Token::Type a, Token::Type b);
Ref<Rule> operator | (Ref<Rule>   a, Ref<Rule>   b);
Ref<Rule> operator | (Token::Type a, Ref<Rule>   b);
Ref<Rule> operator | (Token::Type a, Token::Type b);

/* RULE PROXY *****************************************************************/

class RuleProxy : public Rule {
private:
	Ref<Rule> _rule;

public:
	virtual void                 build() { _rule->build(); }
	virtual vector<Token::Type>  getFirst() const { return _rule->getFirst(); }
	
	virtual void                 advance(Parser& parser) { _rule->advance(parser); }

	virtual void                 print(ostream& output) const { _rule->print(output); }
	virtual void                 printWithName(ostream& output) const { _rule->print(output); }
	
	RuleProxy& operator = (Ref<Rule> rule);
};

/* TERMINAL *******************************************************************/

class Terminal : public Rule {
private:
	Token::Type _type;

public:
	Terminal(Token::Type type) : _type(type) {}
	
	virtual vector<Token::Type>  getFirst() const;

	virtual void                 advance(Parser& parser);

	virtual void                 print(ostream& output) const;
};

/* BINARY RULE ****************************************************************/

class BinaryRule : public Rule {
protected:
	Ref<Rule> _a, _b;

public:
	BinaryRule(Ref<Rule> a, Ref<Rule> b);
	virtual ~BinaryRule() {}
};

/* CHAIN **********************************************************************/

class Chain : public BinaryRule {
public:
	Chain(Ref<Rule> a, Ref<Rule> b) : BinaryRule(a, b) {}
	virtual ~Chain() {}

	virtual void                 build();
	virtual vector<Token::Type>  getFirst() const;

	virtual void                 advance(Parser& parser);

	virtual void                 print(ostream& output) const;
};

/* CHOICE *********************************************************************/

class Choice : public BinaryRule {
private:
	map<Token::Type, Ref<Rule> > _table;

public: 
	Choice(Ref<Rule> a, Ref<Rule> b) : BinaryRule(a, b) {}
	virtual ~Choice() {}
	
	virtual void                 build();
	virtual vector<Token::Type>  getFirst() const;

	virtual void                 advance(Parser& parser);

	virtual void                 print(ostream& output) const;
};

/* PARSER *********************************************************************/

/**
 * The Leela parser and compiler.
 */
class Parser : public Object {
private:
	Token             _current;
	stack<Ref<Rule> > _stack;
	
	Ref<Lexer>        _lexer;

	Ref<Rule>         _startingSymbol;

	void              initGrammar();

public:
	Parser();
	virtual ~Parser();

	void   parse(istream& input);
	
	Token& current() { return _current; }
	void   accept();
	void   push(Ref<Rule> rule);
};

#endif /* end of include guard: PARSER_H_23VERTBYU8N */

