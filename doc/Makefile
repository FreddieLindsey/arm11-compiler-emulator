EXTRA_SRC =
TEX = pdflatex

all: Report.pdf Checkpoint.pdf

.phony: all clean

Report.pdf: Report.tex
	pandoc -o $@ $^

Checkpoint.pdf: Checkpoint.tex
	pandoc -o $@ $^

clean:
	rm -f *.aux
	rm -f *.log
	rm -f *.pdf
