#include <chrono>
#include <iostream>
#include <limits>
struct Result {
  int y0;
  int x0;
  int y1;
  int x1;
  float outer[3];
  float inner[3];
};

std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

void printTime() {
  end = std::chrono::steady_clock::now();
  std::cout << "Time difference = "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     begin)
                   .count()
            << "[µs]" << std::endl;
  begin = std::chrono::steady_clock::now();
}

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

// static inline int color(int y, int x, int c, int nx, int ny,
//                         const float* data) {
//   return data[c + 3 * x + 3 * nx * y];
// }
// static inline int error_inside(int y, int x, int c) {}
// static inline int error_inside(int y, int x, int c) {}

/*
This is the function you need to implement. Quick reference:
- x coordinates: 0 <= x < nx
- y coordinates: 0 <= y < ny
- color components: 0 <= c < 3
- input: data[c + 3 * x + 3 * nx * y]
*/
Result segment(int ny, int nx, const float *data) {
  Result result{0, 0, 0, 0, {0, 0, 0}, {0, 0, 0}};

  double *sums = new double[3 * nx * ny];
  // double* inner = new double(3 * nx * ny);

  for (int y = 0; y < ny; y++) {
    double accumulator[3] = {0, 0, 0};
    for (int x = 0; x < nx; x++) {
      for (int c = 0; c < 3; c++) {
        accumulator[c] += data[c + 3 * x + 3 * nx * y];
        sums[c + 3 * x + 3 * nx * y] =
            accumulator[c] + (y > 0 ? sums[c + 3 * x + 3 * nx * (y - 1)] : 0);
        // std::cout << "Sum: " << sums[c + 3 * x + 3 * nx * y] << std::endl;
      }
    }
  }
  printTime();

  double min_error = std::numeric_limits<double>::infinity();
#pragma omp parallel for
  for (int y0 = 0; y0 < ny; y0++) {
    for (int y1 = y0 + 1; y1 <= ny; y1++) {
      for (int x0 = 0; x0 < nx; x0++) {
        for (int x1 = x0 + 1; x1 <= nx; x1++) {
          double inner_avg[3] = {0.0, 0.0, 0.0};
          double outer_avg[3] = {0.0, 0.0, 0.0};
          for (int c = 0; c < 3; c++) {
            if (x0 == 0 && y0 == 0) {
              inner_avg[c] = sums[c + 3 * (x1 - 1) + 3 * nx * (y1 - 1)];
            } else if (y0 == 0) {
              inner_avg[c] = (sums[c + 3 * (x1 - 1) + 3 * nx * (y1 - 1)] -
                              sums[c + 3 * (x0 - 1) + 3 * nx * (y1 - 1)]);
            } else if (x0 == 0) {
              inner_avg[c] = (sums[c + 3 * (x1 - 1) + 3 * nx * (y1 - 1)] -
                              sums[c + 3 * (x1 - 1) + 3 * nx * (y0 - 1)]);
            } else {
              inner_avg[c] = (sums[c + 3 * (x0 - 1) + 3 * nx * (y0 - 1)] -
                              sums[c + 3 * (x1 - 1) + 3 * nx * (y0 - 1)] -
                              sums[c + 3 * (x0 - 1) + 3 * nx * (y1 - 1)] +
                              sums[c + 3 * (x1 - 1) + 3 * nx * (y1 - 1)]);
            }
            outer_avg[c] =
                (sums[c + 3 * (nx - 1) + 3 * nx * (ny - 1)] - inner_avg[c]) /
                (nx * ny - (x1 - x0) * (y1 - y0));
            inner_avg[c] /= (x1 - x0) * (y1 - y0);
            // std::cout << "Average " << c << ": " << inner_avg[c] <<
            // std::endl;
          }
          double inner_error[3] = {0.0, 0.0, 0.0};
          double outer_error[3] = {0.0, 0.0, 0.0};
          for (int x = 0; x < nx; x++) {
            for (int y = 0; y < ny; y++) {
              if ((x0 <= x) && (x < x1) && (y0 <= y) && (y < y1)) {
                for (int c = 0; c < 3; c++) {
                  double t = data[c + 3 * x + 3 * nx * y] - inner_avg[c];
                  inner_error[c] += t * t;
                }
              } else {
                for (int c = 0; c < 3; c++) {
                  double t = data[c + 3 * x + 3 * nx * y] - outer_avg[c];
                  outer_error[c] += t * t;
                }
              }
            }
          }
          double seg_error = inner_error[0] + inner_error[1] + inner_error[2] +
                             outer_error[0] + outer_error[1] + outer_error[2];
// std::cout << "Minimum error: " << min_error << std::endl;
// std::cout << "Segment error: " << seg_error << std::endl;
#pragma omp critical
          if (seg_error < min_error) {
            min_error = seg_error < min_error ? seg_error : min_error;
            result.x0 = x0;
            result.x1 = x1;
            result.y0 = y0;
            result.y1 = y1;
            for (int c = 0; c < 3; c++) {
              result.inner[c] = inner_avg[c];
              result.outer[c] = outer_avg[c];
            }
          }
        }
      }
    }
  }
  printTime();

  delete[] sums;

  return result;
}
