#include <cmath>
#include <iostream>
#include <vector>

double average(const float *data, const int length);
double std_dev(const double *data, const int length);
double squared_sum(const double *data, const int length);

/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- correlation between rows i and row j has to be stored in result[i + j*ny]
- only parts with 0 <= j <= i < ny need to be filled
*/
void correlate(int ny, int nx, const float *data, float *result) {
  std::vector<double> data_norm(nx * ny);
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

  for (int i = 0; i < ny; i++) {
    for (int j = 0; j <= i; j++) {
      double res = 0.0;
      for (int k = 0; k < nx; ++k) {
        double x = data_norm[k + i * nx];
        double y = data_norm[k + j * nx];

        res += x * y;
      }
      result[i + j * ny] = res;
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