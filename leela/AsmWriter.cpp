/**
 * \file   AsmWriter.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-19
 *
 * \brief  Implementation file for the {@link AsmWriter} class.
 */

#include "AsmWriter.h"

ostream& AsmWriter::currentChunk()
{
	return *_openChunks.top();
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

void AsmWriter::writeInstruction(AsmScanner::Tokens mnemonic)
{
	writeLabels();
	currentChunk() 
		<< "    "
		<< setw(AsmScanner::getLongestMnemonic() << left
		<< AsmScanner::getMnemonic(mnemonic) << endl;
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
	while (!_closedChunks.empty()) {
		output << _closedChunks.top();
		_closedChunks.pop();
	}
}

