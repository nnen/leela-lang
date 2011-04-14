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

call OpenCpp("Object")
call OpenTwo("Object.h", "Object.cpp")
call OpenTwo("Parser.h", "Parser.cpp")
call OpenTwo("grammar.h", "grammar.cpp")

tabnew Session.vim

normal gt
quit

