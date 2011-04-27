" include.vim

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

function! OpenThree(first, second, third)
	execute "tabnew " . a:first
	normal n
	normal L
	execute "edit " . a:second
	normal n
	normal L
	execute "edit " . a:third
	normal =
endfunction

function! OpenCpp(name)
	call OpenTwo(a:name . ".h", a:name . ".cpp")
endfunction

function! ResizeForDesktop()
	set columns=239
	set lines=53
endfunction

