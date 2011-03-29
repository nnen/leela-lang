/**
 * \file   leela/opcodes.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-28
 *
 * \brief  Contains op code definitions as an X-macro.
 */

#ifndef OC(name)
	#define OC(name)
#endif

#ifndef OC1(name)
	#define OC1(name)
#endif

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

