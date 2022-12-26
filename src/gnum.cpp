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

#ifdef __APPLE__
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#endif

#include "gnum.hpp"

using namespace std;

namespace gn
{

#ifdef __APPLE__

static bool metal_inited = false;

static MTL::Device*       device;
static MTL::Function*     fun_add;

void
metal_init()
{
  if (metal_inited) return;

  NS::Error* error;
  NS::AutoreleasePool* p_pool = NS::AutoreleasePool::alloc()->init();
  device = MTL::CreateSystemDefaultDevice();

  NS::String* libpath = NS::String::string("matop.metallib", NS::UTF8StringEncoding);
  auto lib = device->newLibrary(libpath, &error);
  auto fn = NS::String::string("add_arrays", NS::ASCIIStringEncoding);
  fun_add = lib->newFunction(fn);
  metal_inited = true;
}

void
metal_mat_add(float* v1, float* v2, uint size, float* res)
{
  uint i;
  NS::Error* error;
  MTL::Buffer* m1 = device->newBuffer(v1, size * sizeof(float), MTL::ResourceStorageModeShared);
  MTL::Buffer* m2 = device->newBuffer(v2, size * sizeof(float), MTL::ResourceStorageModeShared);
  MTL::Buffer* rs = device->newBuffer(res, size * sizeof(float), MTL::ResourceStorageModeShared);

  MTL::ComputePipelineState*    pipeline_state  = device->newComputePipelineState(fun_add, &error);
  MTL::CommandQueue*            command_queue   = device->newCommandQueue();
  MTL::CommandBuffer*           command_buffer  = command_queue->commandBuffer();
  MTL::ComputeCommandEncoder*   compute_encoder = command_buffer->computeCommandEncoder();


  compute_encoder->setComputePipelineState(pipeline_state);

  compute_encoder->setBuffer(m1, 0, 0);
  compute_encoder->setBuffer(m2, 0, 1);
  compute_encoder->setBuffer(rs, 0, 2);

  MTL::Size grid_size = MTL::Size(size, 1, 1);

  NS::UInteger thread_count = pipeline_state->maxTotalThreadsPerThreadgroup();
  if (thread_count > size)
  {
    thread_count = size;
  }
  MTL::Size thread_group_size = MTL::Size(thread_count, 1, 1);
  compute_encoder->dispatchThreads(grid_size, thread_group_size);
  compute_encoder->endEncoding();

  command_buffer->commit();
  command_buffer->waitUntilCompleted();

//  float* r = (float*) rs->contents();
//  for (i = 0; i < size; i++)
//    printf("%f + %f = %f\n", v1[i], v2[i], r[i]);
}

#endif

float
random(float mu, float sigma)
{

  float U1, U2, W, mult;
  static float X1/*, X2*/;
//  static int call = 0;
//
//  if (call == 1)
//  {
//    call = !call;
//    return (mu + sigma * (float) X2);
//  }

  do
  {
    U1 = -1 + ((float) rand() / RAND_MAX) * 2;
    U2 = -1 + ((float) rand() / RAND_MAX) * 2;
    W = pow (U1, 2) + pow (U2, 2);
  }
  while ( W >= 1 || W == 0 );

  mult = sqrt ((-2 * log (W)) / W);
  X1 = U1 * mult;
//  X2 = U2 * mult;

//  call = !call;

  return (mu + sigma * (float) X1);
}

float
exponential(float val)
{
  return exp(val);
}

float
hyperbolic_tangent(float val)
{
  return tanh(val);
}

class mat::impl
{

public:

  float*         vals;

  uint            rows;

  uint            cols;

