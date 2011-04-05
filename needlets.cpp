#line 435 "needlet_decomp.nw"
#include "needlets.hpp"
#include <gsl/gsl_math.h>
#include <gsl/gsl_spline.h>

#line 555 "needlet_decomp.nw"
static const double needlet_l_points[] = {
   -1.01,-1.,-0.99,-0.98,-0.97,-0.96,-0.95,-0.94,-0.93,-0.92,-0.91,
   -0.9,-0.89,-0.88,-0.87,-0.86,-0.85,-0.84,-0.83,-0.82,-0.81,-0.8,-0.79,
   -0.78,-0.77,-0.76,-0.75,-0.74,-0.73,-0.72,-0.71,-0.7,-0.69,-0.68,
   -0.67,-0.66,-0.65,-0.64,-0.63,-0.62,-0.61,-0.6,-0.59,-0.58,-0.57,
   -0.56,-0.55,-0.54,-0.53,-0.52,-0.51,-0.5,-0.49,-0.48,-0.47,-0.46,
   -0.45,-0.44,-0.43,-0.42,-0.41,-0.4,-0.39,-0.38,-0.37,-0.36,-0.35,
   -0.34,-0.33,-0.32,-0.31,-0.3,-0.29,-0.28,-0.27,-0.26,-0.25,-0.24,
   -0.23,-0.22,-0.21,-0.2,-0.19,-0.18,-0.17,-0.16,-0.15,-0.14,-0.13,
   -0.12,-0.11,-0.1,-0.09,-0.08,-0.07,-0.06,-0.05,-0.04,-0.03,-0.02,
   -0.01,0.,0.01
};
static const double needlet_phi_points[] = {
0,0,6.502743904184239e-26,1.804735934292834e-14,
   1.631468853713497e-10,1.810113957105095e-8,3.339417624783784e-7,
   2.471150140069252e-6,0.00001075015854374423,0.00003336351373616104,
   0.00008236387786715602,0.0001727858298059248,0.0003214113575207205,
   0.0005455739393754458,0.0008621964816387603,0.001287113013132387,
   0.001834648460891867,0.002517402994959833,0.003346184787030743,
   0.004330042963612801,0.005476363319604365,0.006790999529434621,
   0.008278420938907964,0.009941864377262811,0.01178348204478926,
   0.01380448076780671,0.01600525011559473,0.01838547832968979,
   0.02094425594602818,0.02368016755829782,0.02659137249414124,
   0.02967567533735547,0.03293058728637641,0.03635337933135141,
   0.03994112818543715,0.04369075583815134,0.04759906352107971,
   0.05166276079624053,0.05587849039937629,0.06024284939693679,
   0.06475240714787172,0.06940372050010411,0.07419334659675309,
   0.07911785361856135,0.08417382974614265,0.08935789058809582,
   0.09466668528821007,0.1000969014963931,0.1056452693630967,
   0.1113085646954417,0.1170836113945422,0.1229672832773291,
   0.1289565053721521,0.1350482547653029,0.1412395610651023,
   0.1475275065411121,0.1539092259881733,0.1603819063581696,
   0.1669427861965335,0.1735891549154128,0.1803183519310082,
   0.1871277656887677,0.1940148325968177,0.200977035885137,
   0.2080119044054996,0.215117011385051,0.2222899731445174,
   0.2295284477904239,0.236830133889294,0.2441927691305803,
   0.2516141289840201,0.259092025356192,0.2666243052502535,
   0.2742088494321481,0.2818435711059751,0.2895264146006935,
   0.2972553540698853,0.3050283922059126,0.3128435589694673,
   0.320698910335222,0.3285925270540376,0.3365225134319684,
   0.3444869961261173,0.3524841229572322,0.3605120617387989,
   0.3685689991222625,0.376653139457912,0.3847627036708732,
   0.3928959281515799,0.4010510636600322,0.4092263742430952,
   0.4174201361640454,0.4256306368435351,0.4338561738111108,
   0.4420950536663959,0.4503455910490235,0.458606107616387,
   0.4668749310282625,0.4751503939373393,0.4834308329846915,
   0.4917145877992081,0.5,0.5082854122007919 };
static const size_t num_of_needlet_phi_points =
    sizeof(needlet_phi_points) / sizeof(needlet_phi_points[0]);
#line 620 "needlet_decomp.nw"
struct __needlet_t {
    double             b_param;
    unsigned int       nside;
    gsl_interp_accel * acc;
    gsl_spline       * spline;
};
#line 678 "needlet_decomp.nw"
static double
psi(needlet_t * needlet,
    double u)
{
    if (u <= -1.0)
        return 0;
    else if (u >= 1.0)
        return 1.0;
    else
    {
        double neg_u = -fabs(u);
        double spline = gsl_spline_eval(needlet->spline,
                                        neg_u,
					needlet->acc);
	return (u > 0.0) ? (1 - spline) : spline;
    }
}
#line 704 "needlet_decomp.nw"
static double
phi(needlet_t * needlet,
    double factor,
    double b_inv,
    double t)
{
    return psi(needlet, 1 - factor * (t - b_inv));
}
#line 637 "needlet_decomp.nw"
needlet_t *
needlet_init(double b_param, unsigned int nside)
{
    needlet_t * needlet;

    needlet = (needlet_t *) malloc(sizeof (needlet_t));
    needlet->b_param = b_param;
    needlet->nside   = nside;
    needlet->acc     = gsl_interp_accel_alloc();
    needlet->spline  = gsl_spline_alloc(gsl_interp_cspline,
                                        num_of_needlet_phi_points);

    gsl_spline_init(needlet->spline,
                    needlet_l_points,
 		    needlet_phi_points,
                    num_of_needlet_phi_points);
    return needlet;
}
#line 658 "needlet_decomp.nw"
void
needlet_free(needlet_t * needlet)
{
    if (needlet == NULL)
        return;

    gsl_spline_free(needlet->spline);
    gsl_interp_accel_free(needlet->acc);
    free(needlet);
}
#line 737 "needlet_decomp.nw"
void
window_function (needlet_t * needlet,
                 int j,
                 size_t size,
                 const double * ang_scales,
                 double * result)
{
    const double b_inv  = 1 / needlet->b_param;
    const double factor = 2 * needlet->b_param / (needlet->b_param - 1);
    const double b_inv_to_j = gsl_pow_int(b_inv, j);
    double pixel_weight;

    if(needlet->nside > 0) 
        pixel_weight = M_SQRTPI / (M_SQRT3 * needlet->nside);
    else
        pixel_weight = 1.0;

    for (size_t i = 0; i < size; ++i)
    {
        const double l_over_b_to_j = ang_scales[i] * b_inv_to_j;
	const double diff = phi(needlet, factor, b_inv, l_over_b_to_j * b_inv) -
                            phi(needlet, factor, b_inv, l_over_b_to_j);
        if (diff > 0.0)
            result[i] = sqrt(diff) * pixel_weight;
	else
            result[i] = 0.0;
    }
}
