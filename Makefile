HEALPIX_PATH=/usr/local/Healpix/src/cxx/osx/include/
NOWEAVE = noweave
NOTANGLE = notangle
NODEFS = nodefs
NOINDEX = noindex
CPIF = cpif
TEXI2PDF = texi2pdf

SOURCE_DIR = src
DOC_DIR = docs

DEBUG = yes   # Comment this to prevent debugging information to be saved

SOURCE_NW = main.nw needlets.nw
SOURCE_IMAGES = 
TEX_FILES := $(addprefix $(DOC_DIR)/, $(SOURCE_NW:%.nw=%.tex))
CPP_FILES := $(SOURCE_NW:%.nw=%.cpp)
OBJ_FILES := $(CPP_FILES:%.cpp=%.o)
HPP_FILES := needlets.hpp
IMAGES := $(addprefix $(DOC_DIR)/, $(SOURCE_IMAGES))
DEFS_FILES := $(SOURCE_NW:%.nw=%.defs)
BIB_FILE = needlet_decomp.bib
INDEX_FILE = all.defs
TEXI2PDF_FLAGS = --tidy --batch

CXX_PROGRAMS=test-needlet

CXXFLAGS_BASE=-I $(HEALPIX_PATH) -Wall -fopenmp $(shell pkg-config --cflags gsl)
LDFLAGS=-fopenmp
LOADLIBES=-lstdc++ -L/usr/local/Healpix/src/cxx/osx/lib/ -lhealpix_cxx -lcxxsupport -lcfitsio -lfftpack -lc_utils -lpsht $(shell pkg-config --libs gsl)

ifdef DEBUG
	CXXFLAGS=$(CXXFLAGS_BASE) -g
else
	CXXFLAGS=$(CXXFLAGS_BASE) -O3
endif

vpath %.nw $(SOURCE_DIR)

.PHONY: all test

all: needlet_decomp needlet_decomp.pdf $(CXX_PROGRAMS)

needlet_decomp: $(OBJ_FILES)
	$(CC) -o $@ $(LDFLAGS) $(OBJ_FILES) $(LOADLIBES) $(LDLIBS)

test-needlet: test-needlet.o needlets.o

needlet_decomp.pdf: $(TEX_FILES) $(BIB_FILE) $(IMAGES)
	$(TEXI2PDF) $(TEXI2PDF_FLAGS) --pdf -o $@ -I . -I docs $<
	$(NOINDEX) $<
	$(TEXI2PDF) $(TEXI2PDF_FLAGS) --pdf -o $@ -I . -I docs $<

needlet_decomp.tex: $(SOURCE_NW) $(BIB_FILE)
	$(NOWEAVE) -n -delay -index $< | $(CPIF) $@

$(CPP_FILES): $(SOURCE_NW)
	$(NOTANGLE) -L -R$@ $^ | $(CPIF) $@

$(HPP_FILES): $(SOURCE_NW)
	$(NOTANGLE) -R$@ $^ | $(CPIF) $@

$(DOC_DIR)/%.pdf: $(DOC_DIR)/%.asy
	asy -f pdf -o $@ $<

$(INDEX_FILE): $(DEFS_FILES)
	sort -u $? | cpif $@

$(TEX_FILES): | $(DOC_DIR)

$(DOC_DIR):
	mkdir $(DOC_DIR)

$(DOC_DIR)/%.tex: %.nw $(INDEX_FILE)
	$(NOWEAVE) -n -delay -indexfrom $(INDEX_FILE) $< | $(CPIF) $@

%.defs: %.nw
	$(NODEFS) $< | cpif $@