  bool            alloc;

};

mat::mat(uint rows, uint cols, float* vals): pimpl(new impl())
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
    pimpl->vals = (float*)calloc(pimpl->rows * pimpl->cols, sizeof(float));
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
mat::set(uint row, uint col, float value)
{
  if (row >= pimpl->rows)
    throw std::invalid_argument("the row index is not less than the matrix row size.");
  if (col >= pimpl->cols)
    throw std::invalid_argument("the column index is not less than the matrix column size.");

  pimpl->vals[row * pimpl->cols + col] = value;
  return *this;
}

float
mat::get(uint row, uint col) const
{
  if (row >= pimpl->rows || col >= pimpl->cols)
    return DBL_MAX;
  return pimpl->vals[row * pimpl->cols + col];
}

float*
mat::values() const
{
  return pimpl->vals;
}

mat&
mat::rand(float max)
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

mat
mat::tanh()
{
  mat ret(this->pimpl->rows, this->pimpl->cols);

  uint i, j;

  for (i = 0; i < pimpl->rows; i++)
    for (j = 0; j < pimpl->cols; j++)
      ret.set(i, j, gn::hyperbolic_tangent(this->get(i, j)));

  return ret;
}

float
mat::sum()
{
  float sum = 0.0;

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
mat::operator + (const float scalar)
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
mat::operator - (const float scalar)
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
      float sum = 0.0;
      for (k = 0; k < pimpl->cols; k++)
      {
        float m1_val = this->get(i, k);
        float m2_val = m.get(k, j);
        sum += m1_val * m2_val;
      }
      ret.set(i, j, sum);
    }
  }
  return ret;
}

mat
mat::operator * (const float scalar)
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

/*!
** matrix multiplication, A · B.
**
** @param m1
**        the first matrix
**
** @param r1
**        the first matrix row count
**
** @param c1
**        the first matrix column count
**
** @param m2
**        the second matrix
**
** @param r2
**        the second matrix row count
**
** @param c2
**        the second matrix column count
**
*/
int
gn_mat_mul(float* m1, uint r1, uint c1, float* m2, uint r2, uint c2, float** vals)
{

  gn::mat mat1(r1, c1, m1);
  gn::mat mat2(r2, c2, m2);

  uint i, j;
  try
  {
    gn::mat ret = mat1 * mat2;
    *vals = (float*)calloc(r1 * c2, sizeof(float));
    for (i = 0; i < r1; i++)
      for (j = 0; j < c2; j++)
        (*vals)[i * c2 + j] = ret.get(i, j);
  }
  catch (const std::invalid_argument& e)
  {
     return GNUM_ERROR_MATRICES_COLUMN_NOT_EQUALS_TO_ROW;
  }

  return GNUM_SUCCESS;
}

int
gn_mat_add(float* m1, float* m2, uint rows, uint cols, float** vals)
{
  gn::mat mat1(rows, cols, m1);
  gn::mat mat2(rows, cols, m2);

  gn::mat ret = mat1 + mat2;

  uint i, j;

  *vals = (float*)calloc(rows * cols, sizeof(float));
  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++)
      (*vals)[i * cols + j] = ret.get(i, j);
  return GNUM_SUCCESS;
}

int
gn_mat_T(float* mat, uint rows, uint cols, float** vals)
{
  gn::mat m(rows, cols, mat);
  m.transpose();

  uint i, j;
  *vals = (float*)calloc(rows * cols, sizeof(float));
  for (i = 0; i < cols; i++)
    for (j = 0; j < rows; j++)
      (*vals)[i * rows + j] = m.get(j, i);

  return GNUM_SUCCESS;
}

int
gn_mat_rand(float* mat, uint rows, uint cols)
{
  uint i, j;
  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++)
      mat[i * cols + j] = gn::random(0, 1);
  return GNUM_SUCCESS;
}

int
gn_mat_exp(float* mat, uint rows, uint cols, float** vals)
{
  gn::mat m(rows, cols, mat);
  gn::mat expm = m.exp();
  uint i, j;
  *vals = (float*)calloc(rows * cols, sizeof(float));
  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++)
      (*vals)[i * cols + j] = expm.get(i, j);
  return GNUM_SUCCESS;
}

int
gn_mat_tanh(float* mat, uint rows, uint cols, float** vals)
{
  gn::mat m(rows, cols, mat);
  gn::mat tanhm = m.tanh();
  uint i, j;
  *vals = (float*)calloc(rows * cols, sizeof(float));
  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++)
      (*vals)[i * cols + j] = tanhm.get(i, j);

  return GNUM_SUCCESS;
}

float
gn_mat_sum(float* mat, uint rows, uint cols)
{
  gn::mat m(rows, cols, mat);
  return m.sum();
}

void
gn_mat_debug(FILE* stream, float* mat, uint rows, uint cols)
{
  gn::mat m(rows, cols, mat);
  m.debug(stream);
}
