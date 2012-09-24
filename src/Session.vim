" Session.vim

function! CloseOthers()
	let thisBuffer = bufnr('%')
	let all = range(1, bufnr('$'))
	let res = []
	
	for b in all
		if buflisted(b) && b != thisBuffer
			call add(res, b)
		endif
	endfor

	execute "bdelete " . join(res, " ")
endfunction

function! OpenTwo(first, second)
	execute "tabnew " . a:first
	normal n
	normal L
	execute "edit " . a:second
endfunction

function! OpenCpp(name)
	call OpenTwo(a:name . ".h", a:name . ".cpp")
endfunction

call CloseOthers()

edit leela.h
call OpenCpp("Object")
call OpenCpp("Parser")
call OpenCpp("grammar")
call OpenCpp("Instruction")
call OpenCpp("Assembler")
call OpenCpp("AsmScanner")
call OpenTwo("mnemonics.h", "opcodes.h")

tabnew Makefile

normal gt

