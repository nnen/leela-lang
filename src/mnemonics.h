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

// MNEMONIC,      nothing,       integer,      reference,     address,       register
M( STOP,          STOP,          STOP,         STOP,          STOP,          STOP           )
M( NOOP,          NOOP,          NOOP,         NOOP,          NOOP,          NOOP           )
M( RETURN,        RETURN,        RETURN,       RETURN,        RETURN,        RETURN         )
M( CALL,          NOOP,          CALL,         NOOP,          NOOP,          NOOP           )
M( JUMP,          NOOP,          NOOP,         JUMP,          JUMP,          NOOP           )
M( JUMP_IF,       NOOP,          NOOP,         JUMP_IF,       JUMP_IF,       NOOP           )

M( PUSH,          PUSH_NONE,     PUSH,         PUSH_STRING,   PUSH_STRING,   NOOP           )
M( POP,           POP,           POP,          POP,           POP,           NOOP           )
M( DUP,           DUP,           NOOP,         NOOP,          NOOP,          NOOP           )

M( NOT,           NOT,           NOOP,         NOOP,          NOOP,          NOOP           )

M( ADD,           ADD,           NOOP,         NOOP,          NOOP,          NOOP           )
M( SUB,           SUB,           NOOP,         NOOP,          NOOP,          NOOP           )
M( MUL,           MUL,           NOOP,         NOOP,          NOOP,          NOOP           )
M( DIV,           DIV,           NOOP,         NOOP,          NOOP,          NOOP           )

M( EQUALS,        EQUALS,        NOOP,         NOOP,          NOOP,          NOOP           )
M( LESS,          LESS,          NOOP,         NOOP,          NOOP,          NOOP           )
M( GREATER,       GREATER,       NOOP,         NOOP,          NOOP,          NOOP           )
M( LESS_OR_EQ,    LESS_OR_EQ,    NOOP,         NOOP,          NOOP,          NOOP           )
M( GREATER_OR_EQ, GREATER_OR_EQ, NOOP,         NOOP,          NOOP,          NOOP           )

M( AND,           AND,           NOOP,         NOOP,          NOOP,          NOOP           )
M( OR,            OR,            NOOP,         NOOP,          NOOP,          NOOP           )

M( PRINT,         PRINT,         PRINT,        PRINT,         PRINT,         PRINT          )
M( DUMP,          DUMP,          NOOP,         NOOP,          NOOP,          NOOP           )
M( DUMP_STACK,    NOOP,          DUMP_STACK,   NOOP,          NOOP,          NOOP           )
M( DUMP_CONSTS,   DUMP_CONSTS,   NOOP,         NOOP,          NOOP,          NOOP           )

M( LOAD_CONST,    NOOP,          LOAD_CONST,   NOOP,          NOOP,          NOOP           )
M( DEF_CONST,     DEF_CONST,     NOOP,         NOOP,          NOOP,          NOOP           )
M( LOAD,          GET_ITEM,      LOAD,         NOOP,          NOOP,          NOOP           )
M( STORE,         SET_ITEM,      STORE,        NOOP,          NOOP,          STORE_R0       )
M( ALLOC,         NOOP,          ALLOC,        NOOP,          NOOP,          NOOP           )

M( MAKE,          MAKE_TABLE,    NOOP,         MAKE_FUNCTION, MAKE_FUNCTION, NOOP           )
M( LOAD_CLOSURE,  NOOP,          LOAD_CLOSURE, NOOP,          NOOP,          NOOP           )

#undef M

