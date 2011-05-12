/**
 * \file   AsmWriter.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-19
 *
 * \brief  Implementation file for the {@link AsmWriter} class.
 */

#include <iomanip>

#include "AsmWriter.h"

ostream& AsmWriter::currentChunk()
{
	if (_openChunks.empty())
		startChunk();
	return *_openChunks.top();
}

void AsmWriter::clear()
{
	while (!_openChunks.empty()) {
		stringstream* s = _openChunks.top();
		delete s;
		_openChunks.pop();
	}
	_closedChunks.clear();
	//while (!_closedChunks.empty())
	//	_closedChunks.pop();
	while (!_labels.empty())
		_labels.pop();
	while (!_labelStack.empty())
		_labelStack.pop();
	_nextLabel = 0;
}

void AsmWriter::writeLabels()
{
	while (!_labels.empty()) {
		currentChunk() << _labels.front() << ":" << endl;
		_labels.pop();
	}
}

void AsmWriter::writeLabel(string label)
{
	_labels.push(label);
}

void AsmWriter::writeLabel(string label, int number)
{
	stringstream s;
	s << label << "_" << number;
	writeLabel(s.str());
}

void AsmWriter::writeMnemonic(AsmScanner::Tokens mnemonic)
{
	currentChunk()
		<< "    "
		<< setw(AsmScanner::getLongestMnemonic() + 1) << left
		<< AsmScanner::getMnemonic(mnemonic);
}

void AsmWriter::writeInstruction(AsmScanner::Tokens mnemonic)
{
	writeLabels();
	writeMnemonic(mnemonic);
	currentChunk() << endl;
}

void AsmWriter::writeInstruction(AsmScanner::Tokens mnemonic, Integer integer)
{
	writeLabels();
	writeMnemonic(mnemonic);
	currentChunk() << "   " << integer << endl;
}

void AsmWriter::writeInstruction(AsmScanner::Tokens mnemonic, UInteger uinteger)
{
	writeLabels();
	writeMnemonic(mnemonic);
	currentChunk() << "   " << uinteger << endl;
}

void AsmWriter::writeInstruction(AsmScanner::Tokens mnemonic, string reference)
{
	writeLabels();
	writeMnemonic(mnemonic);
	currentChunk() << "   " << reference << endl;
}

void AsmWriter::writeInstruction(AsmScanner::Tokens mnemonic, UInteger reg, string reference)
{
	writeLabels();
	writeMnemonic(mnemonic);
	currentChunk() << "#" << (char)('0' + reg) << " " << reference << endl;
}

void AsmWriter::write(Ref<String> string)
{
	writeLabels();
	currentChunk() << "    ";
	string->repr(currentChunk());
	currentChunk() << endl;
}

void AsmWriter::writeComment(string comment)
{
	currentChunk() << "; " << comment << endl;
}

void AsmWriter::startChunk()
{
	writeLabels();
	_openChunks.push(new stringstream());
	_children.push(vector<stringstream*>());
}

void AsmWriter::endChunk()
{
	stringstream * chunk = _openChunks.top();
	_openChunks.pop();

	vector<stringstream*> children = _children.top();
	_children.pop();

	foreach (child, children) {
		*chunk << endl << (*child)->str();
		delete *child;
	}

	if (_openChunks.empty()) {
		_closedChunks.push_back(chunk->str());
		delete chunk;
	} else {
		_children.top().push_back(chunk);
	}
}

void AsmWriter::endAll()
{
	while (!_openChunks.empty())
		endChunk();
}

string AsmWriter::pushLabel(string prefix)
{
	stringstream s;
	s << prefix << "_" << _nextLabel++;
	_labelStack.push(s.str());
	// writeLabel(s.str());
	return s.str();
}

string AsmWriter::popLabel()
{
	string label = _labelStack.top();
	_labelStack.pop();
	return label;
}

void AsmWriter::popLabel(AsmScanner::Tokens mnemonic)
{
	writeInstruction(mnemonic, _labelStack.top());
	_labelStack.pop();
}

void AsmWriter::makeFunction(UInteger argCount)
{
	writeInstruction(
		AsmScanner::TOKEN_PUSH,
		argCount
	);
	writeInstruction(
		AsmScanner::TOKEN_MAKE,
		_labelStack.top()
	);
	_labelStack.pop();
}

void AsmWriter::output(ostream &output)
{
	endAll();
	
	output << "; Chunk count: " << _closedChunks.size() << endl;

	foreach (chunk, _closedChunks)
		output << endl << *chunk;
}

void AsmWriter::output(Ref<Output> output)
{
	this->output(output->stream());
}

