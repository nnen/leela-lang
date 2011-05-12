/**
 * \file   leela/Instruction.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-28
 *
 * \brief  Header file for the Instruction class.
 */

#ifndef INTRUCTION_H_23GDFVFG7JBQA
#define INTRUCTION_H_23GDFVFG7JBQA

#include <ostream>

#include "leela.h"

using namespace std;

struct Instruction {
public:
	enum OpCode {
		#define OC(name) name,
		#define OC1(name, argname, argtype) name,
		#include "opcodes.h"
		
		OPCODE_COUNT
	};
	
	union Payload {
		Integer integer;
		UInteger uinteger;
		Address address;
	};
	
	OpCode  opcode;
	Payload payload;
	
	Instruction();
	Instruction(OpCode opcode);
	Instruction(OpCode opcode, Integer  argument);
	Instruction(OpCode opcode, UInteger argument);
	~Instruction();
	
	bool         hasArgument() const;
	
	void         print(ostream &output) const;
	
	static bool  isOpCodeDefined(OpCode opcode);
	static bool  hasOpCodeArgument(OpCode opcode);
	static const char * getOpName(OpCode opcode);
};

#endif /* end of include guard: INTRUCTION_H_23GDFVFG7JBQA */

