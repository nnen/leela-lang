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
class Symbol;
class InputSymbol;
class Nonterminal;

/* PARSER *********************************************************************/

class Parser : public Object {
private:
	Ref<Lexer>                   _lexer;
	Token                        _current;
	
	stack<Ref<Symbol> >          _stack;
	// stack<vector<Ref<Symbol> > > _auxiliary;
	stack<Ref<Nonterminal> >     _nonterminals;

	ostream                    * _output;

public:
	Parser();
	virtual ~Parser() {}
	
	void parse(istream& input, ostream& output);
	void parse(istream& input) { parse(input, cout); }
	void parse() { parse(cin, cout); }
	
	ostream&              getOutput() { return *_output; }
	
	void                  push(Ref<Symbol> symbol);
	void                  append(Ref<InputSymbol> symbol);
	void                  accept();
	void                  startNonterminal(Ref<Nonterminal> nonterminal);
	Ref<Nonterminal>      getNonterminal();
	// vector<Ref<Symbol> >& getMatched();
	void                  endNonterminal();

	void                  dumpState(ostream& output);
};

/* SYMBOLS ********************************************************************/

/**
 * Represents an LL grammar symbol.
 *
 * The symbols fall in two main cathegories:
 *
 * \li Input symbols - Match some sequence of terminal and nonterminal symbols.
 * \li Output symbols - do not match any input symbols, but may change the state
 *                      of the parser and write to the output.
 */
class Symbol : public Object {
public:
	Symbol() : Object() {}

	virtual void onPop(Parser& parser) {}
	virtual void onPush(Parser& parser) {}

	virtual void print(ostream& output) const { output << "Symbol[]"; }
};

ostream& operator << (ostream& output, const Symbol& symbol);

class InputSymbol : public Symbol {
public:
	InputSymbol() : Symbol() {}
	
	// virtual vector<Ref<Symbol> > getMatched() { return vector<Ref<Symbol> >(); }
};

class Terminal : public InputSymbol {
private:
	Token       _token;

public:
	Terminal(Token token) : InputSymbol(), _token(token) {}
	virtual ~Terminal() {}
};

class Nonterminal : public InputSymbol {
public:
	vector<Ref<Symbol> > matched;

	Nonterminal() : InputSymbol() {}
	
	virtual void onFinished(Parser& parser) { matched.clear(); }

	void append(Ref<InputSymbol> symbol) { matched.push_back(symbol); }
};

class OutputSymbol : public Symbol {
public:
	OutputSymbol() : Symbol() {}
	// virtual vector<Ref<Symbol> > getMatched();
};

class EndOfNonterminal : public OutputSymbol {
public:
	EndOfNonterminal() : OutputSymbol() {}

	virtual void onPop(Parser& parser)
	{
		parser.endNonterminal();
	}
};

/* RULES **********************************************************************/

class Rule : public InputSymbol {
public:
	Rule() : InputSymbol() {}

	virtual vector<Token::Type> getFirsts() { return vector<Token::Type>(); }
	virtual bool maybeEmpty() { return false; }
	
	virtual void dump(ostream& output) = 0;
};

Ref<Rule> operator + (Ref<Rule> a, Ref<Rule> b);
Ref<Rule> operator | (Ref<Rule> a, Ref<Rule> b);

class EpsilonRule : public Rule {
public:
	EpsilonRule() : Rule() {}

	virtual bool maybeEmpty() { return true; }

	virtual void dump(ostream& output)
	{
		output << "e";
	}
};

class TerminalRule : public Rule {
private:
	Token::Type _type;

public:
	TerminalRule(Token::Type type) : Rule(), _type(type) {}

	virtual vector<Token::Type> getFirsts()
	{
		vector<Token::Type> v;
		v.push_back(_type);
		return v;
	}

	virtual void dump(ostream& output)
	{
		const char * c = Token::getTypeName(_type);
		while (*c != '\0')
			output.put(tolower(*(c++)));
	}
};

