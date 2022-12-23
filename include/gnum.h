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
gn_mat_mul(double* m1, uint r1, uint c1, double* m2, uint r2, uint c2, double** vals);

int
gn_mat_add(double* m1, double* m2, uint rows, uint cols, double** vals);

int
gn_mat_exp(double* mat, uint rows, uint cols, double** vals);

int
gn_mat_tanh(double* mat, uint rows, uint cols, double** vals);

double
gn_mat_sum(double* mat, uint rows, uint cols);

int
gn_mat_T(double* mat, uint rows, uint cols, double** vals);

int
gn_mat_rand(double* mat, uint rows, uint cols);

void
gn_mat_debug(FILE* stream, double* mat, uint rows, uint cols);

#endif
