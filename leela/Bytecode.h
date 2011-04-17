/**
 * \file   Bytecode.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  
 */

#ifndef BYTECODE_H_FVDFV405968021SS
#define BYTECODE_H_FVDFV405968021SS

#include <vector>
#include <iostream>

#include "leela.h"
#include "Object.h"
#include "Instruction.h"
#include "Input.h"

#define BYTE(nr) (2 ^ (nr * 8 * sizeof(char)))

using namespace std;

class Bytecode : public Object {
private:
	vector<char> _bytes;
	Address      _address;

public:
	Bytecode();
	Bytecode(const Bytecode &other);
	virtual ~Bytecode() {}
	
	int                 size() const { return _bytes.size(); }
	Address             getAddress() const { return _address; }
	void                setAddress(Address value) { _address = value; }
	void                rewind() { setAddress(0); }
	void                clear() { _bytes.clear(); rewind(); }
	
	int                 write(         char                value  );
	int                 writeInstr(    Instruction         instr  );
	int                 writeOpCode(   Instruction::OpCode opcode );
	int                 writeInteger(  Integer             value  );
	int                 writeUInteger( UInteger            value  );
	int                 writeAddress(  Address             value  );
	
	char                read();
	Instruction         readInstr();
	Instruction::OpCode readOpCode();
	Integer             readInteger();
	UInteger            readUInteger();
	Address             readAddress();
	
	void                write(ostream &output);
	void                read(istream &input);
	void                read(Ref<Input> input);
	
	void                dump(ostream &output);
	
	static bool         isLittleEndian();
};

#endif /* end of include guard: BYTECODE_H_FVDFV405968021SS */

