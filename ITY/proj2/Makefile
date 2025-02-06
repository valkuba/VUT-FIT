#
# Typografie a publikování
# Projekt č.2
# Autor: Jakub Valeš, xvales04
# Datum: 20.3.2022
#

PR=proj2

$(PR).pdf: $(PR).ps
	ps2pdf $(PR).ps

$(PR).ps: $(PR).dvi
	dvips -t a4 $(PR).dvi

$(PR).dvi: $(PR).tex
	latex $(PR).tex
	latex $(PR).tex

clean:
	rm -f $(PR).dvi $(PR).ps $(PR).aux $(PR).log $(PR).out
