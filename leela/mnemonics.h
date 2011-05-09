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
M( RETURN,       RETURN,     RETURN,       RETURN,        RETURN,        RETURN         )
M( CALL,         NOOP,       CALL,         NOOP,          NOOP,          NOOP           )
M( BUILTIN,      NOOP,       BUILTIN,      NOOP,          NOOP,          NOOP           )
M( JUMP,         NOOP,       NOOP,         JUMP,          JUMP,          NOOP           )
M( JUMP_IF,      NOOP,       NOOP,         JUMP_IF,       JUMP_IF,       NOOP           )

M( PUSH,         PUSH_NONE,  PUSH,         PUSH_STRING,   PUSH_STRING,   NOOP           )
M( POP,          POP,        POP,          POP,           POP,           NOOP           )
M( DUP,          DUP,        NOOP,         NOOP,          NOOP,          NOOP           )

M( NOT,          NOT,        NOOP,         NOOP,          NOOP,          NOOP           )

M( ADD,          ADD,        NOOP,         NOOP,          NOOP,          NOOP           )
M( SUB,          SUB,        NOOP,         NOOP,          NOOP,          NOOP           )
M( MUL,          MUL,        NOOP,         NOOP,          NOOP,          NOOP           )
M( DIV,          DIV,        NOOP,         NOOP,          NOOP,          NOOP           )

M( PRINT,        PRINT,      PRINT,        PRINT,         PRINT,         PRINT          )
M( DUMP,         DUMP,       NOOP,         NOOP,          NOOP,          NOOP           )
M( DUMP_STACK,   NOOP,       DUMP_STACK,   NOOP,          NOOP,          NOOP           )

M( LOAD,         GET_ITEM,   LOAD,         NOOP,          NOOP,          NOOP           )
M( STORE,        SET_ITEM,   STORE,        NOOP,          NOOP,          NOOP           )
M( PULL,         NOOP,       PULL,         NOOP,          NOOP,          NOOP           )
M( ALLOC,        NOOP,       ALLOC,        NOOP,          NOOP,          NOOP           )

M( MAKE,         MAKE_TABLE, NOOP,         MAKE_FUNCTION, MAKE_FUNCTION, NOOP           )
M( LOAD_CLOSURE, NOOP,       LOAD_CLOSURE, NOOP,          NOOP,          NOOP           )

#undef M

