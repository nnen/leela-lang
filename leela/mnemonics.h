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

// MNEMONIC, nothing, integer, reference, address, register
M( STOP,     STOP,    STOP,    STOP,      STOP,    STOP           )
M( NOOP,     NOOP,    NOOP,    NOOP,      NOOP,    NOOP           )
M( POP,      POP,     POP,     POP,       POP,     STORE_REGISTER )
M( PRINT,    PRINT,   PRINT,   PRINT,     PRINT,   PRINT          )

M( PUSH,     NOOP,    PUSH,    PUSH,      PUSH,    LOAD_REGISTER  )
M( LOAD,     NOOP,    LOAD,    LOADP,     LOADP,   LOAD_REGISTER  )
M( STORE,    NOOP,    STORE,   STOREP,    STOREP,  STORE_REGISTER )

#undef M

