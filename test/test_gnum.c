#include <stdlib.h>
#include <stdio.h>
#include "gnum.h"

int main()
{
  uint i, j;
  double* m1 = (double*)malloc(6 * sizeof(double));
  double* m2 = (double*)malloc(6 * sizeof(double));
  double* res = NULL;

  gn_mat_rand(m1, 3, 2);
  gn_mat_rand(m2, 2, 3);

  gn_mat_mul(m1, 3, 2, m2, 2, 3, &res);
  gn_mat_debug(stdout, res, 3, 3);
  free(res);

  gn_mat_add(m1, m2, 3, 2, &res);
  gn_mat_debug(stdout, res, 3, 2);
  free(res);


  gn_mat_debug(stdout, m1, 3, 2);
  gn_mat_T(m1, 3, 2, &res);
  gn_mat_debug(stdout, res, 2, 3);
  free(res);


  if (m1 != NULL)
    free(m1);
  if (m2 != NULL)
    free(m2);
  return 0;
}
