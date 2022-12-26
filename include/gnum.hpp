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

#ifndef __GNUM_HPP__
#define __GNUM_HPP__

#include <stdio.h>

#ifndef uint
typedef unsigned int      uint;
#endif

namespace gn
{

void
metal_init();

void
metal_mat_add(float* v1, float* v2, uint size, float* res);

/*!
** Gaussian generator:
**   https://phoxis.org/2013/05/04/generating-random-numbers-from-normal-distribution-in-c/
*/
float
random(float mu, float sigma);

class mat
{
public:

  mat(uint rows, uint cols, float* vals = NULL);

  ~mat(void);

  /*!
  **
  */
  uint rows(void) const;

  /*!
  **
  */
  uint cols(void) const;

  /*!
  **
  */
  mat& set(uint row, uint col, float val);

  float get(uint row, uint col) const;

  float* values() const;

  mat& rand(float max = 0);

  mat exp(void);

  mat tanh(void);

  float sum(void);

  /*!
  **
  */
  void debug(FILE* stream);

  /*!
  **
  */
  mat transpose(void);

  mat operator + (const mat& mat);

  mat operator + (const float scalar);

  mat operator - (const mat& m);

  mat operator - (const float scalar);

  mat operator * (const mat& m);

  mat operator * (const float scalar);

private:

  class   impl;

  impl*   pimpl;

};

};

extern "C" {
  #include "gnum.h"
}

#endif // __GNUM_HPP__
