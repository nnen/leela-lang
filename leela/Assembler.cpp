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

Instruction::OpCode Assembler::getOpCode(AsmScanner::Tokens mnemonic, AsmScanner::Tokens argument)
{
	#define M(m, nothing, int_op, ref_op, addr_op, reg_op)                       \
		case AsmScanner::TOKEN_##m:                                             \
			switch (argument) {                                                \
				case AsmScanner::TOKEN_INTEGER: return Instruction::int_op;   \
				case AsmScanner::TOKEN_REFERENCE: return Instruction::ref_op; \
				case AsmScanner::TOKEN_ADDRESS: return Instruction::addr_op;  \
				case AsmScanner::TOKEN_REGISTER: return Instruction::reg_op;  \
				default: return Instruction::nothing;                         \
			}                                                                  \
			break;
	
	switch (mnemonic) {
	#include "mnemonics.h"
	default: return Instruction::NOOP;
	}
}

void Assembler::doUnexpectedToken(AsmScanner::Tokens token, string message)
{
	cerr << "ERROR: Unexpected token: " << AsmScanner::getTokenName(token) << ". ";
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
	Instruction instr(getOpCode(mnemonic, _scanner.peekToken()));
	
	if (instr.opcode == Instruction::NOOP && mnemonic != AsmScanner::TOKEN_NOOP)
		cerr << "Warning: Mnemonic translated as NOOP at "
		     << _bytecode.getAddress() << "." << endl;
	
	#define M(m, nothing, int_op, ref_op, addr_op, reg_op)                                         \
		case AsmScanner::TOKEN_##m:                                                               \
			switch (_scanner.peekToken()) {                                                      \
				case AsmScanner::TOKEN_INTEGER:                                                 \
					instr.payload.integer = _scanner.integer;                                  \
					_scanner.getToken();                                                       \
					break;                                                                     \
				case AsmScanner::TOKEN_REFERENCE:                                               \
					instr.payload.address = getLabelAddr(_scanner.reference);                  \
					_scanner.getToken();                                                       \
					break;                                                                     \
				case AsmScanner::TOKEN_ADDRESS:                                                 \
					instr.payload.address = _scanner.address;                                \
					_scanner.getToken();                                                     \
					break;                                                                   \
				case AsmScanner::TOKEN_REGISTER:                                              \
					instr.payload.uinteger = _scanner.reg;                                   \
					_scanner.getToken();                                                     \
					break;                                                                   \
				default:                                                                      \
					break;                                                                   \
			}                                                                                  \
			_bytecode.writeInstr(instr);                                                       \
			break;
	
	switch (mnemonic) {
	#include "mnemonics.h"
	default:
		doUnexpectedToken(mnemonic, "Expected a mnemonic.");
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
