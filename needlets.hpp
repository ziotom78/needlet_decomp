#line 446 "needlet_decomp.nw"
#ifndef NEEDLETS_HPP
#define NEEDLETS_HPP

#include <stddef.h>  /* For size_t */

typedef struct __needlet_t needlet_t;

needlet_t * needlet_init(double b_param, unsigned int nside);
void needlet_free(needlet_t *);

void window_function (needlet_t * needlet,
                      int j,
                      size_t size,
                      const double * ang_scales,
                      double * result);

#endif
