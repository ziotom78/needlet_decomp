AM_CPPFLAGS = $(GSL_CFLAGS)
AM_LDFLAGS = $(GSL_LIBS) -lhealpix_cxx -lpsht -lcxxsupport -lc_utils -lfftpack
bin_PROGRAMS = needlet_decomp test_needlet

needlet_decomp_SOURCES = main.cpp needlets.hpp needlets.cpp
# needlet_decomp_HEADERS = needlets.hpp

test_needlet_SOURCES = test_needlet.cpp needlets.hpp needlets.cpp
# test_needlet_HEADERS = needlets.hpp
test_needlet_LDADD = $(DEPS_LIBS)

main.$(OBJEXT): needlets.hpp
needlets.$(OBJEXT): needlets.hpp

# The following code tells Make that needlet_decomp.nw
# must be used to generate main.cpp, needlets.hpp and
# needlets.cpp. It is taken from sect. 27.9 of the
# GNU AutoMake manual version 1.11.1

main.cpp: needlet_decomp.nw
	$(NOTANGLE) -L -R$@ $^ | $(CPIF) $@

 needlets.hpp needlets.cpp: main.cpp
	@if test -f $@; then \
		touch $@; \
	else \
		rm -f main.cpp; \
		$(MAKE) $(AM_MAKEFLAGS) main.cpp; \
	fi
