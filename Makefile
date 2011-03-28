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

CXX_PROGRAMS=needlet_decomp test_needlet
CPPFLAGS = -I $(HEALPIX_PATH) $(shell pkg-config --cflags gsl)
CXXFLAGS_BASE = -fopenmp -Wall
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

all: needlet_decomp.pdf $(CXX_PROGRAMS)

needlet_decomp: main.o needlets.o
	$(CC) -o $@ $(LDFLAGS) $(OBJ_FILES) $(LOADLIBES) $(LDLIBS)

test_needlet: test_needlet.o needlets.o

needlet_decomp.pdf: $(TEX_FILES) $(IMAGES) $(BIB_FILE)
	$(TEXI2PDF) $(TEXI2PDF_FLAGS) --pdf -o $@ $<

needlet_decomp.tex: $(SOURCE_NW)
	$(NOWEAVE) -n -delay -index $< | $(CPIF) $@

main.cpp needlets.hpp needlets.cpp: $(SOURCE_NW)
	$(NOTANGLE) -L -R$@ $^ | $(CPIF) $@

%.pdf: %.asy
	asy -f pdf -o $@ $<

include $(subst .cpp,.d,$(CPP_FILES))

main.o main.d: needlets.hpp

test_needlets.o test_needlets.d: needlets.hpp

%.d: %.cpp
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
