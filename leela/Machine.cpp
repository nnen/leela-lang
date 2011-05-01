/**
 * \file   Machine.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  Implementation of the {@link Machine} class.
 */

#include "Machine.h"
#include "Exception.h"

/* Program flow conteol *******************************************************/

void Machine::do_STOP() { _stop = true; }

void Machine::do_NOOP() { }

void Machine::do_RETURN()
{
	Ref<Value> value = pop();
	popFrame();
	push(value);
}

void Machine::do_CALL(UInteger count)
{
	vector<Ref<Value> > arguments;
	while (count-- > 0) arguments.push_back(pop());
	pushFrame(pop()->call(arguments));
}

/* Stack operations ***********************************************************/

void Machine::do_POP() { pop(); }

void Machine::do_DUP()
{
	Ref<Value> value = pop();
	push(value);
	push(value);
}

void Machine::do_PUSH(Integer value)
{
	push(new Number(value));
}

void Machine::do_PUSH_NONE()
{
	push(None::getInstance());
}

/* Binary operations **********************************************************/

void Machine::do_ADD()
{
	Ref<Value> second = pop();
	Ref<Value> first  = pop();
	push(first->add(second));
}

void Machine::do_SUB()
{
	Ref<Value> second = pop();
	Ref<Value> first  = pop();
	push(first->subtract(second));
}

void Machine::do_MUL()
{
	Ref<Value> second = pop();
	Ref<Value> first  = pop();
	push(first->multiply(second));
}

void Machine::do_DIV()
{
	Ref<Value> second = pop();
	Ref<Value> first  = pop();
	push(first->divide(second));
}

/* IO operations **************************************************************/

void Machine::do_PRINT()
{
	cout << *pop() << endl;
}

void Machine::do_DUMP()
{
	// TODO: Implement the DUMP instruction.
}

void Machine::do_DUMP_STACK(UInteger limit)
{
	dumpStack(cout, limit);
}

/* Variable operations ********************************************************/

void Machine::do_LOAD(UInteger variable)
{
	/* code */
}

void Machine::do_PULL(UInteger count)
{
	if (_callStack.size() < 2)
		throw RuntimeError("There's no activation frame to pull from.");
	
	for (UInteger i = 0; i < count; i++)
		getFrame()->pushVariable(_callStack[_callStack.size() - 2]->pop());
}

/* Function operations ********************************************************/

void Machine::do_MAKE_FUNCTION(Address address)
{
	Ref<Number> argCount = pop()->toNumber();
	push(new Function(address, argCount->getValue()));
}

void Machine::do_LOAD_CLOSURE(UInteger variable)
{
	/* code */
}

/* Table operations ***********************************************************/

void Machine::do_MAKE_TABLE()
{
	push(new Table());
}

/******************************************************************************/

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
	#define OC(name) case Instruction::name: do_##name(); break;
	#define OC1(name, argname, type) case Instruction::name: do_##name(instr.argname); break;
	default:
		throw RuntimeError();
		break;
	}
	
	/*
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
		// TODO: Figure out how to pass number of arguments to a function constructor.
		push(new Function(instr.payload.address, 0));
		break;
	case Instruction::LOAD_CLOSURE: {
		Ref<Function> function = pop().as<Function>();
		if (function.isNull())
			throw RuntimeError(*this, "Expected function on top of the stack.");
		function->pushClosure(getFrame()->getVar(instr.payload.uinteger));
		push(function);
		} break;
	case Instruction::PULL:
		do_PULL(instr);
		break;
	case Instruction::ALLOC: {
		for (int i = 0; i < (int) instr.payload.uinteger; i++)
			getFrame()->pushVariable();
		} break;
	
	default:
		throw RuntimeError(*this, "Unknown or unimplemented instruction code.");
		break;
	}
	*/
}

void Machine::push(Ref<Value> value)
{
	_callStack.back()->push(value);
}

Ref<Value> Machine::pop()
{
	if (getFrame()->getStack().size() < 1)
		throw RuntimeError("Data stack empty.");
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
		throw RuntimeError("Call stack empty.");
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
	} catch (RuntimeError &e) {
		e.print(cerr);
		cerr << endl;
		
		_stop = true;
	}
	
	return true;
}

void Machine::reset()
{
	_callStack.clear();
	
	Ref<Function> bootLoader = new Function(0, 0);
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

void Machine::dumpStack(ostream &output, int limit)
{
	CallStack::const_reverse_iterator it = _callStack.rbegin();
	int offset = _callStack.size() - limit - 1;
	
	output << "Call stack:" << endl;
	
	for (it = _callStack.rbegin(); it != _callStack.rend() && limit > 0; it++, limit--) {
		output << "[" << offset + limit << "] ";
		(*it)->print(output);
		output << endl;
		// output << **it << endl;
	}
	
	if (limit < (int) _callStack.size())
		output << "(" << _callStack.size() - limit << " more frames)" << endl;
	
	output << endl;
}

