/**
 * \file   leela/Instruction.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-28
 *
 * \brief  Header file for the Instruction class.
 */

#ifndef INTRUCTION_H_23GDFVFG7JBQA
#define INTRUCTION_H_23GDFVFG7JBQA

#define INSTRUCTION_WIDTH 4

// #include <cstdint>
#include <stdint.h>

struct Instruction {
public:
	enum OpCode {
		#define OC(name) name,
		#define OC1(name, argname) name,
		#include "opcodes.h"
		
		OPCODE_COUNT
	};
	
	union {
		uint8_t  bytes[INSTRUCTION_WIDTH];
		
		struct {
			uint8_t  opcode;
			uint8_t  argHigh;
			uint16_t argLow;
		} parts;
	} payload;
	
	Instruction();
	Instruction(OpCode opcode);
	Instruction(OpCode opcode, unsigned int argument);
	~Instruction();
	
	Instruction& setOpcode(OpCode value);
	Instruction& setUInt(unsigned int value);
	
	OpCode       getOpcode() const;
	unsigned int getUInt() const;

	bool         hasArgument() const;

	static bool  hasOpCodeArgument(OpCode opcode);
};

#endif /* end of include guard: INTRUCTION_H_23GDFVFG7JBQA */

