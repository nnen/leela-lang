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

// Program flow control
OC(  STOP                                )
OC(  NOOP                                )
OC(  RETURN                              )
OC1( CALL,           uinteger, UInteger  )
OC1( BUILTIN,        uinteger, UInteger  )
OC1( JUMP,           address,  Address   )
OC1( JUMP_IF,        address,  Address   )

// Stack operations
OC1( PUSH,           integer,  Integer   )
OC1( PUSH_STRING,    address,  Address   )
OC(  PUSH_NONE                           )
OC(  POP                                 )
OC(  DUP                                 )

// Unary operations
OC(  NOT                                 )

// Binary operations
OC(  ADD                                 )
OC(  SUB                                 )
OC(  MUL                                 )
OC(  DIV                                 )

// IO operations
OC(  PRINT                               )
OC(  DUMP                                )
OC1( DUMP_STACK,     uinteger, UInteger  )
OC(  DUMP_CONSTS                         )

// Variable operations
OC1( LOAD_CONST,     uinteger, UInteger  )
OC(  DEF_CONST                           )
OC1( LOAD,           uinteger, UInteger  )
OC1( STORE,          uinteger, UInteger  )
OC1( PULL,           uinteger, UInteger  )
OC1( ALLOC,          uinteger, UInteger  )

// Function operations
OC1( MAKE_FUNCTION,  address,  Address   )
OC1( LOAD_CLOSURE,   uinteger, UInteger  )

// Table operations
OC(  MAKE_TABLE                          )
OC(  SET_ITEM                            )
OC(  GET_ITEM                            )

#undef OC
#undef OC1

