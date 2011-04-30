/**
 * \file   AsmWriter.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-19
 *
 * \brief  Header file for the {@link AsmWriter} class.
 */

#ifndef ASM_WRITER_H_0192SDCCSBG48QWQW
#define ASM_WRITER_H_0192SDCCSBG48QWQW

#include <stack>
#include <iostream>
#include <sstream>
#include <queue>

#include "Object.h"
#include "AsmScanner.h"
#include "Output.h"

class AsmWriter : public Object {
private:
	stack<stringstream*> _openChunks;
	stack<string>        _closedChunks;
	queue<string>        _labels;

	ostream&             currentChunk();
	
	void writeLabels();
	void writeMnemonic(AsmScanner::Tokens mnemonic);

public:
	AsmWriter() : Object() { startChunk(); }
	virtual ~AsmWriter() {}
	
	void clear();
	
	void writeLabel(string label);
	void writeInstruction(AsmScanner::Tokens mnemonic);
	void writeInstruction(AsmScanner::Tokens mnemonic, Integer integer);
	void writeInstruction(AsmScanner::Tokens mnemonic, UInteger uinteger);
	void writeInstruction(AsmScanner::Tokens mnemonic, string reference);
	
	void startChunk();
	void endChunk();
	
	void output(ostream &output);
	void output(Ref<Output> output);
};

#endif /* end of include guard: ASM_WRITER_H_0192SDCCSBG48QWQW */

