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

/*!
** Gaussian generator:
**   https://phoxis.org/2013/05/04/generating-random-numbers-from-normal-distribution-in-c/
*/
double
random(double mu, double sigma);

class mat
{
public:

  mat(uint rows, uint cols, double* vals = NULL);

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
  mat& set(uint row, uint col, double val);

  double get(uint row, uint col) const;


  mat& rand(double max = 0);

  /*!
  **
  */
  void debug(FILE* stream);

  /*!
  **
  */
  mat transpose(void);

  mat operator + (const mat& mat);

  mat operator + (const double scalar);

  mat operator - (const mat& m);

  mat operator - (const double scalar);

  mat operator * (const mat& m);

  mat operator * (const double scalar);

private:

  class   impl;

  impl*   pimpl;

};

};

extern "C" {
  #include "gnum.h"
}

#endif // __GNUM_HPP__
