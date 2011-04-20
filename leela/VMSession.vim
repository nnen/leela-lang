
source include.vim

edit leela.h
call OpenTwo("opcodes.h", "mnemonics.h")
call OpenCpp("Object")
call OpenCpp("Value")
call OpenCpp("Function")
call OpenCpp("Instruction")
call OpenCpp("Assembler")
call OpenCpp("Machine")

