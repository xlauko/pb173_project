SOURCES=${wildcard *.md}

all:	${SOURCES:.md=.pdf}

%.pdf:	%.tex
	rubber --pdf $<

%.tex: %.md
	pandoc -s -t beamer -o $@ $<

.PRECIOUS: