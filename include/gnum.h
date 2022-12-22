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

int
gn_mat_mul(double* m1, uint r1, uint c1, double* m2, uint r2, uint c2, double** vals);

int
gn_mat_add(double* m1, double* m2, uint rows, uint cols, double** vals);

int
gn_mat_T(double* mat, uint rows, uint cols, double** vals);

int
gn_mat_rand(double* mat, uint rows, uint cols);

void
gn_mat_debug(FILE* stream, double* mat, uint rows, uint cols);

#endif
