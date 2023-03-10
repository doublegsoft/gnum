/*
**
**    █████████  ██████   █████ █████  █████ ██████   ██████
**   ███░░░░░███░░██████ ░░███ ░░███  ░░███ ░░██████ ██████
**  ███     ░░░  ░███░███ ░███  ░███   ░███  ░███░█████░███
** ░███          ░███░░███░███  ░███   ░███  ░███░░███ ░███
** ░███    █████ ░███ ░░██████  ░███   ░███  ░███ ░░░  ░███
** ░░███  ░░███  ░███  ░░█████  ░███   ░███  ░███      ░███
**  ░░█████████  █████  ░░█████ ░░████████   █████     █████
**   ░░░░░░░░░  ░░░░░    ░░░░░   ░░░░░░░░   ░░░░░     ░░░░░
**
*/

#ifndef __GNUM_H__
#define __GNUM_H__

#ifndef uint
typedef unsigned int      uint;
#endif

#define GNUM_SUCCESS                                          0
#define GNUM_ERROR_MATRICES_COLUMN_NOT_EQUALS_TO_ROW          200001
#define GNUM_ERROR_MATRICES_ROW_NOT_EQUALS                    200002
#define GNUM_ERROR_MATRICES_COLUMN_NOT_EQUALS                 200003

int
gn_mat_mul(float* m1, uint r1, uint c1, float* m2, uint r2, uint c2, float** vals);

int
gn_mat_add(float* m1, float* m2, uint rows, uint cols, float** vals);

int
gn_mat_exp(float* mat, uint rows, uint cols, float** vals);

int
gn_mat_tanh(float* mat, uint rows, uint cols, float** vals);

float
gn_mat_sum(float* mat, uint rows, uint cols);

int
gn_mat_T(float* mat, uint rows, uint cols, float** vals);

int
gn_mat_rand(float* mat, uint rows, uint cols);

void
gn_mat_debug(FILE* stream, float* mat, uint rows, uint cols);

#endif
