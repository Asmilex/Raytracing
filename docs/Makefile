#
# Author: Jake Zimmerman <jake@zimmerman.io>
#
# ===== Usage ================================================================
#
# NOTE:
#   When running these commands at the command line, replace $(TARGET_PDF) with
#   the actual value of the TARGET_PDF variable.
#
#
# make                  Compile all *.md files to PDFs
# make <filename>.pdf   Compile <filename>.md to a PDF
# make <filename>.tex   Generate the intermediate LaTeX for <filename>.md
#
# make view             Compile $(TARGET_PDF).md to a PDF, then view it
# make again            Force everything to recompile
#
# make clean            Get rid of all intermediate generated files
# make veryclean        Get rid of ALL generated files:
#
# make print            Send $(TARGET_PDF).pdf to the default printer:
#
# ============================================================================


TARGET_PDF = TFG
TARGET_WEB = index

SOURCES = ./headers/meta.md ./headers/math.md
SOURCES += $(shell find . -wholename './chapters/*.md' | sort | awk -F'\0' '{print $2}')

SOURCES_WEB = ./headers/web.md
SOURCES_PDF = ./headers/pdf.md

PANDOC_FLAGS =\
	--from markdown \
	--listings \
	--top-level-division="chapter" \
	--filter pandoc-eqnos \
	--filter pandoc-fignos \
	--citeproc \
	--csl ./headers/chicago-alt.csl \
	--bibliography ./chapters/bibliography.bib

PANDOC_FLAGS_PDF = --template ./headers/eisvogel.tex
PANDOC_FLAGS_WEB =\
	--standalone \
	--toc \
	--toc-depth 4 \
	--number-sections \
	--mathjax="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-svg-full.js" \
	--lua-filter ./headers/standard-code.lua \

LATEX_FLAGS = \

PANDOC_EXEC = pandoc    # Posible uso de pandoc.exe si ejecutas a través de WSL.

PDF_ENGINE = xelatex
PANDOCVERSIONGTEQ2 := $(shell expr `$(PANDOC_EXEC) --version | grep ^pandoc | sed 's/^.* //g' | cut -f1 -d.` \>= 2)

ifeq "$(PANDOCVERSIONGTEQ2)" "1"
    LATEX_FLAGS += --pdf-engine=$(PDF_ENGINE)
else
    LATEX_FLAGS += --latex-engine=$(PDF_ENGINE)
endif

all: pdf web

pdf: $(SOURCES) $(SOURCES_PDF)
	$(info Compilando $(TARGET_PDF).pdf)
	$(PANDOC_EXEC) $(PANDOC_FLAGS) $(PANDOC_FLAGS_PDF) $(LATEX_FLAGS) -o $(TARGET_PDF).pdf $(SOURCES) $(SOURCES_PDF)
	$(info Terminado.)

web: $(SOURCES) $(SOURCES_WEB)
	$(info Compilando $(TARGET_WEB).html...)
	$(PANDOC_EXEC) $(PANDOC_FLAGS) $(PANDOC_FLAGS_WEB) $(LATEX_FLAGS) -o $(TARGET_WEB).html $(SOURCES) $(SOURCES_WEB)
	$(info Terminado.)

clean:
	rm -f *.aux *.log *.nav *.out *.snm *.toc *.vrb tags || true

veryclean: clean
	rm -f $(TARGET_PDF).pdf $(TARGET_WEB).html

view: $(TARGET_PDF).pdf
	if [ "Darwin" = "$(shell uname)" ]; then open $(TARGET_PDF).pdf ; else xdg-open $(TARGET_PDF).pdf ; fi

.PHONY: all clean veryclean view
