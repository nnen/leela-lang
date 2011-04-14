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
#include <set>

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
	stack<Ref<Nonterminal> >     _nonterminals;
	
	ostream                    * _output;

public:
	Parser();
	virtual ~Parser() {}
	
	void parse(istream& input, ostream& output);
	void parse(istream& input) { parse(input, cout); }
	void parse() { parse(cin, cout); }
	
	ostream&              getOutput() { return *_output; }
	
	Token                 current() { return _current; }
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
protected:
	Ref<Object> _attribute;

public:
	Symbol() : Object() {}
	virtual ~Symbol() {}

	virtual void onPop(Parser& parser) {}
	virtual void onPush(Parser& parser) {}

	virtual void print(ostream& output) const { output << "Symbol[]"; }

	Ref<Object> getAttribute() const { return _attribute; }
};

ostream& operator << (ostream& output, const Symbol& symbol);

class InputSymbol : public Symbol {
public:
	InputSymbol() : Symbol() {}
	virtual ~InputSymbol() {}
	
	// virtual vector<Ref<Symbol> > getMatched() { return vector<Ref<Symbol> >(); }
};

class Terminal : public InputSymbol {
private:
	Token       _token;

public:
	Terminal(Token token) : InputSymbol(), _token(token) {}
	virtual ~Terminal() {}

	virtual void print(ostream& output) const { output << _token; }

	Token& getToken() { return _token; }
	Ref<Value> getValue() { return _token.value; }

	template<class T>
	Ref<T> getValue() { return _token.value.as<T>(); }
};

class Nonterminal : public InputSymbol {
public:
	vector<Ref<Symbol> > matched;

	Nonterminal() : InputSymbol() {}
	
	virtual void onFinished(Parser& parser) { }
	
	void append(Ref<InputSymbol> symbol) { matched.push_back(symbol); }

	template<class T>
	Ref<T> getMatched(int index)
	{
		return matched[index].as<T>();
	}
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

	virtual void print(ostream& output) const
	{
		output << "(EON)";
	}
};

/* RULES **********************************************************************/

class Rule : public InputSymbol {
public:
	Rule() : InputSymbol() {}
	virtual ~Rule() {}
	
	virtual void                onPop(Parser& parser) { std::cerr << "ERROR" << std::endl; }
	
	virtual void                init() {}
	virtual vector<Token::Type> getFirsts() { return vector<Token::Type>(); }
	virtual void                clearFollow() {}
	virtual void                addFollow(vector<Ref<Rule> > following) {}
	virtual bool                maybeEmpty() { return false; }
	
	virtual void                dump(ostream& output) const = 0;
	virtual void                print(ostream& output) const { dump(output); }
};

Ref<Rule> operator + (Ref<Rule> a, Ref<Rule> b);
Ref<Rule> operator | (Ref<Rule> a, Ref<Rule> b);

class EpsilonRule : public Rule {
public:
	EpsilonRule() : Rule() {}
	virtual ~EpsilonRule() {}
	
	virtual void onPop(Parser& parser) {}
	
	virtual bool maybeEmpty() { return true; }
	
	virtual void print(ostream& output) const { dump(output); }
		
	virtual void dump(ostream& output) const { output << "e"; }
};

class TerminalRule : public Rule {
private:
	Token::Type _type;

public:
	TerminalRule(Token::Type type) : Rule(), _type(type) {}
	virtual ~TerminalRule() {}
	
	virtual void onPop(Parser& parser)
	{
		if (parser.current().type != _type) {
			parser.dumpState(std::cerr);
			std::cerr << "ERROR: Expected token " << Token::getTypeName(_type)
				<< ", found " << Token::getTypeName(parser.current().type) << "." << std::endl;
			// TODO: Finish error handling here.
		}
		
		parser.accept();
	}

	virtual vector<Token::Type> getFirsts()
	{
		vector<Token::Type> v;
		v.push_back(_type);
		return v;
	}

	virtual void print(ostream& output) const { dump(output); }

	virtual void dump(ostream& output) const
	{
		const char * c = Token::getTypeName(_type);
		while (*c != '\0')
			output.put(tolower(*(c++)));
	}
};

template<class TNonterminal>
class NonterminalRule : public Rule {
private:
	static set<Token::Type> _follow;

public:
	// This is static so that it can be referenced BEFORE it
	// is assigned any value. Also, since this static variable
	// exists in a template, there's one instance for each
	// template parameter (for each class derived from the
	// Nonterminal class).
	static Ref<Rule> rule;

	NonterminalRule() : Rule() {}
	virtual ~NonterminalRule() {}

	virtual void onPop(Parser& parser)
	{
		parser.startNonterminal(new TNonterminal());
		parser.push(new EndOfNonterminal());
		parser.push(rule);
	}
	
	virtual vector<Token::Type> getFirsts()
	{
		return rule->getFirsts();
	}

	virtual void clearFollow() { _follow.clear(); }
	
	virtual void addFollow(vector<Ref<Rule> > following)
	{
		foreach(r, following) {
			vector<Token::Type> firsts = (*r)->getFirsts();
			foreach(first, firsts)
				_follow.insert(*first);
		}
	}
	
	static void  calculateFollow()
	{
		vector<Ref<Rule> > v;

		foreach(it, _follow)
			v.push_back(new TerminalRule(*it));

		rule->addFollow(v);
	}

	virtual bool maybeEmpty() { return rule->maybeEmpty(); }
	
	virtual void print(ostream& output) const
	{
		output << TNonterminal::getName();
	}

	virtual void dump(ostream& output) const
	{
		output << TNonterminal::getName();
	}

