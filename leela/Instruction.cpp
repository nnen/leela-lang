/**
 *
 */

#include "Instruction.h"

Instruction::Instruction()
{
	setOpcode(NOOP);
	setUInt(0);
}

Instruction::Instruction(Instruction::OpCode opcode)
{
	setOpcode(opcode);
	setUInt(0);
}

Instruction::Instruction(Instruction::OpCode opcode, unsigned int argument)
{
	setOpcode(opcode);
	setUInt(argument);
}

Instruction::~Instruction()
{
	setOpcode(NOOP);
	setUInt(0);
}

Instruction& Instruction::setOpcode(Instruction::OpCode value)
{
	payload.parts.opcode = (uint8_t) value;
	return *this;
}

Instruction& Instruction::setUInt(unsigned int value)
{
	payload.parts.argLow  = (uint16_t) value;
	payload.parts.argHigh = (uint8_t) (value / (UINT16_MAX + 1));
	return *this;
}

Instruction::OpCode Instruction::getOpcode() const
{
	return (Instruction::OpCode) payload.parts.opcode;
}

unsigned int Instruction::getUInt() const
{
	return (int) payload.parts.argLow + (int) ((UINT16_MAX + 1) * payload.parts.argHigh);
}

bool Instruction::hasArgument() const
{
	return hasOpCodeArgument(getOpcode());
}

bool Instruction::hasOpCodeArgument(OpCode opcode)
{
	switch (opcode) {
	#define OC1(name, argname) case name:
	#include "opcodes.h"
		return true;
	default:
		return false;
	}
}

