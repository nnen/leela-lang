/**
 * \file   Machine.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  Implementation of the {@link Machine} class.
 */

#include "Machine.h"

RuntimeError::RuntimeError(Machine &machine, string message)
{
	if (machine.getCallStack().size() < 1)
		_address = -1;
	else
		_address = machine.getCallStack().back()->instrCounter;
	_message = message;
}

void RuntimeError::print(ostream &output) const
{
	output << "RuntimeError (@" << _address << "): " << _message;
}

Instruction Machine::loadInstr()
{
	if (_callStack.size() < 1)
		return Instruction(Instruction::STOP);
	
	_code->setAddress(_callStack.back()->instrCounter);
	Instruction instr = _code->readInstr();
	_callStack.back()->instrCounter = _code->getAddress();
	
	return instr;
}

void Machine::execute(Instruction instr)
{
	switch (instr.opcode) {
	case Instruction::STOP:
		_stop = true;
		break;
	case Instruction::PRINT:
		cout << *pop() << endl;
		break;
	
	case Instruction::PUSH:
		push(new Number(instr.payload.integer));
		break;
	
	default:
		throw RuntimeError(*this, "Unknown or unimplemented instruction code.");
		break;
	}
}

void Machine::push(Ref<Value> value)
{
	_callStack.back()->push(value);
}

Ref<Value> Machine::pop()
{
	if (_callStack.size() < 1) return Ref<Value>();
	return _callStack.back()->pop();
}

Machine::Machine()
{
	reset();
}

Machine::~Machine()
{
}

bool Machine::step()
{
	if (_stop) return false;
	if (_callStack.size() < 1) return false;
	
	try {
		Instruction instr = loadInstr();
		execute(instr);
	} catch (const RuntimeError &e) {
		e.print(cerr);
		cerr << endl;
		
		_stop = true;
	}
	
	return true;
}

void Machine::reset()
{
	_callStack.clear();
	
	Ref<Function> bootLoader = new Function(0);
	_callStack.push_back(bootLoader->activate());
	
	_stop = false;
}

void Machine::run()
{
	reset();
	while (step());
}

void Machine::loadBytecode(Ref<Input> input)
{
	if (_code.isNull())
		_code = new Bytecode();
	
	_code->read(input);
	reset();
}

