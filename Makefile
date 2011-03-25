HEALPIX_PATH=/usr/local/Healpix/src/cxx/osx/include/
NOWEAVE = noweave
NOTANGLE = notangle
CPIF = cpif
TEXI2PDF = texi2pdf

DEBUG = yes   # Comment this to prevent debugging information to be saved

SOURCE_NW = needlet_decomp.nw
SOURCE_IMAGES = 
TEX_FILES := $(SOURCE_NW:%.nw=%.tex)
CPP_FILES := main.cpp needlets.cpp
OBJ_FILES := $(CPP_FILES:%.cpp=%.o)
HPP_FILES := needlets.hpp
IMAGES := $(SOURCE_IMAGES)
BIB_FILE = needlet_decomp.bib
TEXI2PDF_FLAGS = --tidy --batch

CXX_PROGRAMS=test_needlet

CXXFLAGS_BASE = -fopenmp $(shell pkg-config --cflags gsl) \
	-I $(HEALPIX_PATH) -Wall
LDFLAGS = -fopenmp
LOADLIBES = -lstdc++ $(shell pkg-config --libs gsl) \
	-L/usr/local/Healpix/src/cxx/osx/lib/ -lhealpix_cxx -lcxxsupport \
	-lcfitsio -lfftpack -lc_utils -lpsht

ifdef DEBUG
	CXXFLAGS=$(CXXFLAGS_BASE) -g
else
	CXXFLAGS=$(CXXFLAGS_BASE) -O3
endif

.PHONY: all test

all: needlet_decomp needlet_decomp.pdf $(CXX_PROGRAMS)

needlet_decomp: $(OBJ_FILES)
	$(CC) -o $@ $(LDFLAGS) $(OBJ_FILES) $(LOADLIBES) $(LDLIBS)

test_needlet: test_needlet.o needlets.o

needlet_decomp.pdf: $(TEX_FILES) $(BIB_FILE) $(IMAGES)
	$(TEXI2PDF) $(TEXI2PDF_FLAGS) --pdf -o $@ $<

needlet_decomp.tex: $(SOURCE_NW) $(BIB_FILE)
	$(NOWEAVE) -n -delay -index $< | $(CPIF) $@

$(CPP_FILES): $(SOURCE_NW)
	$(NOTANGLE) -L -R$@ $^ | $(CPIF) $@

$(HPP_FILES): $(SOURCE_NW)
	$(NOTANGLE) -R$@ $^ | $(CPIF) $@

%.pdf: %.asy
	asy -f pdf -o $@ $<

test_needlet.o: $(HPP_FILES)

main.o: $(HPP_FILES)
