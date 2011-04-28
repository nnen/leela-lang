/**
 * \file   GSymbol.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-21
 *
 * \brief  
 */

#include <queue>

#include "GSymbol.h"
#include "Grammar.h"

/* GSymbol ********************************************************************/

void GSymbol::getStrongComponents(vector<GSymbol::Set>& components,
                                  stack<Ref<GSymbol> >& stack,
                                  int&                  nextIndex)
{
	_index = nextIndex++;
	_lowlink = _index;
	
	stack.push(this);
	
	Set children = getChildren();
	foreach (child, children) {
		if ((*child)->_index < 0) {
			(*child)->getStrongComponents(components, stack, nextIndex);
			_lowlink = (_lowlink < (*child)->_lowlink ? _lowlink : (*child)->_lowlink);
		} else {
			_lowlink = (_lowlink < (*child)->_index ? _lowlink : (*child)->_index);
		}
	}
	
	if (_index == _lowlink) {
		Ref<GSymbol> stackItem;
		Set result;
		do {
			stackItem = stack.top();
			stack.pop();
			result.insert(stackItem);
		} while (stackItem.getPtr() != this);
		components.push_back(result);
	}
}

GSymbol::GSymbol(Grammar * grammar, int line)
	: Object(), _grammar(grammar), _nonterminal(), _line(line), _index(-1), _lowlink(-1)
{
	
	
}

GSymbol::~GSymbol()
{
	_grammar = NULL;
}

/*
vector<GSymbol::Set> GSymbol::getStrongComponent()
{
	if (_index >= 0)
		return vector<Set>();
	
	int index = 0;
	vector<Set> components;
	stack<Ref<GSymbol> > stack;
	getStrongComponent(components, stack, index);
	return components;
}
*/

void GSymbol::addFollow(Token::Type terminal)
{
	set<Token::Type> terminals;
	terminals.insert(terminal);
	addFollow(terminals);
}

void GSymbol::addFollow(set<Token::Type> terminals)
{
	_follow.insert(terminals.begin(), terminals.end());
}

void GSymbol::print(ostream& output) const
{
	output << "[Symbol]";
}

Ref<GSymbol> operator+(Ref<GSymbol> a, Ref<GSymbol> b)
{
	return new Chain(a->getGrammar(), a->getLine(), a, b);
}

Ref<GSymbol> operator|(Ref<GSymbol> a, Ref<GSymbol> b)
{
	return new Alternation(a->getGrammar(), a->getLine(), a, b);
}

ostream& operator<<(ostream& output, const GSymbol& symbol)
{
	symbol.print(output);
	return output;
}

ostream& operator<<(ostream& output, Ref<GSymbol> symbol)
{
	output << *symbol;
	return output;
}

/* Terminal *******************************************************************/

Terminal::Terminal(Grammar * grammar, int line, Token::Type terminal)
	: GSymbol(grammar, line), _terminal(terminal), _epsilon(false)
{
}

Terminal::Terminal(Grammar * grammar, int line)
	: GSymbol(grammar, line), _terminal(Token::UNKNOWN), _epsilon(true)
{
}

Set<Token::Type> Terminal::getFirst()
{
	return Set<Token::Type>(_terminal);
}

void Terminal::print(ostream& output) const
{
	if (_epsilon)
		output << "epsilon";
	else
		output << Token::getRepresentation(_terminal);
}

/* Nonterminal ****************************************************************/

Nonterminal::Nonterminal(Grammar * grammar, int line)
	: GSymbol(grammar, line)
{
}

void Nonterminal::checkRecursion()
{
	queue<Ref<GSymbol> > queue;
	queue.push(this);
	
	while (queue.size() > 0) {
		if (queue.front() == this) {
			_recursive = true;
			return;
		}
		foreach (child, queue.front()->getChildren())
			queue.push(*child);
		queue.pop();
	}
	
	_recursive = false;
}

/* NonterminalRef *************************************************************/

NonterminalRef::NonterminalRef(Grammar * grammar, int line, string name)
	: Nonterminal(grammar, line), _name(name)
{
}

GSymbol::Set NonterminalRef::getChildren()
{
	Set set;
	set.insert(getDefinition()->getSymbol());
	return set;
}

bool NonterminalRef::maybeEmpty()
{
	return getDefinition()->getSymbol()->maybeEmpty();
}

bool NonterminalRef::hasLeftRecursion(Ref<NonterminalDef> nonterminal)
{
	if (getName() == nonterminal->getName()) return true;
	return getDefinition()->getSymbol()->hasLeftRecursion(nonterminal);
}

void NonterminalRef::addFollow(set<Token::Type> terminals)
{
	GSymbol::addFollow(terminals);
	getDefinition()->getSymbol()->addFollow(terminals);
}

set<Token::Type> NonterminalRef::getFirst()
{
	return getDefinition()->getSymbol()->getFirst();
}

void NonterminalRef::print(ostream& output) const
{
	output << _name;
}

Ref<NonterminalDef> NonterminalRef::getDefinition()
{
	if (_definition.isNull())
		_definition = _grammar->getNonterminal(_name);
	return _definition;
}

/* BinarySymbol ***************************************************************/

void BinarySymbol::setNonterminal(Ref<NonterminalDef> nonterminal)
{
	GSymbol::setNonterminal(nonterminal);
	_a->setNonterminal(nonterminal);
	_b->setNonterminal(nonterminal);
}

GSymbol::Set BinarySymbol::getChildren()
{
	Ref<GSymbol> children[] = { _a, _b };
	return Set(children, children + 2);
}

/* Chain **********************************************************************/

bool Chain::maybeEmpty()
{
	return _a->maybeEmpty() && _b->maybeEmpty();
}

bool Chain::hasLeftRecursion(Ref<NonterminalDef> nonterminal)
{
	if (_a->maybeEmpty())
		return (_a->hasLeftRecursion(nonterminal) || _b->hasLeftRecursion(nonterminal));
	return _a->hasLeftRecursion(nonterminal);
}

void Chain::calculateFollow()
{
	_a->addFollow(_b->getFirst());
	// _a->calculateFollow();
	// _b->calculateFollow();
}

Set<Token::Type> Chain::getFirst()
{
	Set<Token::Type> result = _a->getFirst();
	if (_a->maybeEmpty())
		result = result.merge(_b->getFirst());
	return result;
}

void Chain::print(ostream& output) const
{
	output << _a << " " << _b;
}

/* Alternation ****************************************************************/

bool Alternation::maybeEmpty()
{
	return _a->maybeEmpty() || _b->maybeEmpty();
}

bool Alternation::hasLeftRecursion(Ref<NonterminalDef> nonterminal)
{
	return (_a->hasLeftRecursion(nonterminal) || _b->hasLeftRecursion(nonterminal));
}

void Alternation::calculateFollow()
{
	// _a->calculateFollow();
	// _b->calculateFollow();
}

void Alternation::print(ostream& output) const
{
	output << _a << " | " << _b;
}

/* NonterminalDef *************************************************************/

NonterminalDef::NonterminalDef(Grammar * grammar, int line, string name)
	: Object(), _grammar(grammar), _line(line), _name(name)
{
	/* code */
}
bool NonterminalDef::hasLeftRecursion()
{
	return _symbol->hasLeftRecursion(this);
}

NonterminalDef& NonterminalDef::operator=(Ref<GSymbol> symbol)
{
	_symbol = symbol;
	symbol->setNonterminal(this);
	return *this;
}

