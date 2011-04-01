AM_CPPFLAGS = $(GSL_CFLAGS)
AM_LDFLAGS = $(GSL_LIBS) -lhealpix_cxx -lpsht -lcxxsupport -lc_utils -lfftpack

bin_PROGRAMS = needlet_decomp test_needlet

needlet_decomp_SOURCES = main.cpp needlets.hpp needlets.cpp
# needlet_decomp_HEADERS = needlets.hpp

test_needlet_SOURCES = test_needlet.cpp needlets.hpp needlets.cpp
# test_needlet_HEADERS = needlets.hpp
test_needlet_LDADD = $(DEPS_LIBS)

main.cpp needlets.hpp needlets.cpp: $(SOURCE_NW)
	$(NOTANGLE) -L -R$@ $^ | $(CPIF) $@
