#include "needlets.hpp"
#include <cstdio>
#include <cstdlib>

int
main (int argc, const char ** argv)
{
    if (argc != 4)
    {
	std::fprintf(stderr, "Usage: %s B_VALUE J_MAX FILENAME_BASE\n", argv[0]);
	return EXIT_FAILURE;
    }

    const double b_value = std::atof(argv[1]);
    const int j_max = std::atoi(argv[2]);
    const int num_of_ells = 1000;
    double ells[num_of_ells];
    double win_func[num_of_ells];

    for (int ell = 0; ell < num_of_ells; ++ell)
    {
	ells[ell] = ell;
    }

    needlet_t * needlet = needlet_init(b_value, 0);
    for (int j = 1; j <= j_max; ++j)
    {
	window_function(needlet, j, num_of_ells, ells, win_func);
	
	char file_name[128];
	std::sprintf(file_name, "%s-%02d.txt", argv[3], j);
	FILE * f = std::fopen(file_name, "wt");
	for (int ell = 0; ell < num_of_ells; ++ell)
	{
	    std::fprintf(f, "%.0f\t%.5f\n", ells[ell], win_func[ell]);
	}
	std::fclose(f);
    }
    needlet_free(needlet);

    return 0;
}
