#line 231 "needlet_decomp.nw"
#include <stdlib.h>   // For `atoi' and `atof'
#include <stdio.h>
#include <string>
#include <vector>
#line 240 "needlet_decomp.nw"
#include <gsl/gsl_math.h>
#include <fitsio.h>
#line 248 "needlet_decomp.nw"
#include <alm.h>
#include <alm_fitsio.h>
#include <alm_healpix_tools.h>
#include <datatypes.h>
#include <healpix_base.h>
#include <healpix_map.h>
#include <healpix_map_fitsio.h>
#line 262 "needlet_decomp.nw"
#include <config.h> // Generated by "configure"
#include "needlets.hpp"

#line 271 "needlet_decomp.nw"
std::string input_file_name;
double b_parameter;
int j_min;
int j_max;
const char * map_file_name_template = "needlet-map-%03d.fits";
const char * alm_file_name_template = "needlet-alm-%03d.fits";
int no_alm_flag = 0;  /* Whether to save a_lm maps */
#line 353 "needlet_decomp.nw"
void
print_help(const char * argv0)
{
    fprintf(stderr, "Usage: %s [OPTIONS] MAP_FILE B_PARAM MIN_J MAX_J\n\
\n\
Read a FITS file containing a Healpix map and decompose it in its\n\
needlet components from MIN_J to MAX_J. B_PARAM is a floating-point\n\
positive value specifying the shape of the needlet. Possible\n\
OPTIONS are:\n\
\n\
   -h, --help             Print this help and exit.\n\
   -v, --version          Print version information on stdout and exit.\n\
   --no-alm               Do not save a_lm coefficients in FITS files.\n\
   -o, --output FILE      Specify a template for needlet map file names.\n\
                          You can use *one* instance of `%%d' (or similar,\n\
                          as accepted by the Posix `printf' command) as a\n\
                          placeholder for the value of j, e.g. \n\
                          \"needlet-%%03d.fits\".\n\
   -a, --alm-output FILE  Specify a template for a_lm file names. The\n\
                          same said for `-o' appies here as well.\n\
\n\
Report bugs to <" PACKAGE_BUGREPORT ">\n", argv0);
}

void
print_version(void)
{
    fputs(PACKAGE_STRING " - compute needlet transforms of Healpix maps\n",
	  stderr);
}

#line 214 "needlet_decomp.nw"
int main(int argc, char ** argv)
{
    
#line 290 "needlet_decomp.nw"
int cur_arg = 1;
while(argv[cur_arg][0] == '-')
{
    if(   strcmp(argv[cur_arg], "-h") == 0
       || strcmp(argv[cur_arg], "--help") == 0)
    {
	print_help(argv[0]);
	return 0;
    } else if (   strcmp(argv[cur_arg], "-v") == 0
	       || strcmp(argv[cur_arg], "--version") == 0)
    {
	print_version();
	return 0;
    } else if (strcmp(argv[cur_arg], "--no-alm") == 0)
    {
	no_alm_flag = 1;
    } else if (   strcmp(argv[cur_arg], "-o") == 0
	       || strcmp(argv[cur_arg], "--output") == 0)
    {
	map_file_name_template = argv[++cur_arg];
    } else if (   strcmp(argv[cur_arg], "-a") == 0
	       || strcmp(argv[cur_arg], "--alm-output") == 0)
    {
	alm_file_name_template = argv[++cur_arg];
    } else {
	fprintf(stderr, PACKAGE_NAME ": unrecognized switch `%s'\n",
		argv[cur_arg]);
	return 1;
    }

    ++cur_arg;
}
#line 217 "needlet_decomp.nw"
    
#line 329 "needlet_decomp.nw"
if (argc - cur_arg != 4)
{
    print_help(argv[0]);
    return 1;
}

input_file_name = argv[cur_arg++];

if ((b_parameter = atof(argv[cur_arg++])) <= 0.0)
{
    fputs(PACKAGE_NAME ": the B parameter must be a positive number\n", stderr);
    return 1;
}

if (   (j_min = atoi(argv[cur_arg])) < 0
    || (j_max = atoi(argv[cur_arg + 1])) < 0)
{
    fputs(PACKAGE_NAME ": the number of components must be positive\n", stderr);
    return 1;
}
#line 218 "needlet_decomp.nw"
    
#line 400 "needlet_decomp.nw"
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
#line 433 "needlet_decomp.nw"
unsigned int l_max = 3 * input_map.Nside() - 1;
unsigned int m_max = l_max;

Alm<xcomplex<double> > input_alm(l_max, m_max);
arr<double> ring_weights(2 * input_map.Nside());
for (size_t i = 0; i < ring_weights.size(); ++i)
    ring_weights[i] = 1.0;

fputs("Decomposing the map into its spherical harmonics...\n", stderr);
map2alm_iter(input_map, input_alm, 5, ring_weights);
fputs("...done\n", stderr);
#line 219 "needlet_decomp.nw"
    
#line 459 "needlet_decomp.nw"
arr<double> ang_scales(l_max + 1);
for (unsigned int l = 0; l <= l_max; ++l)
    ang_scales[l] = l;

needlet_t * needlet = needlet_init(b_parameter, input_map.Nside());
#line 474 "needlet_decomp.nw"
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

    if (! no_alm_flag)
    {
	char * alm_file_name;
	asprintf(&alm_file_name, alm_file_name_template, j);
	write_Alm_to_fits(std::string("!") + alm_file_name,
			  output_alm, l_max, m_max, PLANCK_FLOAT64);
	fprintf(stderr, "File %s written to disk\n", alm_file_name);
    }

    Healpix_Map<double> output_map(input_map.Nside(), RING, SET_NSIDE);
    fputs("    converting the coefficients into a map\n", stderr);
    alm2map(output_alm, output_map);

    char * map_file_name;
    asprintf(&map_file_name, map_file_name_template, j);
    write_Healpix_map_to_fits(std::string("!") + map_file_name,
                              output_map, PLANCK_FLOAT64);
    fprintf(stderr, "File %s written to disk\n", map_file_name);
}
needlet_free(needlet);

#line 221 "needlet_decomp.nw"
    return 0;
}
