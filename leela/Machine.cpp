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
	
	Ref<ActivationFrame> frame = getFrame();
	
	_code->setAddress(frame->instrCounter);
	Instruction instr = _code->readInstr();
	frame->instrCounter = _code->getAddress();
	
	return instr;
}

void Machine::execute(Instruction instr)
{
	_instr = instr;
	
	switch (instr.opcode) {
	case Instruction::STOP:
		_stop = true;
		break;
	case Instruction::NOOP:
		break;
	case Instruction::POP:
		pop();
		break;
	case Instruction::CALL: {
		Ref<Function> fn = pop().as<Function>();
		pushFrame(fn->activate());
		} break;
	case Instruction::RETURN: {
		Ref<Value> val = pop();
		popFrame();
		push(val);
		} break;
	case Instruction::PRINT:
		cout << *pop() << endl;
		break;
	
	case Instruction::DUMP_STACK:
		dumpStack(cout);
		break;
	
	case Instruction::PUSH:
		push(new Number(instr.payload.integer));
		break;
	case Instruction::LOAD:
		push(getFrame()->getVar(instr.payload.uinteger)->getValue());
		break;
	case Instruction::STORE:
		getFrame()->getVar(instr.payload.uinteger)->setValue(pop());
		break;
	
	case Instruction::MAKE_FUNCTION:
		push(new Function(instr.payload.address));
		break;
	case Instruction::LOAD_CLOSURE: {
		Ref<Function> function = pop().as<Function>();
		if (function.isNull())
			throw RuntimeError(*this, "Expected function on top of the stack.");
		function->pushClosure(getFrame()->getVar(instr.payload.uinteger));
		push(function);
		} break;
	case Instruction::ALLOC: {
		for (int i = 0; i < (int) instr.payload.uinteger; i++)
			getFrame()->pushVariable();
		} break;
	
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
	if (getFrame()->getStack().size() < 1)
		throw RuntimeError(*this, "Data stack empty.");
	return getFrame()->pop();
}

Machine::Machine()
{
	reset();
}

Machine::~Machine()
{
}

Ref<ActivationFrame> Machine::getFrame()
{
	if (_callStack.size() < 1)
		return NULL;
	return _callStack.back();
}

void Machine::pushFrame(Ref<ActivationFrame> frame)
{
	_callStack.push_back(frame);
}

Ref<ActivationFrame> Machine::popFrame()
{
	if (_callStack.size() < 1)
		throw RuntimeError(*this, "Stack empty.");
	Ref<ActivationFrame> top = _callStack.back();
	_callStack.pop_back();
	return top;
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

void Machine::dumpStack(ostream &output, int limit) const
{
	CallStack::const_reverse_iterator it = _callStack.rbegin();
	int offset = _callStack.size() - limit - 1;
	
	output << "Call stack:" << endl;
	
	for (it = _callStack.rbegin(); it != _callStack.rend() && limit > 0; it++, limit--) {
		output << "[" << offset + limit << "] ";
		output << **it << endl;
	}
	
	if (limit < (int) _callStack.size())
		output << "(" << _callStack.size() - limit << " more frames)" << endl;
	
	output << endl;
}

