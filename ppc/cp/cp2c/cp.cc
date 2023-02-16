#include <cmath>
#include <iostream>
#include <vector>

typedef double double4_t __attribute__((vector_size(4 * sizeof(double))));

constexpr double4_t d4zero = {0.0, 0.0, 0.0, 0.0};

static double4_t *double4_alloc(std::size_t n) {
  void *tmp = 0;
  if (posix_memalign(&tmp, sizeof(double4_t), sizeof(double4_t) * n)) {
    throw std::bad_alloc();
  }
  return (double4_t *)tmp;
}

double reduce_double4(double4_t d) {
  double aggr = 0.0;
  for (int i = 0; i < 4; i++) {
    aggr += d[i];
  }
  return aggr;
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

/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- correlation between rows i and row j has to be stored in result[i + j*ny]
- only parts with 0 <= j <= i < ny need to be filled
*/
void correlate(int ny, int nx, const float *data, float *result) {
  int onx = nx;
  nx = nx % 4 ? nx / 4 + 1 : nx / 4;
  double4_t *data_norm = double4_alloc(nx * ny);

  for (int y = 0; y < ny; y++) {
    double4_t *values = &data_norm[y * nx];
    values[nx - 1] = d4zero;

    for (int i = 0; i < onx; i++) {
      values[i / 4][i % 4] = data[y * onx + i];
    }

    double mean = sum(values, nx) / onx;
    // std::cout << "Mean of simple norm: " << mean << std::endl;
    for (int i = 0; i < nx; i++) {
      values[i] = data_norm[y * nx + i] - mean;
    }
    if (onx % 4)
      for (int i = onx % 4; i < 4; i++) {
        values[nx - 1][i] = 0.0;
      }

    double s = squared_sum(values, nx) / onx;
    // std::cout << "Standard deviation of simple norm: " << s << std::endl;
    for (int i = 0; i < nx; i++) {
      values[i] /= s;
    }

    double factor = std::sqrt(squared_sum(values, nx));
    // std::cout << "Factor of squared norm: " << factor << std::endl;
    for (int i = 0; i < nx; i++) {
      values[i] /= factor;
    }
  }

  for (int i = 0; i < ny; i++) {
    for (int j = 0; j <= i; j++) {
      double4_t res = d4zero;
      for (int k = 0; k < nx; ++k) {
        double4_t x = data_norm[k + i * nx];
        double4_t y = data_norm[k + j * nx];

        res += x * y;
      }
      result[i + j * ny] = reduce_double4(res);
    }
  }

  std::free(data_norm);
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
