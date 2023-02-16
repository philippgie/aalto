#include <chrono>
#include <cmath>
#include <iostream>
using namespace std::chrono;

double average(const float *data, const int length);
double std_dev(const double *data, const int length);
double squared_sum(const double *data, const int length);

// constexpr int P = 8;

/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- correlation between rows i and row j has to be stored in result[i + j*ny]
- only parts with 0 <= j <= i < ny need to be filled
*/
void correlate(int ny, int nx, const float *data, float *result) {
  // block size
  constexpr int nd = 3;
  // how many blocks of rows
  int nc = (ny + nd - 1) / nd;
  // number of rows after padding
  int ncd = nc * nd;

  double *data_norm = new double[nx * ncd]{0.0};
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

  for (int i = ny; i < ncd; i++) {
    for (int j = 0; j < nx; j++) {
      data_norm[i * nx + j] = 0.0;
    }
  }

  double res[nd][nd];
  for (int ic = 0; ic < nc; ic++) {
    for (int jc = 0; jc < nc; jc++) {
      for (int id = 0; id < nd; ++id) {
        for (int jd = 0; jd < nd; ++jd) {
          res[id][jd] = 0.0;
        }
      }
      for (int k = 0; k < nx; k++) {
        double x0 = data_norm[k + (ic * nd) * nx];
        double x1 = data_norm[k + (ic * nd + 1) * nx];
        double x2 = data_norm[k + (ic * nd + 2) * nx];
        double y0 = data_norm[k + (jc * nd) * nx];
        double y1 = data_norm[k + (jc * nd + 1) * nx];
        double y2 = data_norm[k + (jc * nd + 2) * nx];

        res[0][0] += x0 * y0;
        res[0][1] += x0 * y1;
        res[0][2] += x0 * y2;
        res[1][0] += x1 * y0;
        res[1][1] += x1 * y1;
        res[1][2] += x1 * y2;
        res[2][0] += x2 * y0;
        res[2][1] += x2 * y1;
        res[2][2] += x2 * y2;
      }
      for (int id = 0; id < nd; ++id) {
        for (int jd = 0; jd < nd; ++jd) {
          int i = ic * nd + id;
          int j = jc * nd + jd;
          if (i < ny && j < ny) {
            result[ny * i + j] = res[id][jd];
          }
        }
      }
    }
  }
  delete[] data_norm;
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