/**
 * \file   Bytecode.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  
 */

#include <iomanip>

#include "Bytecode.h"

Bytecode::Bytecode()
	: Object()
{
	_address = 0;
}

Bytecode::Bytecode(const Bytecode &other)
	: _bytes(other._bytes), _address(other._address)
{
}

int Bytecode::write(char value)
{
	while (_address >= _bytes.size())
		_bytes.push_back(0);
	_bytes[_address++] = value;
	return 1;
}

int Bytecode::writeInstr(Instruction instr)
{
	int result = writeOpCode(instr.opcode);
	
	#define OC1(name, argument, type)                    \
		case Instruction::name:                           \
			result += write##type(instr.payload.argument); \
			break;
	
	switch (instr.opcode) {
	#include "opcodes.h"
	default: break;
	}
	
	return result;
}

int Bytecode::writeOpCode(Instruction::OpCode opcode)
{
	return write((char)opcode);
}

int Bytecode::writeInteger(Integer value)
{
	union {
		char    bytes[sizeof(Integer)];
		Integer value;
	} tmp;
	
	tmp.value = value;
	for (int i = 0; i < sizeof(Integer); i++)
		write(tmp.bytes[i]);
	
	return sizeof(Integer);
	
	/*
	bool negative = (value < 0);
	if (negative) value *= -1;
	
	if (isLittleEndian()) {
		write(address + 0, (value /       1) % BYTE(1));
		write(address + 1, (value / BYTE(1)) % BYTE(1));
		write(address + 2, (value / BYTE(2)) % BYTE(1));
		
		char highest = (value / BYTE(3)) % BYTE(1);

	} else {
	}
	*/
}

int Bytecode::writeUInteger(UInteger value)
{
	union {
		char     bytes[sizeof(UInteger)];
		UInteger value;
	} tmp;
	
	tmp.value = value;
	for (int i = 0; i < sizeof(UInteger); i++)
		write(tmp.bytes[i]);
	
	return sizeof(UInteger);
}

int Bytecode::writeAddress(Address value)
{
	return writeUInteger((UInteger)value);
}

char Bytecode::read()
{
	return _bytes[_address++];
}

Instruction Bytecode::readInstr()
{
	Instruction instr;
	
	instr.opcode = readOpCode();
	
	#define OC1(name, argument, type)            \
		case Instruction::name:                   \
			instr.payload.argument = read##type(); \
			break;
	
	switch (instr.opcode) {
	#include "opcodes.h"
	default: break;
	}
	
	return instr;
}

Instruction::OpCode Bytecode::readOpCode()
{
	return (Instruction::OpCode)read();
}

Integer Bytecode::readInteger()
{
	union {
		char    bytes[sizeof(Integer)];
		Integer value;
	} tmp;

	for (int i = 0; i < sizeof(Integer); i++)
		tmp.bytes[i] = read();
	
	return tmp.value;
	
	/*
	char bytes[sizeof(Integer)];
	
	for (int i = 0; i < sizeof(Integer); i++)
		bytes[i] = read(address + 1);

	Integer result = 0;
	
	result += ((Integer)bytes[0]);
	result += ((Integer)bytes[1]) * (1 ^ sizeof(char));
	result += ((Integer)bytes[2]) * (2 ^ sizeof(char));
	
	char highest = bytes[3];
	bool negative = (bool)(highest & 0x80);
	
	if (negative) highest = (~highest) + 1;
	
	result += ((Integer)highest) * (3 ^ sizeof(char));
	
	if (negative) result *= -1;
	
	return result;
	*/
}

UInteger Bytecode::readUInteger()
{
	union {
		char     bytes[sizeof(UInteger)];
		UInteger value;
	} tmp;

	for (int i = 0; i < sizeof(UInteger); i++)
		tmp.bytes[i] = read();
	
	return tmp.value;
	
	/*
	char bytes[sizeof(Integer)];
	for (int i = 0; i < sizeof(Integer); i++)
		bytes[i] = read(address + i);
	
	UInteger result = 0;
	
	result += ((UInteger)bytes[0]);
	result += ((UInteger)bytes[1]) * (1 ^ sizeof(char));
	result += ((UInteger)bytes[2]) * (2 ^ sizeof(char));
	result += ((UInteger)bytes[3]) * (3 ^ sizeof(char));
	
	return result;
	*/
}

Address Bytecode::readAddress()
{
	return (Address) readUInteger();
}

void Bytecode::write(ostream &output)
{
	foreach(c, _bytes)
		output.put(*c);
}

void Bytecode::read(istream &input)
{
	clear();
	int c;
	while (!input.eof() && ((c = input.get()) >= 0))
		write((char)c);
	rewind();
}

void Bytecode::read(Ref<Input> input)
{
	read(input->stream());
}

void Bytecode::dump(ostream &output)
{
	rewind();
	while (_address < _bytes.size()) {
		output << setw(8) << right << _address << "  ";
		readInstr().print(output);
		output << endl;
	}
}

bool Bytecode::isLittleEndian()
{
	union {
		char bytes[sizeof(int)];
		int  integer;
	} x;
	
	x.integer = 1;
	
	return (bool) x.bytes[0];
}

