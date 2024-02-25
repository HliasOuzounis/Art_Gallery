.PHONY: clean

all:
	mkdir -p build
	pdflatex -shell-escape report.tex
	bibtex report
	pdflatex -shell-escape report.tex
	pdflatex -shell-escape report.tex
	mv report.aux report.log report.bbl report.blg report.out build