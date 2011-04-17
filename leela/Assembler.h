/**
 * \file   Assembler.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-15
 *
 * \brief  Header file for the Assembler class.
 */

#ifndef ASSEMBLER_H_2389FDVD1209CSA
#define ASSEMBLER_H_2389FDVD1209CSA

#include <iostream>
#include <queue>

#include "Object.h"
#include "AsmScanner.h"
#include "Bytecode.h"

using namespace std;

class Assembler : public Object {
private:
	string                      _inputStr;
	istream                   * _input;
	
	AsmScanner                  _scanner;
	Bytecode                    _bytecode;
	
	unsigned int                _addr;
	map<string, unsigned int>   _labels;
	
	int  getLabelAddr(string label);
	void setLabel();
	
	/*
	void write(char c);
	void writeInstr(Instruction::OpCode opcode);
	void writeIntegerInstr(Instruction::OpCode opcode, Integer integer);
	void writeAddressInstr(Instruction::OpCode opcode, Address address);
	void writeUIntegerInstr(Instruction::OpCode opcode, UInteger integer);
	*/
	void doUnexpectedToken(AsmScanner::Tokens token, string message);
	void doUnexpectedToken(AsmScanner::Tokens token);
	
	void readInput();
	void readLine();
	void readLabelLine();
	void readLabel();
	void readInstruction();
	
	void setInput(istream &input);

public:
	Assembler();
	// Assembler(istream * input, ostream * output);
	virtual ~Assembler();
	
	Bytecode assemble(istream &input);
};

#endif /* end of include guard: ASSEMBLER_H_2389FDVD1209CSA */

