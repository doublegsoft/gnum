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
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>

#include <string>

#include "gnum.hpp"

using namespace std;

namespace gn
{

double
random(double mu, double sigma)
{

  double U1, U2, W, mult;
  static double X1, X2;
//  static int call = 0;
//
//  if (call == 1)
//  {
//    call = !call;
//    return (mu + sigma * (double) X2);
//  }

  do
  {
    U1 = -1 + ((double) rand() / RAND_MAX) * 2;
    U2 = -1 + ((double) rand() / RAND_MAX) * 2;
    W = pow (U1, 2) + pow (U2, 2);
  }
  while ( W >= 1 || W == 0 );

  mult = sqrt ((-2 * log (W)) / W);
  X1 = U1 * mult;
  X2 = U2 * mult;

//  call = !call;

  return (mu + sigma * (double) X1);
}

double
exponential(double val)
{
  return exp(val);
}

class mat::impl
{

public:

  double*         vals;

  uint            rows;

  uint            cols;

  bool            alloc;

};

mat::mat(uint rows, uint cols, double* vals): pimpl(new impl())
{
  if (rows <= 0)
    throw std::invalid_argument("the initial matrix row size is not greater than zero.");
  if (cols <= 0)
    throw std::invalid_argument("the initial matrix column size is not greater than zero.");

  pimpl->vals = vals;
  pimpl->rows = rows;
  pimpl->cols = cols;
  pimpl->alloc = false;
  if (pimpl->vals == NULL)
  {
    pimpl->vals = (double*)calloc(pimpl->rows * pimpl->cols, sizeof(double));
    pimpl->alloc = true;
  }
}

mat::~mat(void)
{
  if (pimpl->vals != NULL && pimpl->alloc)
  {
    delete[] pimpl->vals;
    pimpl->vals = NULL;
  }
  delete pimpl;
}

uint
mat::rows(void) const
{
  return pimpl->rows;
}

uint
mat::cols(void) const
{
  return pimpl->cols;
}

mat&
mat::set(uint row, uint col, double value)
{
  if (row >= pimpl->rows)
    throw std::invalid_argument("the row index is not less than the matrix row size.");
  if (col >= pimpl->cols)
    throw std::invalid_argument("the column index is not less than the matrix column size.");

  pimpl->vals[row * pimpl->cols + col] = value;
  return *this;
}

double
mat::get(uint row, uint col) const
{
  if (row >= pimpl->rows || col >= pimpl->cols)
    return DBL_MAX;
  return pimpl->vals[row * pimpl->cols + col];
}

mat&
mat::rand(double max)
{
  uint i, j;

  if (max != 0)
    srand(time(NULL));
  if (pimpl->rows > 0 && pimpl->cols > 0)
  {
    for (i = 0; i < pimpl->rows; i++)
      for (j = 0; j < pimpl->cols; j++)
        if (max != 0)
          pimpl->vals[i * pimpl->cols + j] = gn::random(0, max);
        else
          pimpl->vals[i * pimpl->cols + j] = 0;
  }
  return *this;
}

mat
mat::exp()
{
  mat ret(this->pimpl->rows, this->pimpl->cols);

  uint i, j;

  for (i = 0; i < pimpl->rows; i++)
    for (j = 0; j < pimpl->cols; j++)
      ret.set(i, j, gn::exponential(this->get(i, j)));

  return ret;
}

double
mat::sum()
{
  double sum = 0.0;

  uint i, j;

  for (i = 0; i < pimpl->rows; i++)
    for (j = 0; j < pimpl->cols; j++)
      sum += this->get(i, j);

  return sum;
}

void
mat::debug(FILE* stream)
{
  uint i, j;
  string res = "[";
  for (i = 0; i < pimpl->rows; i++)
  {
    if (i > 0)
      res += "\n ";
    res += "[";
    for (j = 0; j < pimpl->cols; j++)
    {
      if (j > 0)
        res += ",";
      res += to_string(pimpl->vals[i * pimpl->cols + j]);
    }
    res += "]";
    if (i != pimpl->rows - 1)
      res += ",";
  }
  res += "]\n";
  fprintf(stream, res.c_str());
}

mat
mat::transpose(void)
{
  uint i, j;
  mat ret(pimpl->cols, pimpl->rows);

  for (i = 0; i < ret.rows(); i++)
    for (j = 0; j < ret.cols(); j++)
      ret.set(i, j, this->get(j, i));
  return ret;
}

mat
mat::operator + (const mat& m)
{
  if (pimpl->rows != m.rows())
    throw std::invalid_argument("two matrice' row count is not equal.");
  if (pimpl->cols != m.cols())
    throw std::invalid_argument("two matrice' column count is not equal.");

  uint i, j;
  mat ret(m.rows(), m.cols());
  for (i = 0; i < pimpl->rows; i++)
    for (j = 0; j < pimpl->cols; j++)
      ret.set(i, j, pimpl->vals[i * pimpl->cols + j] + m.get(i, j));
  return ret;
}

mat
mat::operator + (const double scalar)
{
  uint i, j;
  mat ret(pimpl->rows, pimpl->cols);
  for (i = 0; i < pimpl->rows; i++)
    for (j = 0; j < pimpl->cols; j++)
      ret.set(i, j, pimpl->vals[i * pimpl->cols + j] + scalar);
  return ret;
}

mat
mat::operator - (const mat& m)
{
  if (pimpl->rows != m.rows())
    throw std::invalid_argument("two matrice' row count is not equal.");
  if (pimpl->cols != m.cols())
    throw std::invalid_argument("two matrice' column count is not equal.");

  uint i, j;
  mat ret(m.rows(), m.cols());
  for (i = 0; i < pimpl->rows; i++)
    for (j = 0; j < pimpl->cols; j++)
      ret.set(i, j, pimpl->vals[i * pimpl->cols + j] - m.get(i, j));
  return ret;
}

mat
mat::operator - (const double scalar)
{
  uint i, j;
  mat ret(pimpl->rows, pimpl->cols);
  for (i = 0; i < pimpl->rows; i++)
    for (j = 0; j < pimpl->cols; j++)
      ret.set(i, j, pimpl->vals[i * pimpl->cols + j] - scalar);
  return ret;
}

/*!
** @see https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm
*/
mat
mat::operator * (const mat& m)
{
  if (pimpl->cols != m.rows())
    throw std::invalid_argument("the first matrix column count is not equal to the second matrix row count.");

  uint i, j, k;
  mat ret(pimpl->rows, m.cols());
  for (i = 0; i < pimpl->rows; i++)
  {
    for (j = 0; j < m.cols(); j++)
    {
      double sum = 0.0;
      for (k = 0; k < pimpl->cols; k++)
      {
        double m1_val = this->get(i, k);
        double m2_val = m.get(k, j);
        sum += m1_val * m2_val;
      }
      ret.set(i, j, sum);
    }
  }
  return ret;
}

mat
mat::operator * (const double scalar)
{
  uint i, j;
  mat ret(pimpl->rows, pimpl->cols);
  for (i = 0; i < pimpl->rows; i++)
  {
    for (j = 0; j < pimpl->cols; j++)
    {
      ret.set(i, j, pimpl->vals[i * pimpl->cols + j] * scalar);
    }
  }
  return ret;
}

};

