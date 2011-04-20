/**
 * \file   mnemonics.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  
 */

#ifndef M
	#define M(name, nothing, integer, reference, address, register)
#endif

// MNEMONIC,     nothing,    integer,      reference,     address,       register
M( STOP,         STOP,       STOP,         STOP,          STOP,          STOP           )
M( NOOP,         NOOP,       NOOP,         NOOP,          NOOP,          NOOP           )
M( POP,          POP,        POP,          POP,           POP,           STORE_REGISTER )
M( DUP,          DUP,        NOOP,         NOOP,          NOOP,          NOOP           )
M( PRINT,        PRINT,      PRINT,        PRINT,         PRINT,         PRINT          )

M( CALL,         CALL,       NOOP,         NOOP,          NOOP,          NOOP           )
M( RETURN,       RETURN,     RETURN,       RETURN,        RETURN,        RETURN         )

M( PUSH,         NOOP,       PUSH,         PUSH,          PUSH,          LOAD_REGISTER  )
M( LOAD,         NOOP,       LOAD,         NOOP,          NOOP,          LOAD_REGISTER  )
M( STORE,        NOOP,       STORE,        NOOP,          NOOP,          STORE_REGISTER )

M( MAKE,         MAKE_LIST,  NOOP,         MAKE_FUNCTION, MAKE_FUNCTION, NOOP           )
M( LOAD_CLOSURE, NOOP,       LOAD_CLOSURE, NOOP,          NOOP,          NOOP           )
M( PULL,         NOOP,       PULL,         NOOP,          NOOP,          NOOP           )
M( ALLOC,        NOOP,       ALLOC,        NOOP,          NOOP,          NOOP           )

M( DUMP_STACK,   DUMP_STACK, DUMP_STACK,   DUMP_STACK,    DUMP_STACK,    DUMP_STACK     )

#undef M

