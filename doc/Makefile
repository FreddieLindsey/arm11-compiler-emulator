EXTRA_SRC =
TEX = pdflatex

all: Report.pdf Checkpoint.pdf

.phony: all clean

Report.pdf: Report.tex title.tex introduction.tex emulate.tex assemble.tex \
gpio.tex extension.tex testing.tex groupreflections.tex \
adamhosier.tex pontusliljeblad.tex freddielindsey.tex dylantracey.tex \
conclusion.tex
	pandoc -o $@ $^

Checkpoint.pdf: Checkpoint.tex
	pandoc -o $@ $^

clean:
	rm -f *.aux
	rm -f *.log
	rm -f *.pdf