	static void dumpRule(ostream& output)
	{
		Ref<Rule> r = rule;
		
		output << TNonterminal::getName() << " -> ";
		r->dump(output);
		
		output << endl << "\tMAYBE EMPTY: " << (r->maybeEmpty() ? "TRUE" : "FALSE");
		output << endl << "\tFIRST: ";
		vector<Token::Type> v = r->getFirsts();
		foreach(it, v)
			output << Token::getTypeName(*it) << ", ";

		output << endl << "\tFOLLOW: ";
		foreach(it, _follow)
			output << Token::getTypeName(*it) << ", ";
	}
};

template<class TNonterminal>
Ref<Rule> NonterminalRule<TNonterminal>::rule;

template<class TNonterminal>
set<Token::Type> NonterminalRule<TNonterminal>::_follow;

class RepeatRule : public Rule {
private:
	Ref<Rule>        _rule;
	set<Token::Type> _first;

public:
	RepeatRule(Ref<Rule> rule) : _rule(rule) {}
	virtual ~RepeatRule() {}

	virtual void onPop(Parser& parser)
	{
		if (_first.count(parser.current().type) < 1)
			return;
		parser.push(this);
		parser.push(_rule);
	}
	
	virtual void init()
	{
		vector<Token::Type> first = getFirsts();
		_first.clear();
		foreach(it, first)
			_first.insert(*it);
		_rule->init();
	}
	virtual void clearFollow() { _rule->clearFollow(); }
	virtual void addFollow(vector<Ref<Rule> > following)
	{ 
		vector<Token::Type> first = _rule->getFirsts();
		foreach (it, first)
			following.push_back(new TerminalRule(*it));
		_rule->addFollow(following);
	}
	virtual vector<Token::Type> getFirsts()
	{
		return _rule->getFirsts();
	}
	
	virtual bool maybeEmpty() { return true; }

	virtual void dump(ostream& output) const
	{
		output << "{ ";
		_rule->dump(output);
		output << " }";
	}
};

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

	virtual void dump(ostream& output) const
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

	virtual void print(ostream& output) const { dump(output); }

	virtual void dump(ostream& output) const
	{
		output << "(\"" << _str << "\")";
	}
};

class BinaryRule : public Rule {
protected:
	Ref<Rule> _a, _b;

public:
	BinaryRule(Ref<Rule> a, Ref<Rule> b) : Rule(), _a(a), _b(b) {}
	virtual ~BinaryRule() {}

	virtual void init()
	{
		_a->init();
		_b->init();
	}
	
	virtual void clearFollow()
	{
		_a->clearFollow();
		_b->clearFollow();
	}
};

class ChainRule : public BinaryRule {
public:
	ChainRule(Ref<Rule> a, Ref<Rule> b) : BinaryRule(a, b) {}
	virtual ~ChainRule() {}

	virtual void onPop(Parser& parser)
	{
		parser.push(_b);
		parser.push(_a);
	}

	virtual vector<Token::Type> getFirsts()
	{
		vector<Token::Type> v1 = _a->getFirsts();
		
		if (_a->maybeEmpty()) {
			vector<Token::Type> v2 = _b->getFirsts();
			foreach(it, v2) v1.push_back(*it);
		}
		
		return v1;
	}

	virtual void addFollow(vector<Ref<Rule> > following)
	{
		vector<Ref<Rule> > v;
		v.push_back(_b);

		if (_b->maybeEmpty()) {
			v.insert(v.end(), following.begin(), following.end());
			_a->addFollow(v);
		} else {
			_a->addFollow(v);
		}
		
		_b->addFollow(following);
	}

	virtual bool maybeEmpty() { return _a->maybeEmpty() && _b->maybeEmpty(); }
	
	virtual void print(ostream& output) const { dump(output); }
		
	virtual void dump(ostream& output) const
	{
		_a->dump(output);
		output << " ";
		_b->dump(output);
	}
};

class ChoiceRule : public BinaryRule {
private:
	map<Token::Type, Ref<Rule> > _table;

public:
	ChoiceRule(Ref<Rule> a, Ref<Rule> b) : BinaryRule(a, b) {}
	virtual ~ChoiceRule() {}

	virtual void onPop(Parser& parser)
	{
		// NOTE: This logic assumes there are not
		//       FIRST/FOLLOW conflicts.
		// 1. Try selecting rule by first terminal
		// 2. If it can be empty, try using first symbol
		// 3. If it can be empty, try using second symbol
		// 4. Throw syntax error
		if (_table.count(parser.current().type) > 0) {
			parser.push(_table[parser.current().type]);
		} else if (_a->maybeEmpty()) {
			parser.push(_a);
		} else if (_b->maybeEmpty()) {
			parser.push(_b);
		} else {
			// TODO: Handle error.
		}
	}
	
	virtual void init()
	{
		BinaryRule::init();

		vector<Token::Type> v1 = _a->getFirsts();
		vector<Token::Type> v2 = _b->getFirsts();
		
		_table.clear();
		
		foreach(it, v1)
			_table[*it] = _a;
			
		foreach(it, v2)
			_table[*it] = _b;
	}
	virtual vector<Token::Type> getFirsts()
	{
		// TODO: Use the table to get the FIRST
		vector<Token::Type> v1 = _a->getFirsts();
		vector<Token::Type> v2 = _b->getFirsts();
		
		vector<Token::Type> v;
		v.insert(v.end(), v1.begin(), v1.end());
		v.insert(v.end(), v2.begin(), v2.end());

		return v;
	}
	
	virtual void addFollow(vector<Ref<Rule> > following)
	{
		_a->addFollow(following);
		_b->addFollow(following);
	}

	virtual bool maybeEmpty() { return _a->maybeEmpty() || _b->maybeEmpty(); }

	virtual void print(ostream& output) const { dump(output); }
	virtual void dump(ostream& output) const
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

