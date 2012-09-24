/**
 * \file   Output.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-28
 *
 * \brief  
 */

#include <fstream>

#include "Output.h"

FileOutput::FileOutput(string fileName)
	: Output(), _fileName(fileName), _output(NULL)
{
}

FileOutput::~FileOutput()
{
	close();
}

void FileOutput::open()
{
	close();
	_output = new ofstream(_fileName.c_str());
}

void FileOutput::close()
{
	if (_output) _output->close();
	delete _output;
	_output = NULL;
}

ostream& FileOutput::stream()
{
	if (_output == NULL) open();
	return *_output;
}

