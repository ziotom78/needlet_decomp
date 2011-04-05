#line 229 "needlet_decomp.nw"
#include <stdlib.h>   // For `atoi' and `atof'
#include <stdio.h>
#include <string>
#include <vector>
#line 238 "needlet_decomp.nw"
#include <gsl/gsl_math.h>
#include <fitsio.h>
#line 246 "needlet_decomp.nw"
#include <alm.h>
#include <alm_fitsio.h>
#include <alm_healpix_tools.h>
#include <datatypes.h>
#include <healpix_base.h>
#include <healpix_map.h>
#include <healpix_map_fitsio.h>
#line 260 "needlet_decomp.nw"
#include <config.h> // Generated by "configure"
#include "needlets.hpp"

#line 269 "needlet_decomp.nw"
std::string input_file_name;
double b_parameter;
int j_min;
int j_max;

#line 213 "needlet_decomp.nw"
int main(int argc, char ** argv)
{
    
#line 279 "needlet_decomp.nw"
if (argc != 5)
{
    fputs("Usage: needlet_decomp MAP_FILE B_PARAM MIN_J MAX_J\n", stderr);
    return 1;
}

input_file_name = argv[1];

if ((b_parameter = atof(argv[2])) <= 0.0)
{
    fputs("The B parameter must be a positive number\n", stderr);
    return 1;
}

if (   (j_min = atoi(argv[3])) < 0
    || (j_max = atoi(argv[4])) < 0)
{
    fputs("The number of components must be positive\n", stderr);
    return 1;
}
#line 216 "needlet_decomp.nw"
    
#line 317 "needlet_decomp.nw"
Healpix_Map<double> input_map;
fprintf(stderr, "Reading map %s\n", input_file_name.c_str());
read_Healpix_map_from_fits(input_file_name, input_map);
if(input_map.Scheme() != RING)
{
    fputs("Converting map into RING scheme\n", stderr);
    input_map.swap_scheme();
}
fputs("Map read into memory\n", stderr);

unsigned long num_of_flagged_pixels = 0;
arr<double> map_pixels = input_map.Map();
for (unsigned long i = 0; i < map_pixels.size(); ++i)
{
    if (map_pixels[i] < -1.6e+30 || ! gsl_finite(map_pixels[i]))
    {
	map_pixels[i] = 0.0;
	num_of_flagged_pixels++;
    }
}
if (num_of_flagged_pixels > 0)
    fprintf(stderr, "%lu pixels were flagged and set to zero\n",
		 num_of_flagged_pixels);

input_map.Set(map_pixels, input_map.Scheme());
#line 350 "needlet_decomp.nw"
unsigned int l_max = 3 * input_map.Nside() - 1;
unsigned int m_max = l_max;

Alm<xcomplex<double> > input_alm(l_max, m_max);
arr<double> ring_weights(2 * input_map.Nside());
for (size_t i = 0; i < ring_weights.size(); ++i)
    ring_weights[i] = 1.0;

fputs("Decomposing the map into its spherical harmonics...\n", stderr);
map2alm_iter(input_map, input_alm, 5, ring_weights);
fputs("...done\n", stderr);
#line 217 "needlet_decomp.nw"
    
#line 376 "needlet_decomp.nw"
arr<double> ang_scales(l_max + 1);
for (unsigned int l = 0; l <= l_max; ++l)
    ang_scales[l] = l;

needlet_t * needlet = needlet_init(b_parameter, input_map.Nside());
#line 391 "needlet_decomp.nw"
for(int j = j_min; j <= j_max; ++j)
{
    fprintf(stderr, "Processing j = %d\n", j);

    arr<double> filter(l_max + 1);
    Alm<xcomplex<double> > output_alm;
    output_alm = input_alm;

    fputs("    estimating the window function\n", stderr);
    window_function(needlet, j, l_max + 1,
		    ang_scales.begin(), filter.begin());

    fputs("    filtering the alm coefficients\n", stderr);
    output_alm.ScaleL(filter);

    char * alm_file_name;
    asprintf(&alm_file_name, "needlet-alm-%02d.fits", j);
    write_Alm_to_fits(std::string("!") + alm_file_name,
                      output_alm, l_max, m_max, PLANCK_FLOAT64);
    fprintf(stderr, "File %s written to disk\n", alm_file_name);

    Healpix_Map<double> output_map(input_map.Nside(), RING, SET_NSIDE);
    fputs("    converting the coefficients into a map\n", stderr);
    alm2map(output_alm, output_map);

    char * map_file_name;
    asprintf(&map_file_name, "needlet-map-%02d.fits", j);
    write_Healpix_map_to_fits(std::string("!") + map_file_name,
                              output_map, PLANCK_FLOAT64);
    fprintf(stderr, "File %s written to disk\n", map_file_name);
}
needlet_free(needlet);

#line 219 "needlet_decomp.nw"
    return 0;
}