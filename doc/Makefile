EXTRA_SRC =
TEX = pdflatex

all: Report.pdf Checkpoint.pdf

.phony: all clean

%.pdf: %.tex $(EXTRA_SRC)
	while( $(TEX) $< ; grep -q "Rerun to get cross" $(<:.tex=.log)) ; \
	  do true ; \
	 done

clean:
	rm -f *.fdb_latexmk
	rm -f *.fls
	rm -f *.aux
	rm -f *.fdb_latexmk
	rm -f *.fls
	rm -f *.log
	rm -f *.pdf
