Leela Programming Language
==========================

Leela is a toy Pascal-like dynamic programming language with anonymous
functions and lambda expressions.

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

