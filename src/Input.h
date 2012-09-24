/**
 * \file   Input.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  
 */

#ifndef INPUT_H_234098SVF11098
#define INPUT_H_234098SVF11098

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Object.h"

using namespace std;

class Input : public Object {
public:
	Input() {}
	virtual ~Input() {}
	
	virtual istream& stream() = 0;
};

class StreamInput : public Input {
private:
	istream * _input;

public:
	StreamInput();
	StreamInput(istream * input);
	virtual ~StreamInput();
	
	virtual istream& stream() { return *_input; }
};

class FileInput : public Input {
private:
	string   _fileName;
	ifstream _input;

public:
	FileInput(string fileName);
	virtual ~FileInput();
	
	virtual istream& stream() { return _input; }
	
	string getBasename() const;
	string getExtension() const;
};

class BufferedInput : public Input {
private:
	string         _buffer;
	stringstream * _stream;

public:
	BufferedInput(Input& input);
	virtual ~BufferedInput();

	void rewind();
	
	virtual istream& stream() { return *_stream; }
};

#endif /* end of include guard: INPUT_H_234098SVF11098 */

