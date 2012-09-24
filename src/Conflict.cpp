/**
 * \file   Conflict.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-22
 *
 * \brief  
 */

#include "Conflict.h"

/* Conflict *******************************************************************/

Conflict::Conflict(Ref<GSymbol> origin)
	: Object(), _origin(origin)
{
}

/* LeftRectusion **************************************************************/

LeftRecursion::LeftRecursion(Ref<GSymbol> origin)
	: Conflict(origin)
{
}

/* FirstFirst *****************************************************************/

FirstFirst::FirstFirst(Ref<GSymbol> origin, Token::Type first)
	: Conflict(origin), _first(first)
{
}

/* FirstFollow ****************************************************************/

FirstFollow::FirstFollow(Ref<GSymbol> origin, Token::Type first)
	: Conflict(origin), _first(first)
{
}

