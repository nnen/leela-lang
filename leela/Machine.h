/**
 * \file   Machine.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  Header file for the {@link Machine} class.
 */

#ifndef MACHINE_H_FVZAS0983YUA2VFDV6
#define MACHINE_H_FVZAS0983YUA2VFDV6

#include <exception>
#include <string>
#include <vector>

#include "leela.h"
#include "Value.h"
#include "Function.h"
#include "Bytecode.h"
#include "Input.h"

using namespace std;

class Machine;

class RuntimeError : public exception {
private:
	Address _address;
	string  _message;

public:
	RuntimeError(Machine &machine, string message);
	virtual ~RuntimeError() throw () { }
	
	Address getAddress() const { return _address; }
		
	void print(ostream &output) const;
};

class Machine {
public:
	typedef vector<Ref<ActivationFrame> > CallStack;

private:
	Ref<Bytecode>                _code;
	CallStack                    _callStack;
	
	Instruction                  _instr;
	bool                         _stop;
	
	void        do_PULL(Instruction instr);
	
	Instruction loadInstr();
	void        execute(Instruction instr);
	
	void        push(Ref<Value> value);
	Ref<Value>  pop();

public:
	Machine();
	virtual ~Machine();
	
	Instruction          getInstr() const { return _instr; }
	
	Ref<ActivationFrame> getFrame();
	void                 pushFrame(Ref<ActivationFrame> frame);
	Ref<ActivationFrame> popFrame();
	
	CallStack&           getCallStack() { return _callStack; }
	
	bool step();
	void reset();
	void run();
	
	void loadBytecode(Ref<Input> input);

	void dumpStack(ostream &output, int limit = 10) const;
};

#endif /* end of include guard: MACHINE_H_FVZAS0983YUA2VFDV6 */

