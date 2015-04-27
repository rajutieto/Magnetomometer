/*
 * find_resultant.c
 *
 * Code generation for function 'find_resultant'
 *
 * C source code generated on: Thu Apr 23 21:56:12 2015
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "find_resultant.h"

/* Function Definitions */
void find_resultant(const real_T test_obtData[16], real_T resultant[2])
{
  int32_T k;
  real_T S1_nm;
  real_T S2_nm;

  /*  This function is to calcutate resultant of 6 sensor data. We have used */
  /*  square root of x1, y1 and z1 and similarly for x2, y2 and z2. and finally */
  /*  taking square root of two obtained result. */
  /*  INPUT test_obtData -> matrix containing all data */
  /*  OUTPUT resultant -> 1 column resultant vector */
  for (k = 0; k < 2; k++) {
    S1_nm = sqrt((test_obtData[4 + k] * test_obtData[4 + k] + test_obtData[6 + k]
                  * test_obtData[6 + k]) + test_obtData[8 + k] * test_obtData[8
                 + k]);
    S2_nm = sqrt((test_obtData[10 + k] * test_obtData[10 + k] + test_obtData[12
                  + k] * test_obtData[12 + k]) + test_obtData[14 + k] *
                 test_obtData[14 + k]);
    resultant[k] = sqrt(S1_nm * S1_nm + S2_nm * S2_nm);
  }
}

/* End of code generation (find_resultant.c) */
