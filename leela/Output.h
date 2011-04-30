/**
 * \file   Output.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-28
 *
 * \brief  
 */

#ifndef OUTPUT_H_12DFD019DTGBFV128957DF35AZ65G
#define OUTPUT_H_12DFD019DTGBFV128957DF35AZ65G

#include <ostream>

#include "Object.h"

class Output : public Object {
public:
	Output() {}
	virtual ~Output() {}
	
	virtual ostream& stream() = 0;
};

class FileOutput : public Output {
private:
	string     _fileName;
	ofstream * _output;

public:
	FileOutput(string fileName);
	virtual ~FileOutput();
	
	void open();
	void close();
	
	virtual ostream& stream() { return _output; }
};

class StdOutput : public Output {
public:
	StdOutput() {}
	virtual ~StdOutput() {}

	virtual ostream& stream() { return cout; }
}

#endif /* end of include guard: OUTPUT_H_12DFD019DTGBFV128957DF35AZ65G */

