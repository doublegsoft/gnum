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

#include <stdio.h>
#include <time.h>

#include "gnum.hpp"

static void
test_rand()
{
  printf("test rand:\n");
  gn::mat mat0(3, 2);
  mat0.rand(1);
  mat0.debug(stdout);
}

static void
test_mat_add()
{
  printf("test mat add:\n");
  gn::mat mat0(3, 2);
  gn::mat mat1(3, 2);

  mat0.rand(1);
  mat1.rand(1);
  gn::mat mat2 = mat0 + mat1;
  mat2.debug(stdout);
}

static void
test_mat_mul()
{
  printf("test mat mul:\n");
  gn::mat mat0(3, 2);
  gn::mat mat1(2, 3);

  mat0.set(0, 0, 0.584214);
  mat0.set(0, 1, -0.257189);
  mat0.set(1, 0, 0.662184);
  mat0.set(1, 0, 1.117756);
  mat0.set(2, 0, 0.616137);
  mat0.set(2, 1, -1.635551);

  mat1.set(0, 0, 0.292107);
  mat1.set(0, 1, -0.128595);
  mat1.set(0, 2, 0.331092);
  mat1.set(1, 0, 0.558878);
  mat1.set(1, 1, 0.308068);
  mat1.set(1, 2, -0.817776);

  gn::mat res = mat0 * mat1;
  res.debug(stdout);
}

static void
test_perf()
{
  printf("test perf:\n");
  int i = 0;
  double v1[] = {
    -0.094491, -0.443977,  0.313917,
    -0.490796, -0.229903,  0.065460,
     0.072921,  0.172246, -0.357751,
     0.104514, -0.463000,  0.079367,
    -0.226080, -0.154659, -0.038422,
     0.406115, -0.192794, -0.441992,
     0.181755,  0.088268,  0.277574,
    -0.055335,  0.491792,  0.263102
  };
  double v2[] = {
     0.023074,  0.479901,  0.432148,  0.375480, -0.364732, -0.119840,  0.266070, -0.351000,
    -0.368008,  0.424778, -0.257104, -0.148817,  0.033922,  0.353874, -0.144942,  0.130904,
     0.422434,  0.364503,  0.467865, -0.020302, -0.423890, -0.438777,  0.268529, -0.446787
  };
  for (i = 0; i < 1000000; i++)
  {
    gn::mat mat0(8, 3, v1);
    gn::mat mat1(3, 8, v2);

    gn::mat res = mat0 * mat1;
  }
}

int
main()
{

  test_rand();
  test_mat_add();
  test_mat_mul();

  struct timespec before, after;
  long elapsed_nsecs;

  clock_gettime(CLOCK_REALTIME, &before);

  test_perf();

  /* handle connection */
  clock_gettime(CLOCK_REALTIME, &after);

  elapsed_nsecs = (after.tv_sec - before.tv_sec) * 1000000000 +
                  (after.tv_nsec - before.tv_nsec);

  printf("  elapsed %0.2f ms!\n", (double) elapsed_nsecs / 1000000);
  return 0;
}
