#
# Typografie a publikování
# Projekt č.5
# Autor: Jakub Valeš, xvales04
# Datum: 8.5.2022
#

PROJ=proj5

pdflatex: $(PROJ).tex
	pdflatex $^
	pdflatex $^

clean:
	rm -f $(PROJ).nav $(PROJ).toc $(PROJ).aux $(PROJ).log $(PROJ).out $(PROJ).snm $(PROJ).vrb

cleanall:
	clean rm -f $(PROJ).pdf
