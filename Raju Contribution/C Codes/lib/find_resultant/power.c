/*
 * power.c
 *
 * Code generation for function 'power'
 *
 * C source code generated on: Tue Apr 07 13:26:05 2015
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "find_resultant.h"
#include "power.h"

/* Function Definitions */
void b_power(const real_T a[238], real_T y[238])
{
  int32_T k;
  for (k = 0; k < 238; k++) {
    y[k] = sqrt(a[k]);
  }
}

void power(const real_T a[238], real_T y[238])
{
  int32_T k;
  for (k = 0; k < 238; k++) {
    y[k] = a[k] * a[k];
  }
}

/* End of code generation (power.c) */
