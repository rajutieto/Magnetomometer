/*
 * find_resultant.c
 *
 * Code generation for function 'find_resultant'
 *
 * C source code generated on: Tue Apr 07 13:26:05 2015
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "find_resultant.h"
#include "power.h"

/* Function Definitions */
void find_resultant(const real_T test_obtData[1904], real_T resultant[238])
{
  real_T dv0[238];
  real_T S2_nm[238];
  real_T dv1[238];
  real_T dv2[238];
  int32_T i;
  real_T dv3[238];
  real_T dv4[238];
  real_T b_S2_nm[238];
  power(*(real_T (*)[238])&test_obtData[476], dv0);
  power(*(real_T (*)[238])&test_obtData[714], S2_nm);
  power(*(real_T (*)[238])&test_obtData[952], dv1);
  for (i = 0; i < 238; i++) {
    dv2[i] = (dv0[i] + S2_nm[i]) + dv1[i];
  }

  power(*(real_T (*)[238])&test_obtData[1190], dv0);
  power(*(real_T (*)[238])&test_obtData[1428], S2_nm);
  power(*(real_T (*)[238])&test_obtData[1666], dv1);
  for (i = 0; i < 238; i++) {
    dv3[i] = (dv0[i] + S2_nm[i]) + dv1[i];
  }

  b_power(dv3, S2_nm);
  b_power(dv2, dv4);
  power(dv4, dv0);
  memcpy(&b_S2_nm[0], &S2_nm[0], 238U * sizeof(real_T));
  power(b_S2_nm, S2_nm);
  for (i = 0; i < 238; i++) {
    dv1[i] = dv0[i] + S2_nm[i];
  }

  b_power(dv1, resultant);
}

/* End of code generation (find_resultant.c) */
