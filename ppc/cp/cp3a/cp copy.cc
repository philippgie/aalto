#include <x86intrin.h>

#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

std::chrono::steady_clock::time_point begin;
std::chrono::steady_clock::time_point end;
typedef double double4_t __attribute__((vector_size(4 * sizeof(double))));

constexpr double4_t d4zero = {0.0, 0.0, 0.0, 0.0};
// constexpr int P = 8;

static double4_t *double4_alloc(std::size_t n) {
  void *tmp = 0;
  if (posix_memalign(&tmp, sizeof(double4_t), sizeof(double4_t) * n)) {
    throw std::bad_alloc();
  }
  return (double4_t *)tmp;
}

inline double reduce_double4(double4_t d) {
  double aggr = 0.0;
  for (int i = 0; i < 4; i++) {
    aggr += d[i];
  }
  return aggr;
}

static inline double4_t swap4(double4_t x) {
  return _mm256_permute2f128_pd(x, x, 0b00000001);
}
static inline double4_t swap2(double4_t x) {
  return _mm256_permute2f128_pd(x, x, 0b00000001);
}
static inline double4_t swap1(double4_t x) {
  return _mm256_permute_pd(x, 0b0101);
}

// void printrow(double4_t *values, int nx) {
//   for (int i = 0; i < nx; i++) {
//     std::cout << values[i / 4][i % 4] << " ";
//   }
//   std::cout << std::endl;
//   std::cout << std::endl;
// }

double sum(const double4_t *data, const int length);
// double std_dev(const double4_t *data, const int length);
double squared_sum(const double4_t *data, const int length);

void print_vd(double4_t *vd, int dimA, int dimB) {
  for (int i = 0; i < dimA; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < dimB; k++) {
        std::cout << vd[i * dimB + k][j] << " ";
      }
      std::cout << std::endl;
    }
  }
}

/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- correlation between rows i and row j has to be stored in result[i + j*ny]
- only parts with 0 <= j <= i < ny need to be filled
*/
void correlate(int ny, int nx, const float *data, float *result) {
  constexpr int nb = 4;
  // vectors per input column
  int na = (ny + nb - 1) / nb;

  // input data, padded, converted to vectors
  double4_t *vd = double4_alloc(nx * na);
  // input data, transposed, padded, converted to vectors

  // std::chrono::steady_clock::time_point begin;
  // std::chrono::steady_clock::time_point end;

  int nc = nx % 4 ? nx / 4 + 1 : nx / 4;
  double4_t *data_norm = double4_alloc(nc * ny);

#pragma omp parallel for
  for (int y = 0; y < ny; y++) {
    double4_t *values = &data_norm[y * nc];
    values[nc - 1] = d4zero;

    for (int i = 0; i < nx; i++) {
      values[i / 4][i % 4] = data[y * nx + i];
    }

    double mean = sum(values, nc) / nx;
    // std::cout << "Mean of simple norm: " << mean << std::endl;
    for (int i = 0; i < nc; i++) {
      values[i] = data_norm[y * nc + i] - mean;
    }
    if (nx % 4)
      for (int i = nx % 4; i < 4; i++) {
        values[nc - 1][i] = 0.0;
      }

    double s = squared_sum(values, nc) / nx;
    // std::cout << "Standard deviation of simple norm: " << s << std::endl;
    for (int i = 0; i < nc; i++) {
      values[i] /= s;
    }

    double factor = std::sqrt(squared_sum(values, nc));
    // std::cout << "Factor of squared norm: " << factor << std::endl;
    for (int i = 0; i < nc; i++) {
      values[i] /= factor;
    }
  }

#pragma omp parallel for
  for (int ja = 0; ja < na; ++ja) {
    for (int i = 0; i < nx; ++i) {
      for (int jb = 0; jb < nb; ++jb) {
        int j = ja * nb + jb;
        vd[nx * ja + i][jb] = j < ny ? data_norm[nc * j + i / nb][i % nb] : 0.0;
      }
    }
  }
  // print_vd(vd, na, nx);

  // begin = std::chrono::steady_clock::now();
#pragma omp parallel for
  for (int ia = 0; ia < na; ++ia) {
    for (int ja = 0; ja < na; ++ja) {
      double4_t vv00 = d4zero;
      double4_t vv01 = d4zero;
      double4_t vv10 = d4zero;
      double4_t vv11 = d4zero;
      // double4_t vv100 = d4zero;
      // double4_t vv101 = d4zero;
      // double4_t vv110 = d4zero;
      // double4_t vv111 = d4zero;
      for (int k = 0; k < nx; ++k) {
        constexpr int PF = 20;
        __builtin_prefetch(&vd[nx * ia + k + PF]);
        __builtin_prefetch(&vd[nx * ja + k + PF]);
        double4_t a00 = vd[nx * ia + k];
        double4_t b00 = vd[nx * ja + k];
        // double4_t a100 = swap4(a000);
        double4_t a10 = swap2(a00);
        // double4_t a110 = swap2(a100);
        double4_t b01 = swap1(b00);
        vv00 += a00 * b00;
        vv01 += a00 * b01;
        vv10 += a10 * b00;
        vv11 += a10 * b01;
        //   vv100 += a100 * b000;
        //   vv101 += a100 * b001;
        //   vv110 += a110 * b000;
        //   vv111 += a110 * b001;
      }
      double4_t vv[4] = {vv00, vv01, vv10, vv11};
      for (int kb = 1; kb < 4; kb += 2) {
        vv[kb] = swap1(vv[kb]);
      }
      for (int jb = 0; jb < 4; ++jb) {
        for (int ib = 0; ib < 4; ++ib) {
          int i = ib + ia * 4;
          int j = jb + ja * 4;
          if (j < ny && i < ny && i <= j) {
            result[ny * i + j] = vv[ib ^ jb][jb];
          }
        }
      }
    }
  }

  // end = std::chrono::steady_clock::now();
  // std::cout << "Time difference = "
  //           << std::chrono::duration_cast<std::chrono::milliseconds>(end -
  //                                                                    begin)
  //                  .count()
  //           << "[ms]" << std::endl;

  std::free(data_norm);
  std::free(vd);
}

inline double sum(const double4_t *data, const int length) {
  double4_t aggr = d4zero;
  for (int x = 0; x < length; x++) {
    aggr += data[x];
  }
  return reduce_double4(aggr);
}

inline double squared_sum(const double4_t *data, const int length) {
  double4_t aggr = d4zero;
  for (int x = 0; x < length; x++) {
    aggr += data[x] * data[x];
  }
  return reduce_double4(aggr);
}
