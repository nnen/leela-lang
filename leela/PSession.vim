" PSession.vim
" Parser session

source include.vim

edit leela.h
call OpenTwo("opcodes.h", "mnemonics.h")
call OpenCpp("Object")
call OpenCpp("Token")
call OpenCpp("Lexer")
call OpenCpp("Parser")
call OpenTwo("token_types.h", "grammar.h")
call OpenCpp("Block")

" asm.cpp
" AsmScanner.cpp
" Assembler.cpp
" Block.cpp
" Bytecode.cpp
" Char.cpp
" Function.cpp
" grammar.cpp
" Input.cpp
" Instruction.cpp
" leela.cpp
" Lexer.cpp
" Machine.cpp
" Object.cpp
" Parser.cpp
" Token.cpp
" Value.cpp

" AsmScanner.h
" Assembler.h
" Block.h
" Bytecode.h
" Char.h
" Function.h
" grammar.h
" Input.h
" Instruction.h
" Interpreter.h
" leela.h
" Lexer.h
" Machine.h
" mnemonics.h
" Object.h
" opcodes.h
" Parser.h
" Token.h
" token_types.h
" Value.h

