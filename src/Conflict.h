/**
 * \file   Conflict.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-22
 *
 * \brief  
 */

#ifndef CONFLICT_H_09SACVR0912FTGF
#define CONFLICT_H_09SACVR0912FTGF

#include "Object.h"
#include "GSymbol.h"

/* Conflict *******************************************************************/

class Conflict : public Object {
protected:
	Ref<GSymbol> _origin;

public:
	Conflict(Ref<GSymbol> origin);
	virtual ~Conflict() {}
};

/* LeftRecursion **************************************************************/

class LeftRecursion : public Conflict {
private:
public:
	LeftRecursion(Ref<GSymbol> origin);
	virtual ~LeftRecursion() {}
};

/* FirstFirst *****************************************************************/

class FirstFirst : public Conflict {
private:
	Token::Type _first;

public:
	FirstFirst(Ref<GSymbol> origin, Token::Type first);
	virtual ~FirstFirst() {}
};

/* FirstFollow ****************************************************************/

class FirstFollow : public Conflict {
private:
	Token::Type _first;

public:
	FirstFollow(Ref<GSymbol> origin, Token::Type first);
	virtual ~FirstFollow() {}
};

#endif /* end of include guard: CONFLICT_H_09SACVR0912FTGF */

