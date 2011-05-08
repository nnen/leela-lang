" ASMSession.vim
" Parser session

source include.vim

call CloseOthers()

edit leela-test.cpp
Edit2 leela.cpp leela.h
Edit2 opcodes.h mnemonics.h
Editcpp Object
Editcpp Token
Edit3 Lexer.h Lexer.cpp token_types.h
Edit3 Parser.h Parser.cpp leela_grammar.txt
Edit3 grammar.py Parser.rules.tpl.cpp leela_grammar.txt
Editcpp Context
Editcpp StringTable
Editcpp AsmWriter

tabnew
copen
tabfirst

