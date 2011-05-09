/**
 * \file   builtins.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-05-09
 *
 * \brief  Contains X-macro definitions of the builtin symbols.
 * \note   Do not insert include guards
 */

#ifndef BUILTIN
	#define BUILTIN( name, capname, expression )
#endif

BUILTIN( none,  NONE,  None::getInstance()               )
BUILTIN( true,  TRUE,  new Boolean(true)                 )
BUILTIN( false, FALSE, new Boolean(false)                )

BUILTIN( table, TABLE, new BuiltinFunction(makeTable, 0) )
BUILTIN( size,  SIZE,  new BuiltinFunction(getSize, 0)   )

#undef BUILTIN

