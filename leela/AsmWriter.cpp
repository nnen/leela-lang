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
	return *_openChunks.top();
}

void AsmWriter::clear()
{
	while (!_openChunks.empty()) {
		stringstream* s = _openChunks.top();
		delete s;
		_openChunks.pop();
	}
	while (!_closedChunks.empty())
		_closedChunks.pop();
	while (!_labels.empty())
		_labels.pop();
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

void AsmWriter::writeMnemonic(AsmScanner::Tokens mnemonic)
{
	currentChunk()
		<< "    "
		<< setw(AsmScanner::getLongestMnemonic()) << left
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
	currentChunk() << integer << endl;
}

void AsmWriter::writeInstruction(AsmScanner::Tokens mnemonic, UInteger uinteger)
{
	writeLabels();
	writeMnemonic(mnemonic);
	currentChunk() << uinteger << endl;
}

void AsmWriter::writeInstruction(AsmScanner::Tokens mnemonic, string reference)
{
	writeLabels();
	writeMnemonic(mnemonic);
	currentChunk() << reference << endl;
}

void AsmWriter::startChunk()
{
	_openChunks.push(new stringstream());
}

void AsmWriter::endChunk()
{
	stringstream * stream = _openChunks.top();
	_openChunks.pop();
	_closedChunks.push(stream->str());
	delete stream;
	stream = NULL;
}

void AsmWriter::output(ostream &output)
{
	output << "; Chunk count: " << _closedChunks.size() << endl;
	
	while (!_closedChunks.empty()) {
		output << _closedChunks.top();
		_closedChunks.pop();
	}
}

void AsmWriter::output(Ref<Output> output)
{
	this->output(output->stream());
}

