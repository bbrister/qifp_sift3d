#include <string.h>
#include <math.h>
#include "miniball.h"
#include "Miniball.hpp"

static void _miniball(double **points, const int num, 
        const double dim, double *const center, double *const rad);

/* Inputs:
 * points - A [num x dim] nested array.
 * num - The number of points.
 * dim - The dimensionality of each point.
 * center - A 3-wide array to output the center of the ball, or NULL.
 * rad - A scalar to output the radus of the ball, or NULL.
 * 
 * Returns 0 on success, nonzero on failure. */
int miniball(double **points, const int num, const double dim,
        double *const center, double *const rad) {

        // Isolate all exceptions
        try {
                _miniball(points, num, dim, center, rad);
        } catch (...) {
                return 1;
        }

        return 0;
}

/* See miniball(). */
static void _miniball(double **points, const int num, 
        const double dim, double *const center, double *const rad) {

        // Create the Miniball insteace
        typedef Miniball::
                Miniball <Miniball::
                        CoordAccessor<double* const*, const double*> > 
                MB;
        MB mb (dim, points, points + num);

        // Optionally return the results 
        if (center != NULL)
                memcpy(center, mb.center(), dim * sizeof(double)); //FIXME is this correct?

        if (rad != NULL)
                *rad = sqrt(mb.squared_radius());
}
