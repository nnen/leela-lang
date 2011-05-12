report.pdf: report.tex
	pdflatex -draftmode -interaction errorstop report.tex
	pdflatex -draftmode -interaction batchmode report.tex
	pdflatex -interaction batchmode report.tex

rebuild:
	make clean
	make

clean:
	rm -f report.pdf report.aux report.out report.toc