/*
********************************************************************************
********************************************************************************
**
** C FUNCTIONS
**
********************************************************************************
********************************************************************************
*/

int
gn_mat_mul(double* m1, uint r1, uint c1, double* m2, uint r2, uint c2, double** vals)
{

  gn::mat mat1(r1, c1, m1);
  gn::mat mat2(r2, c2, m2);

  gn::mat ret = mat1 * mat2;

  uint i, j;

  *vals = (double*)calloc(r1 * c2, sizeof(double));
  for (i = 0; i < r1; i++)
    for (j = 0; j < c2; j++)
      (*vals)[i * c2 + j] = ret.get(i, j);
  return 0;
}

int
gn_mat_add(double* m1, double* m2, uint rows, uint cols, double** vals)
{
  gn::mat mat1(rows, cols, m1);
  gn::mat mat2(rows, cols, m2);

  gn::mat ret = mat1 + mat2;

  uint i, j;

  *vals = (double*)calloc(rows * cols, sizeof(double));
  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++)
      (*vals)[i * cols + j] = ret.get(i, j);
  return 0;
}

int
gn_mat_T(double* mat, uint rows, uint cols, double** vals)
{
  gn::mat m(rows, cols, mat);
  m.transpose();

  uint i, j;
  *vals = (double*)calloc(rows * cols, sizeof(double));
  for (i = 0; i < cols; i++)
    for (j = 0; j < rows; j++)
      (*vals)[i * rows + j] = m.get(j, i);
}

int
gn_mat_rand(double* mat, uint rows, uint cols)
{
  uint i, j;
  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++)
      mat[i * cols + j] = gn::random(0, 1);
}

void
gn_mat_debug(FILE* stream, double* mat, uint rows, uint cols)
{
  gn::mat m(rows, cols, mat);
  m.debug(stream);
}
