/**
 * \file   leela/opcodes.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-28
 *
 * \brief  Contains op code definitions as an X-macro.
 *
 * \note   Do NOT put include guards in this file.
 */

#ifndef OC
	#define OC(name)
#endif

#ifndef OC1
	#define OC1(name, argument, type)
#endif

/*
OC(  NOOP ) // a -> memory[a]
OC(  DR   ) // a -> memory[a]
OC(  UNM  ) // a -> -a
OC(  ST   ) // a b ->        (memory[a] = b)
OC(  WRT  ) // a ->          (prints a)
OC(  STOP ) // ->            (stops execution)

OC1( TC,  constant  ) // -> c          (pushes constant on top of stack)
OC1( TA,  address   ) // -> a          (pushes address on top of stack, same as TC for some reason)
OC1( BOP, operation ) // a b -> c      (binary operation)
OC1( JU,  address   ) // ->            (unconditional jump)
OC1( IFJ, address   ) // a ->          (conditional jump, if a is 0)
*/

OC( STOP          )
OC( NOOP          )
OC( POP           )
OC( DUP           )
OC( PRINT         )

OC( CALL          )
OC( RETURN        )

OC( MAKE_LIST     )
OC( APPEND        )
OC( EXTEND        )
OC( INSERT        )
OC( REMOVE        )
OC( CLEAR_LIST    )

OC( DUMP          )
OC( DUMP_STACK    )

OC1( PUSH,           integer,  Integer  )
OC1( LOAD,           uinteger, UInteger )
OC1( LOAD_GLOBAL,    uinteger, UInteger )
OC1( LOAD_REGISTER,  uinteger, UInteger )
OC1( STORE,          uinteger, UInteger )
OC1( STORE_GLOBAL,   uinteger, UInteger )
OC1( STORE_REGISTER, uinteger, UInteger )

OC1( MAKE_FUNCTION,  address,  Address  )
OC1( LOAD_CLOSURE,   uinteger, UInteger )
OC1( PULL,           uinteger, UInteger )
OC1( ALLOC,          uinteger, UInteger )
// OC1( CALL,           uinteger, UInteger )

#undef OC
#undef OC1

