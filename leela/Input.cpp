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

string FileInput::getBasename() const
{
	int length = _fileName.find_last_of(".");
	return _fileName.substr(0, length);
}

string FileInput::getExtension() const
{
	return _fileName.substr(_fileName.find_last_of(".") + 1);
}

BufferedInput::BufferedInput(Input& input)
	: Input(), _stream(NULL)
{
	stringstream s;
	while (input.stream().good())
		s.put(input.stream().get());
	_buffer = s.str();
	rewind();
}

BufferedInput::~BufferedInput()
{
	delete _stream;
	_stream = NULL;
}

void BufferedInput::rewind()
{
	delete _stream;
	_stream = new stringstream(_buffer);
}

