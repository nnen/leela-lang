/**
 * \file   Input.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  
 */

#include "Input.h"

StreamInput::StreamInput()
	: Input(), _input(&cin)
{
}

StreamInput::StreamInput(istream * input)
	: Input(), _input(input)
{
}

StreamInput::~StreamInput()
{
	_input = NULL;
}

FileInput::FileInput(string fileName)
	: Input(), _fileName(fileName), _input(fileName.c_str())
{
}

FileInput::~FileInput()
{
	_input.close();
}

string FileInput::getExtension() const
{
	return _fileName.substr(_fileName.find_last_of(".") + 1);
}

BufferedInput::BufferedInput(Input& input)
{
	while (input.stream().good())
		_stream.put(input.stream().get());
	rewind();
}

void BufferedInput::rewind()
{
	// _stream.seekg(0, ios::beg);
	_stream.seekg(0);
}

