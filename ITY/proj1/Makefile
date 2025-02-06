#
# Typografie a publikování
# Projekt č.1
# Autor: Jakub Valeš, xvales04
# Datum: 6.3.2022
#

PR=proj1

$(PR).pdf: $(PR).ps
	ps2pdf $(PR).ps

$(PR).ps: $(PR).dvi
	dvips $(PR).dvi

$(PR).dvi: $(PR).tex
	latex $(PR).tex
	latex $(PR).tex

clean:
	rm -f $(PR).dvi $(PR).ps $(PR).aux $(PR).log $(PR).out
