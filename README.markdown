Leela Programming Language
==========================

Leela is a toy Pascal-like dynamic programming language with anonymous
functions and lambda expressions.  Who in their right mind would base their toy
language's syntax on Pascal, you might ask?  Someone who had it assigned as a
homework, that's who.

The implementation consists of a virtual machine executing custom byte code and
two translators, one of which translates Leela to an asm-like intermediary
language and the other translates the intermediary language to binary byte
code.


Compilation
-----------

Use the Makefile in the `/src` directory.  Leela has no dependencies except for
STL and getopt and should compile on both 32 bit and 64 bit platforms.

To run leela, save Leela source code to a file (see code examples in `/tests`) and run

    /src/leela your_leela_file.leela

The output will be printed to stdout, the intermediary language code will be
saved to a file named `your_leela_file.lasm` and the bytecode will be saved to
`your_leela_file.leelac`.


Implementation
--------------

### Parser and Compiler

The Leela parser (`src/Parser.h`, `src/Parser.cpp`, `src/Parser.rules.h` and
`src/Parser.rules.cpp`) is an attributed almost-LL(1) recursive descent parser,
which directly translates Leela source code to an assembly language-like
intermediary language, LASM.

LASM mnemonics (defined in `src/mnemonics.h` map almost one-to-one to Leela
bytecode instructions (declared in `src/opcodes.h` and implemented in
`src/Machine.cpp`).  There's few exceptions (like the `STORE` mnemonic), where
a single mnemonic can map to several different instructions based on the number
of type of the arguments.  LASM also uses string literals and symbolic
addresses, which allows for somewhat simpler Leela parser.

Finally, the LASM is parsed and translated to bytecode (in-memory
representation is defined in `src/Bytecode.h` and `src/Bytecode.cpp`) by the
assembler (`src/Assembler.h` and `src/Assembler.cpp`).


### Virtual Machine

The virtual machine is implemented by the `Machine` class (`src/Machine.h` and
`src/Machine.cpp`).  The virtual machine reads bytecode instructions and
executes the one by one. 

Call stack and data stacks are separate, with each activation frame having its
own private data stack.  Variables are implemented as heap-allocated objects
that keep reference to a value object.  Each activation frame keeps a vector of
it's variables, including arguments.  This means that each variable acts as a
box (much like value boxing in .NET or Java) around a value and as long as at
least one context (activation frame) keeps reference to this box, it is not
destroyed.  This is so that closures could be easily implemented.


