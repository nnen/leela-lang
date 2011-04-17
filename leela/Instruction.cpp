/**
 *
 */

#include <iomanip>

#include "Instruction.h"

Instruction::Instruction()
	: opcode(NOOP)
{
	payload.uinteger = 0;
}

Instruction::Instruction(Instruction::OpCode opcode)
	: opcode(opcode)
{
	payload.uinteger = 0;
}

Instruction::Instruction(Instruction::OpCode opcode, Integer argument)
	: opcode(opcode)
{
	payload.integer = argument;
}

Instruction::Instruction(Instruction::OpCode opcode, UInteger argument)
	: opcode(opcode)
{
	payload.uinteger = argument;
}

Instruction::~Instruction()
{
	opcode = NOOP;
	payload.uinteger = 0;
}

bool Instruction::hasArgument() const
{
	return hasOpCodeArgument(opcode);
}

void Instruction::print(ostream &output) const
{
	output << setw(8) << left << getOpName(opcode) << setw(8) << right;
	
	switch (opcode) {
	#define OC1(name, argname, argtype) case name: output << payload.argname; break;
	#include "opcodes.h"
	default: break;
	}
}

bool Instruction::hasOpCodeArgument(OpCode opcode)
{
	switch (opcode) {
	#define OC1(name, argname, argtype) case name:
	#include "opcodes.h"
		return true;
	default:
		return false;
	}
}

const char * Instruction::getOpName(OpCode opcode)
{
	switch (opcode) {
	#define OC(name) case name: return #name;
	#define OC1(name, argname, argtype) case name: return #name;
	#include "opcodes.h"
	default: return "[NaO]";
	}
}


