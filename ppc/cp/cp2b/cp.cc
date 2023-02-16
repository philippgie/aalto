#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
using namespace std::chrono;

double average(const float *data, const int length);
double std_dev(const double *data, const int length);
double squared_sum(const double *data, const int length);

constexpr int P = 16;

/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- correlation between rows i and row j has to be stored in result[i + j*ny]
- only parts with 0 <= j <= i < ny need to be filled
*/
void correlate(int ny, int nx, const float *data, float *result) {
  // std::chrono::steady_clock::time_point begin, end;
  // std::unique_ptr<double[]> data_norm(new double[nx * ny]);

  std::vector<double> data_norm(nx * ny);
#pragma omp parallel for
  for (int y = 0; y < ny; y++) {
    double *values = &data_norm[y * nx];

    double mean = average(&data[y * nx], nx);
    // std::cout << "Mean of simple norm: " << mean << std::endl;

    for (int i = 0; i < nx; i++) {
      values[i] = data[y * nx + i] - mean;
    }

    double s = std_dev(values, nx);
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

#pragma omp parallel
  {
    double x[P];
    double y[P];
    double res[P] = {0.0};
#pragma omp for
    for (int i = 0; i < ny; i++) {
      for (int j = 0; j <= i; j++) {
        std::fill(res, res + P, 0.0);
        for (int k = 0; k < nx; k += P) {
          if (k + P < nx) {
            for (int l = 0; l < P; l++) {
              x[l] = data_norm[k + l + i * nx];
              y[l] = data_norm[k + l + j * nx];
              res[l] += x[l] * y[l];
            }
          } else {
            for (int l = 0; l + k < nx; l++) {
              x[l] = data_norm[k + l + i * nx];
              y[l] = data_norm[k + l + j * nx];
              res[l] += x[l] * y[l];
            }
          }
        }
        res[0] += res[1];
        res[2] += res[3];
        res[4] += res[5];
        res[6] += res[7];
        res[8] += res[9];
        res[10] += res[11];
        res[12] += res[13];
        res[14] += res[15];
        res[0] += res[2];
        res[4] += res[6];
        res[8] += res[10];
        res[12] += res[14];
        res[0] += res[4];
        res[8] += res[12];

        result[i + j * ny] = res[0] + res[8];
      }
    }
  }
}

inline double average(const float *data, const int length) {
  double aggr = 0.0;
  for (int x = 0; x < length; x++) {
    aggr += static_cast<double>(data[x]);
  }
  return aggr / (double)length;
}

inline double std_dev(const double *data, const int length) {
  double aggr = 0.0;
  for (int x = 0; x < length; x++) {
    aggr += data[x] * data[x];
  }
  return aggr / (double)length;
}

inline double squared_sum(const double *data, const int length) {
  double aggr = 0.0;
  for (int x = 0; x < length; x++) {
    aggr += data[x] * data[x];
  }
  return aggr;
}