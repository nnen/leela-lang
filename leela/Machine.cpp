/**
 * \file   Machine.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  Implementation of the {@link Machine} class.
 */

#include <iomanip>

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
	/*
	vector<Ref<Value> > arguments;
	while (count-- > 0) arguments.push_back(pop());
	pushFrame(pop()->call(arguments));
	*/

	// dumpStack(cout);

	vector<Ref<Value> > arguments(count);
	for (int i = count - 1; i >= 0; i--)
		arguments[i] = pop();
	
	Ref<Callable> callable = pop().as<Callable>();
	
	if (callable.isNull())
		throw RuntimeError("Cannot call noncallable value.");
	
	callable->call(*this, arguments);
}

void Machine::do_JUMP(Address address)
{
	getFrame()->instrCounter = address;
}

void Machine::do_JUMP_IF(Address address)
{
	Ref<Boolean> condition = pop()->toBoolean();
	if (condition->getValue())
		getFrame()->instrCounter = address;
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

void Machine::do_PUSH_STRING(Address address)
{
	push(_code->getString(address));
}

void Machine::do_PUSH_NONE()
{
	push(None::getInstance());
}

/* Unary operations ***********************************************************/

void Machine::do_NOT()
{
	Ref<Boolean> value = pop()->toBoolean();
	push(new Boolean(!value->getValue()));
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

void Machine::do_EQUALS()
{
	Ref<Value> b = pop();
	Ref<Value> a = pop();
	
	push(new Boolean(Value::equals(a, b)));
}

void Machine::do_LESS()
{
	Ref<Value> b = pop();
	Ref<Value> a = pop();

	push(new Boolean(a->lessThan(b)));
}

void Machine::do_GREATER()
{
	Ref<Value> b = pop();
	Ref<Value> a = pop();
	
	push(new Boolean(b->lessThan(a)));
}

void Machine::do_LESS_OR_EQ()
{
	Ref<Value> b = pop();
	Ref<Value> a = pop();
	
	push(new Boolean(a->lessThan(b) || a->equals(b)));
}

void Machine::do_GREATER_OR_EQ()
{
	Ref<Value> b = pop();
	Ref<Value> a = pop();
	
	push(new Boolean(b->lessThan(a) || b->equals(a)));
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

void Machine::do_DUMP_CONSTS()
{
	dumpConstants(cout);
}

/* Variable operations ********************************************************/

void Machine::do_LOAD_CONST(UInteger constant)
{
	if (!hasConstant(constant)) {
		RuntimeError e;
		e.getStream() << "Undefined constant " << constant << "!";
		throw e;
	}
	push(getConstant(constant));
}

void Machine::do_DEF_CONST()
{
	_constants.push_back(pop());
}

void Machine::do_LOAD(UInteger variable)
{
	if (_callStack.empty())
		throw RuntimeError("Call stack is empty.");
	
	push(_callStack.back()->getVar(variable)->getValue());
}

void Machine::do_STORE(UInteger variable)
{
	if (_callStack.empty())
		throw RuntimeError("Call stack is empty.");

	Ref<Value> value = pop();
	_callStack.back()->getVar(variable)->setValue(value);
}

void Machine::do_PULL(UInteger count)
{
	if (_callStack.size() < 2)
		throw RuntimeError("There's no activation frame to pull from.");
	
	for (UInteger i = 0; i < count; i++)
		getFrame()->pushVariable(_callStack[_callStack.size() - 2]->pop());
}

void Machine::do_ALLOC(UInteger count)
{
	for (UInteger i = 0; i < count; i++)
		getFrame()->pushVariable();
}

/* Function operations ********************************************************/

void Machine::do_MAKE_FUNCTION(Address address)
{
	Ref<Number> argCount = pop()->toNumber();
	push(new Function(address, argCount->getValue()));
}

void Machine::do_LOAD_CLOSURE(UInteger variable)
{
	Ref<Function> function = (Ref<Function>) getFrame()->peek();
	
	if (function.isNull())
		throw RuntimeError("Expected a function on stack.");
	
	function->pushClosure(getFrame()->getVar(variable));
}

/* Table operations ***********************************************************/

void Machine::do_MAKE_TABLE()
{
	push(new Table());
}

void Machine::do_SET_ITEM()
{
	Ref<Value> value = pop();
	Ref<Value> key = pop();
	Ref<Table> table = pop().as<Table>();
	
	if (table.isNull())
		throw RuntimeError("Value could not be converted to Table.");
	
	table->set(key, value);
}

void Machine::do_GET_ITEM()
{
	Ref<Value> key = pop();
	Ref<Table> table = pop().as<Table>();

	if (table.isNull())
		throw RuntimeError("Value could not be converted to Table.");

	Ref<Value> value = table->get(key, NULL);

	if (value.isNull())
		throw RuntimeError("Invalid key error.");

	push(value);
}

/******************************************************************************/

void Machine::initBuiltins()
{
	_constants = Constants(BUILTIN_COUNT);
	
	#define BUILTIN( name, capname, expression ) \
		setConstant(BUILTIN_##capname, expression);
	#include "builtins.h"
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
	#define OC(name) case Instruction::name: do_##name(); break;
	#define OC1(name, argname, type) case Instruction::name: do_##name(instr.payload.argname); break;
	#include "opcodes.h"
	default:
		throw RuntimeError("Unknown instruction.");
		break;
	}
}

void Machine::push(Ref<Value> value)
{
	if (getFrame()->getStackSize() >= DATA_STACK_LIMIT)
		throw RuntimeError("Data stack overflow.");
	getFrame()->push(value);
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
	if (_callStack.size() >= CALL_STACK_LIMIT)
		throw RuntimeError("Maximal recursion depth (call stack overflow) reached.");
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

bool Machine::hasConstant(UInteger index)
{
	return (index < _constants.size());
}

void Machine::setConstant(UInteger index, Ref<Value> value)
{
	_constants[index] = value;
}

Ref<Value> Machine::getConstant(UInteger index)
{
	if (index >= _constants.size())
		return None::getInstance();
	return _constants[index];
}

bool Machine::step()
{
	if (_stop) return false;
	if (_callStack.size() < 1) return false;
	
	try {
		Instruction instr = loadInstr();
		execute(instr);
	} catch (RuntimeError e) {
		cerr << "ERROR: ";
		e.print(cerr);
		cerr << endl;
		dumpStack(cerr, 10);
		
		_stop = true;
	}
	
	return true;
}

void Machine::reset()
{
	_callStack.clear();
	_constants.clear();

	initBuiltins();
	
	Ref<Function> bootLoader = new Function(0, 0);
	bootLoader->call(*this, vector<Ref<Value> >());
	// _callStack.push_back(bootLoader->activate());
	
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
		output << "[Frame " << setw(3) << right << offset + limit << "]: ";
		(*it)->print(output);
		output << endl;
		// output << **it << endl;
	}
	
	if (limit < (int) _callStack.size())
		output << "(" << _callStack.size() - limit << " more frames)" << endl;
	
	output << endl;
}

void Machine::dumpConstants(ostream &output)
{
	UInteger index = 0;
	for (index = 0; index < _constants.size(); index++) {
		output << "[" << index << "]  ";
		_constants[index]->repr(output);
		output << endl;
	}
}

bool Machine::getBuiltinIndex(string ident, UInteger& index)
{
	#define BUILTIN( name, capname, expression ) \
		if (ident == #name) { \
			index = (UInteger) BUILTIN_##capname; \
			return true; \
		}
	#include "builtins.h"
	return false;
}

