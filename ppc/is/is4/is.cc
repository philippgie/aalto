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
            << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                     begin)
                   .count()
            << "[µs]" << std::endl;
  begin = std::chrono::steady_clock::now();
}

typedef double double4_t __attribute__((vector_size(4 * sizeof(double))));

constexpr double infty = std::numeric_limits<double>::infinity();

constexpr double4_t d4inf = {infty, infty, infty, infty};
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

  double4_t *sums = double4_alloc(nx * ny);
  double4_t *data_4d = double4_alloc(nx * ny);
  // double* inner = new double(3 * nx * ny);
  // #pragma omp parallel for
  for (int y = 0; y < ny; y++) {
    for (int x = 0; x < nx; x++) {
      for (int c = 0; c < 3; c++) {
        data_4d[x + nx * y][c] = data[c + 3 * x + 3 * nx * y];
      }
      data_4d[x + nx * y][3] = 0.0;
    }
  }

  for (int y = 0; y < ny; y++) {
    double4_t accumulator = d4zero;
    for (int x = 0; x < nx; x++) {
      accumulator += data_4d[x + nx * y];
      sums[x + nx * y] =
          accumulator + (y > 0 ? sums[x + nx * (y - 1)] : d4zero);
      // std::cout << "Sum: " << sums[c + 3 * x + 3 * nx * y] << std::endl;
    }
  }
  printTime();

  // double max_error = std::numeric_limits<double>::infinity();
  double max_error = 0.0;
#pragma omp parallel
  {
    // double max_thread_error = std::numeric_limits<double>::infinity();
    double max_thread_error = 0.0;
    Result thread_result{0, 0, 0, 0, {0, 0, 0}, {0, 0, 0}};
#pragma omp for
    for (int y0 = 0; y0 < ny; y0++) {
      for (int y1 = y0 + 1; y1 <= ny; y1++) {
        for (int x0 = 0; x0 < nx; x0++) {
          for (int x1 = x0 + 1; x1 <= nx; x1++) {
            double4_t inner_error;
            double4_t outer_error;
            double4_t inner_avg;
            double4_t outer_avg;
            if (x0 == 0 && y0 == 0) {
              inner_error = sums[(x1 - 1) + nx * (y1 - 1)];
            } else if (y0 == 0) {
              inner_error = (sums[(x1 - 1) + nx * (y1 - 1)] -
                             sums[(x0 - 1) + nx * (y1 - 1)]);
            } else if (x0 == 0) {
              inner_error = (sums[(x1 - 1) + nx * (y1 - 1)] -
                             sums[(x1 - 1) + nx * (y0 - 1)]);
            } else {
              inner_error = (sums[(x0 - 1) + nx * (y0 - 1)] -
                             sums[(x1 - 1) + nx * (y0 - 1)] -
                             sums[(x0 - 1) + nx * (y1 - 1)] +
                             sums[(x1 - 1) + nx * (y1 - 1)]);
            }
            outer_error = (sums[(nx - 1) + nx * (ny - 1)] - inner_error);
            // for (int c = 0; c < 3; c++) {
            //   std::cout << "inner error: " << inner_error[c] << std::endl;
            //   std::cout << "outer error: " << outer_error[c] << std::endl;
            // }
            inner_avg = inner_error / ((x1 - x0) * (y1 - y0));
            outer_avg = outer_error / (nx * ny - (x1 - x0) * (y1 - y0));
            // for (int c = 0; c < 3; c++) {
            //   std::cout << "inner avg: " << inner_avg[c] << std::endl;
            //   std::cout << "outer avg: " << outer_avg[c] << std::endl;
            // }
            inner_error *= inner_avg;
            outer_error *= outer_avg;

            double seg_error = reduce_double4(inner_error + outer_error);

            // std::cout << "Minimum error: " << max_error << std::endl;
            // std::cout << "x1: " << x1 << "\t x0: " << x0 << std::endl;
            // std::cout << "y1: " << y1 << "\t y0: " << y0 << std::endl;
            // std::cout << "Segment error: " << seg_error << std::endl
            //          << std::endl;
            if (seg_error > max_thread_error) {
              max_thread_error = seg_error;
              thread_result.y1 = y1;
              thread_result.x0 = x0;
              thread_result.x1 = x1;
              thread_result.y0 = y0;
              thread_result.y1 = y1;
              for (int c = 0; c < 3; c++) {
                thread_result.inner[c] = inner_avg[c];
                thread_result.outer[c] = outer_avg[c];
              }
            }
          }
        }
      }
    }
#pragma omp critical
    if (max_thread_error > max_error) {
      max_error = max_thread_error;
      result.x0 = thread_result.x0;
      result.x1 = thread_result.x1;
      result.y0 = thread_result.y0;
      result.y1 = thread_result.y1;
      for (int c = 0; c < 3; c++) {
        result.inner[c] = thread_result.inner[c];
        result.outer[c] = thread_result.outer[c];
      }
    }
  }
  printTime();

  std::free(data_4d);
  std::free(sums);
  return result;
}