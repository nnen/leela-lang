/**
 * \file   Assembler.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-15
 *
 * \brief  Implementation of the Assembler class.
 */

#include <sstream>
#include <exception>

#include "Assembler.h"

int Assembler::getLabelAddr(string label)
{
	if (_labels.count(label) < 1)
		_labels[label] = 0;
	return _labels[label];
}

void Assembler::setLabel()
{
	_labels[_scanner.label] = _bytecode.getAddress();
}

/*
void Assembler::write(char c)
{
	_addr++;
	if (_outputOn)
		_output->put(c);
}

void Assembler::writeInstr(Instruction::OpCode opcode)
{
	write((char) opcode);
}
*/

void Assembler::doUnexpectedToken(AsmScanner::Tokens token, string message)
{
	cerr << "ERROR: Unexpected token: " << AsmScanner::getTokenName(token) << "! ";
	cerr << message << endl;
	throw std::exception();
}

void Assembler::doUnexpectedToken(AsmScanner::Tokens token)
{
	doUnexpectedToken(token, "");
}

void Assembler::readInput()
{
	_bytecode.rewind();
	
	delete _input;
	_input = new istringstream(_inputStr);
	_scanner.reset(_input);
	
	try {
		while (_scanner.peekToken() != AsmScanner::TOKEN_END)
			readLine();
	} catch (std::exception e) {
	}
}

void Assembler::readLine()
{
	switch (_scanner.peekToken()) {
	case AsmScanner::TOKEN_LABEL:    readLabelLine();   break;
	default:                         readInstruction(); break;
	}
}

void Assembler::readLabelLine()
{
	readLabel();
	readInstruction();
}

void Assembler::readLabel()
{
	setLabel();
	_scanner.getToken();
}

void Assembler::readInstruction()
{
	AsmScanner::Tokens mnemonic = _scanner.getToken();
	
	#define M(m, nothing, int_op, ref_op, addr_op, reg_op)                               \
		case AsmScanner::TOKEN_##m:                                                       \
			switch (_scanner.peekToken()) {                                                \
				case AsmScanner::TOKEN_INTEGER:                                             \
					_bytecode.writeInstr(Instruction(Instruction::int_op, _scanner.integer)); \
					_scanner.getToken();                                                     \
					break;                                                                   \
				case AsmScanner::TOKEN_REFERENCE:                                           \
					_bytecode.writeInstr(Instruction(Instruction::ref_op, getLabelAddr(_scanner.label))); \
					_scanner.getToken();                                                     \
					break;                                                                   \
				case AsmScanner::TOKEN_ADDRESS:                                             \
					_bytecode.writeInstr(Instruction(Instruction::addr_op, _scanner.address)); \
					_scanner.getToken();                                                     \
					break;                                                                   \
				case AsmScanner::TOKEN_REGISTER:                                            \
					_bytecode.writeInstr(Instruction(Instruction::reg_op, _scanner.reg)); \
					_scanner.getToken();                                                     \
					break;                                                                   \
				default:                                                                    \
					_bytecode.writeInstr(Instruction::nothing);                              \
					break;                                                                   \
			}                                                                              \
			break;
	
	switch (mnemonic) {
	#include "mnemonics.h"
	default:
		doUnexpectedToken(mnemonic);
		break;
	}
}

void Assembler::setInput(istream &input)
{
	_inputStr = "";
	while (!input.eof()) {
		char buff[1024];
		input.read(buff, sizeof(buff) - 1);
		buff[input.gcount()] = '\0';
		_inputStr += buff;
	}
	
	delete _input;
	_input = NULL;
	
	_input = new istringstream(_inputStr);
	_scanner.reset(_input);
}

Assembler::Assembler()
{
	_input = NULL;
}

Assembler::~Assembler()
{
	// if (_input) _input->close();
	delete _input;
	_input = NULL;
}

Bytecode Assembler::assemble(istream &input)
{
	_labels.clear();
	_bytecode.clear();
	
	setInput(input);
	
	readInput();
	readInput();
	
	return _bytecode;
}

