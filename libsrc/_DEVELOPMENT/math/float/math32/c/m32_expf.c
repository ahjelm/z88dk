/* Exponential function
 *
 * SYNOPSIS:
 *
 * float x, y, expf();
 *
 * y = expf( x );
 *
 *
 * DESCRIPTION:
 *
 * Returns e (2.71828...) raised to the x power.
 *
 * Range reduction is accomplished by separating the argument
 * into an integer k and fraction f such that
 *
 *     x    k  f
 *    e  = 2  e.
 *
 * A polynomial is used to approximate exp(f)
 * in the basic range [-0.5, 0.5].
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      +- MAXLOG   100000      1.7e-7      2.8e-8
 *
 *
 * Error amplification in the exponential function can be
 * a serious matter.  The error propagation involves
 * exp( X(1+delta) ) = exp(X) ( 1 + X*delta + ... ),
 * which shows that a 1 lsb error in representing X produces
 * a relative error of X times 1 lsb in the function.
 * While the routine gives an accurate result for arguments
 * that are exactly represented by a double precision
 * computer number, the result contains amplified roundoff
 * error for large arguments not exactly represented.
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * expf underflow    x < MINLOGF         0.0
 * expf overflow     x > MAXLOGF         MAXNUMF
 *
 */
 
/*
 Cephes Math Library Release 2.2:  June, 1992
 Copyright 1984, 1987, 1989 by Stephen L. Moshier
 Direct inquiries to 30 Frost Street, Cambridge, MA 02140
 */
 
/* Single precision exponential function.
 * test interval: [-0.5, +0.5]
 * trials: 80000
 * peak relative error: 7.6e-8
 * rms relative error: 2.8e-8
 */

#include "m32_math.h"

#define LOG2EF 1.44269504088896341
#define C1 0.693359375
#define C2 -2.12194440e-4

extern float m32_coeff_exp[];

float m32_expf(float x) __z88dk_fastcall
{
    int16_t n;

    n = (int16_t)m32_floorf( LOG2EF * x + 0.5 );

    x -= (float)n * C1;
    x -= (float)n * C2;

    /* Theoretical peak relative error in [-0.5, +0.5] is 4.2e-9. */
    return m32_ldexpf(((m32_sqrf(x) * m32_polyf(x, m32_coeff_exp, 5)) + x + 1.0), n);
}