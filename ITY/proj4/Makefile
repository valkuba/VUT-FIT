#
# Typografie a publikování
# Projekt č.4
# Autor: Jakub Valeš, xvales04
# Datum: 18.4.2022
#

PR=proj4

$(PR).pdf: $(PR).ps
	ps2pdf $(PR).ps

$(PR).ps: $(PR).dvi
	dvips -sPAPERSIZE=a4 $(PR).dvi

$(PR).dvi: $(PR).tex
	latex $(PR).tex
	bibtex $(PR)
	latex $(PR).tex
	latex $(PR).tex

clean:
	rm -f $(PR).dvi $(PR).ps $(PR).aux $(PR).log $(PR).out $(PR).bbl $(PR).blg