template<class TNonterminal>
class NonterminalRule : public Rule {
public:
	static Ref<Rule> rule;

	NonterminalRule() : Rule() {}

	virtual void onPop(Parser& parser)
	{
		parser.startNonterminal(new TNonterminal());
		parser.push(rule);
		parser.push(new EndOfNonterminal());
	}

	virtual vector<Token::Type> getFirsts()
	{
		return rule->getFirsts();
	}

	virtual bool maybeEmpty() { return rule->maybeEmpty(); }

	virtual void dump(ostream& output)
	{
		output << TNonterminal::getName();
		// rule->dump(output);
	}

	static void dumpRule(ostream& output)
	{
		output << TNonterminal::getName() << " -> ";
		rule->dump(output);

		output << " {FIRST: ";
		vector<Token::Type> v = rule->getFirsts();
		foreach(it, v)
			output << Token::getTypeName(*it) << ", ";
		output << "}";
	}
};

template<class TNonterminal>
Ref<Rule> NonterminalRule<TNonterminal>::rule;

template<class TNonterminal>
class SemanticAction : public Rule {
public:
	typedef void (TNonterminal::*Delegate)(Parser& parser);

private:
	Delegate         _action;
	string           _name;

public:
	SemanticAction(Delegate action, string name)
		: Rule(), _action(action), _name(name) {}
	
	virtual ~SemanticAction() {}
	
	virtual void onPop(Parser& parser)
	{
		Ref<TNonterminal> nonterminal = parser.getNonterminal();

		if (nonterminal.isNull())
			return; // TODO: Handle null reference.
		
		(nonterminal.getPtr()->*_action)(parser);
	}

	virtual bool maybeEmpty() { return true; }

	virtual void dump(ostream& output)
	{
		if (_name.length() > 0)
			output << "(" << _name << ")";
		else
			output << "(semantic action)";
	}
};

class StringOutput : public Rule {
private:
	string _str;

public:
	StringOutput(string str) : Rule(), _str(str) {}
	virtual ~StringOutput() {}

	virtual void onPop(Parser& parser)
	{
		parser.getOutput() << _str;
	}
	
	virtual bool maybeEmpty() { return true; }

	virtual void dump(ostream& output)
	{
		output << "(\"" << _str << "\")";
	}
};

class BinaryRule : public Rule {
protected:
	Ref<Rule> _a, _b;

public:
	BinaryRule(Ref<Rule> a, Ref<Rule> b) : Rule(), _a(a), _b(b) {}
};

class ChainRule : public BinaryRule {
public:
	ChainRule(Ref<Rule> a, Ref<Rule> b) : BinaryRule(a, b) {}

	virtual vector<Token::Type> getFirsts()
	{
		vector<Token::Type> v1 = _a->getFirsts();
		
		if (_a->maybeEmpty()) {
			vector<Token::Type> v2 = _b->getFirsts();
			foreach(it, v2) v1.push_back(*it);
		}
		
		return v1;
	}

	virtual bool maybeEmpty() { return _a->maybeEmpty() && _b->maybeEmpty(); }
	
	virtual void dump(ostream& output)
	{
		_a->dump(output);
		output << " ";
		_b->dump(output);
	}
};

class ChoiceRule : public BinaryRule {
public:
	ChoiceRule(Ref<Rule> a, Ref<Rule> b) : BinaryRule(a, b) {}

	virtual vector<Token::Type> getFirsts()
	{
		vector<Token::Type> v = _a->getFirsts();
		vector<Token::Type> v2 = _b->getFirsts();
		
		foreach(it, v2)
			v.push_back(*it);

		return v;
	}

	virtual bool maybeEmpty() { return _a->maybeEmpty() || _b->maybeEmpty(); }

	virtual void dump(ostream& output)
	{
		// output << "( ";
		_a->dump(output);
		// output << " ) | ( ";
		output << " | ";
		_b->dump(output);
		// output << " )";
	}
};


#endif /* end of include guard: PARSER_H_23VERTBYU8N */

