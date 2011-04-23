/**
 * \file   GSymbol.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-21
 *
 * \brief  
 */

#ifndef GSYMBOL_H_123AXSV0SDVG214323SFVDFV
#define GSYMBOL_H_123AXSV0SDVG214323SFVDFV

#include <set>
#include <stack>

#include "Object.h"
#include "Token.h"

using namespace std;

class Grammar;
class NonterminalDef;

/* GSymbol ********************************************************************/

class GSymbol : public Object {
public:
	typedef set<Ref<GSymbol> > Set;

protected:
	Grammar *           _grammar;
	Ref<NonterminalDef> _nonterminal;
	int                 _line;
	set<Token::Type>    _follow;

	// For the use with Tarjan's strongly connected components algorithm
	// (http://tinyurl.com/3gs5qq2 - Wikipedia)
	int                 _index;
	int                 _lowlink;
	
	Set                 getStrongComponent(stack<Ref<GSymbol> >& stack, int& nextIndex);

public:
	GSymbol(Grammar * grammar, int line);
	virtual ~GSymbol();
	
	Ref<NonterminalDef>      getNonterminal() const { return _nonterminal; }
	void                     setNonterminal(Ref<NonterminalDef> nonterminal) { _nonterminal = nonterminal; }
	int                      getLine() const { return _line; }
	
	virtual Set              getChildren() const { return Set(); }
	Set                      getStrongComponent();
	
	virtual bool             maybeEmpty() { return false; }
	virtual void             checkRecursion() {}
	virtual bool             hasLeftRecursion(Ref<NonterminalDef> nonterminal) { return false; }
	virtual void             calculateFirst() {}
	virtual void             calculateFollow() {}
	virtual void             addFollow(Token::Type terminal);
	virtual void             addFollow(set<Token::Type> terminals);
	
	virtual bool             isRecursive() { return false; }
	virtual set<Token::Type> getFirst() = 0;
	virtual set<Token::Type> getFollow() { return _follow; }
};

/* Terminal *******************************************************************/

class Terminal : public GSymbol {
private:
	Token::Type _terminal;
	bool        _epsilon;

public:
	Terminal(Grammar * grammar, int line, Token::Type terminal);
	Terminal(Grammar * grammar, int line);
	virtual ~Terminal() {}
	
	virtual set<Token::Type> getFirst();
	virtual set<Token::Type> getFollow() { return set<Token::Type>(); }
	
	bool isEpsilon() { return _epsilon; }
};

/* Nonterminal ****************************************************************/

class Nonterminal : public GSymbol {
protected:
	bool                _recursive;
	set<Token::Type>    _first;

public:
	Nonterminal(Grammar * grammar, int line);
	virtual ~Nonterminal() {}
	
	virtual void                checkRecursion(); 
	
	virtual bool                isRecursive() { return _recursive; }
	virtual set<Token::Type>    getFirst() { return _first; }
};

/* NonterminalRef *************************************************************/

class NonterminalRef : public Nonterminal {
protected:
	string              _name;
	Ref<NonterminalDef> _definition;

public:
	NonterminalRef(Grammar * grammar, int line, string name);
	virtual ~NonterminalRef() {}
	
	virtual Set              getChildren();
	
	virtual bool             maybeEmpty();
	virtual bool             hasLeftRecursion(Ref<NonterminalDef> nonterminal);
	virtual set<Token::Type> getFirst();
	virtual void             addFollow(set<Token::Type> terminals);
	virtual void             calculateFollow() {}
	
	string                   getName() { return _name; }
	Ref<NonterminalDef>      getDefinition();
};

/* BinarySymbol ***************************************************************/

class BinarySymbol : public GSymbol {
protected:
	Ref<GSymbol> _a, _b;

public:
	virtual ~BinarySymbol() {}
	
	virtual void setNonterminal(Ref<NonterminalDef> nonterminal);
	
	virtual Set  getChildren() const;
};

/* Chain **********************************************************************/

class Chain : public BinarySymbol {
public:
	virtual ~Chain() {}
	
	virtual bool                maybeEmpty();
	virtual bool                hasLeftRecursion(Ref<NonterminalDef> nonterminal);
	virtual set<Token::Type>    getFirst() = 0;
	virtual void                addFollow(Token::Type terminal) = 0;
	virtual void                calculateFollow();
};

/* Alternation ****************************************************************/

class Alternation : public BinarySymbol {
public:
	virtual ~Alternation() {}
	
	virtual bool                maybeEmpty();
	virtual bool                hasLeftRecursion(Ref<NonterminalDef> nonterminal);
	virtual set<Token::Type>    getFirst() = 0;
	virtual void                addFollow(Token::Type terminal) = 0;
	virtual void                calculateFollow();
};

/* NonterminalDef *************************************************************/

class NonterminalDef : public Object {
private:
	Grammar      * _grammar;
	int            _line;
	string         _name;
	Ref<GSymbol>   _symbol;

public:
	NonterminalDef(Grammar * grammar, int line, string name);
	virtual ~NonterminalDef() {}
	
	int getLine() { return _line; }
	
	bool hasLeftRecursion();
	
	string       getName() { return _name; }
	Ref<GSymbol> getSymbol() { return _symbol; }
	
	NonterminalDef& operator= (Ref<GSymbol> symbol);
};

#endif /* end of include guard: GSYMBOL_H_123AXSV0SDVG214323SFVDFV */

