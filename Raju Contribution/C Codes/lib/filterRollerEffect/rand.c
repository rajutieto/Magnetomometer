/*
 * rand.c
 *
 * Code generation for function 'rand'
 *
 * C source code generated on: Thu Apr 09 19:31:28 2015
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "filterRollerEffect.h"
#include "rand.h"
#include "filterRollerEffect_data.h"

/* Variable Definitions */
static uint32_T c_state[625];

/* Function Declarations */
static real_T eml_rand_mt19937ar(uint32_T d_state[625]);
static void twister_state_vector(uint32_T mt[625], real_T seed);

/* Function Definitions */
static real_T eml_rand_mt19937ar(uint32_T d_state[625])
{
  real_T r;
  int32_T exitg1;
  uint32_T u[2];
  int32_T i;
  uint32_T mti;
  int32_T kk;
  uint32_T y;
  uint32_T b_y;
  uint32_T c_y;
  uint32_T d_y;
  boolean_T isvalid;
  boolean_T exitg2;

  /* <LEGAL>   This is a uniform (0,1) pseudorandom number generator based on: */
  /* <LEGAL> */
  /* <LEGAL>    A C-program for MT19937, with initialization improved 2002/1/26. */
  /* <LEGAL>    Coded by Takuji Nishimura and Makoto Matsumoto. */
  /* <LEGAL> */
  /* <LEGAL>    Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura, */
  /* <LEGAL>    All rights reserved. */
  /* <LEGAL> */
  /* <LEGAL>    Redistribution and use in source and binary forms, with or without */
  /* <LEGAL>    modification, are permitted provided that the following conditions */
  /* <LEGAL>    are met: */
  /* <LEGAL> */
  /* <LEGAL>      1. Redistributions of source code must retain the above copyright */
  /* <LEGAL>         notice, this list of conditions and the following disclaimer. */
  /* <LEGAL> */
  /* <LEGAL>      2. Redistributions in binary form must reproduce the above copyright */
  /* <LEGAL>         notice, this list of conditions and the following disclaimer in the */
  /* <LEGAL>         documentation and/or other materials provided with the distribution. */
  /* <LEGAL> */
  /* <LEGAL>      3. The names of its contributors may not be used to endorse or promote */
  /* <LEGAL>         products derived from this software without specific prior written */
  /* <LEGAL>         permission. */
  /* <LEGAL> */
  /* <LEGAL>    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS */
  /* <LEGAL>    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT */
  /* <LEGAL>    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR */
  /* <LEGAL>    A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR */
  /* <LEGAL>    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, */
  /* <LEGAL>    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, */
  /* <LEGAL>    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR */
  /* <LEGAL>    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF */
  /* <LEGAL>    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING */
  /* <LEGAL>    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS */
  /* <LEGAL>    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */
  do {
    exitg1 = 0;
    for (i = 0; i < 2; i++) {
      u[i] = 0U;
    }

    for (i = 0; i < 2; i++) {
      mti = d_state[624] + 1U;
      if (mti >= 625U) {
        for (kk = 0; kk < 227; kk++) {
          y = (d_state[kk] & 2147483648U) | (d_state[1 + kk] & 2147483647U);
          if ((int32_T)(y & 1U) == 0) {
            b_y = y >> 1U;
          } else {
            b_y = y >> 1U ^ 2567483615U;
          }

          d_state[kk] = d_state[397 + kk] ^ b_y;
        }

        for (kk = 0; kk < 396; kk++) {
          y = (d_state[227 + kk] & 2147483648U) | (d_state[228 + kk] &
            2147483647U);
          if ((int32_T)(y & 1U) == 0) {
            c_y = y >> 1U;
          } else {
            c_y = y >> 1U ^ 2567483615U;
          }

          d_state[227 + kk] = d_state[kk] ^ c_y;
        }

        y = (d_state[623] & 2147483648U) | (d_state[0] & 2147483647U);
        if ((int32_T)(y & 1U) == 0) {
          d_y = y >> 1U;
        } else {
          d_y = y >> 1U ^ 2567483615U;
        }

        d_state[623] = d_state[396] ^ d_y;
        mti = 1U;
      }

      y = d_state[(int32_T)mti - 1];
      d_state[624] = mti;
      y ^= y >> 11U;
      y ^= y << 7U & 2636928640U;
      y ^= y << 15U & 4022730752U;
      y ^= y >> 18U;
      u[i] = y;
    }

    r = 1.1102230246251565E-16 * ((real_T)(u[0] >> 5U) * 6.7108864E+7 + (real_T)
                                  (u[1] >> 6U));
    if (r == 0.0) {
      if ((d_state[624] >= 1U) && (d_state[624] < 625U)) {
        isvalid = TRUE;
      } else {
        isvalid = FALSE;
      }

      if (isvalid) {
        isvalid = FALSE;
        i = 1;
        exitg2 = FALSE;
        while ((exitg2 == FALSE) && (i < 625)) {
          if (d_state[i - 1] == 0U) {
            i++;
          } else {
            isvalid = TRUE;
            exitg2 = TRUE;
          }
        }
      }

      if (!isvalid) {
        twister_state_vector(d_state, 5489.0);
      }
    } else {
      exitg1 = 1;
    }
  } while (exitg1 == 0);

  return r;
}

static void twister_state_vector(uint32_T mt[625], real_T seed)
{
  uint32_T r;
  int32_T mti;
  if (seed < 4.294967296E+9) {
    if (seed >= 0.0) {
      r = (uint32_T)seed;
    } else {
      r = 0U;
    }
  } else if (seed >= 4.294967296E+9) {
    r = MAX_uint32_T;
  } else {
    r = 0U;
  }

  mt[0] = r;
  for (mti = 0; mti < 623; mti++) {
    r = (r ^ r >> 30U) * 1812433253U + (1 + mti);
    mt[1 + mti] = r;
  }

  mt[624] = 624U;
}

void b_rand(real_T r[238])
{
  int32_T k;
  int32_T hi;
  uint32_T test1;
  uint32_T test2;
  if (method == 4U) {
    for (k = 0; k < 238; k++) {
      hi = (int32_T)(state / 127773U);
      test1 = 16807U * (state - hi * 127773U);
      test2 = 2836U * hi;
      if (test1 < test2) {
        state = (test1 - test2) + 2147483647U;
      } else {
        state = test1 - test2;
      }

      r[k] = (real_T)state * 4.6566128752457969E-10;
    }
  } else if (method == 5U) {
    for (k = 0; k < 238; k++) {
      test1 = 69069U * b_state[0] + 1234567U;
      test2 = b_state[1] ^ b_state[1] << 13U;
      test2 ^= test2 >> 17U;
      test2 ^= test2 << 5U;
      b_state[0] = test1;
      b_state[1] = test2;
      r[k] = (real_T)(test1 + test2) * 2.328306436538696E-10;
    }
  } else {
    if (!state_not_empty) {
      memset(&c_state[0], 0, 625U * sizeof(uint32_T));
      twister_state_vector(c_state, 5489.0);
      state_not_empty = TRUE;
    }

    for (k = 0; k < 238; k++) {
      r[k] = eml_rand_mt19937ar(c_state);
    }
  }
}

/* End of code generation (rand.c) */
