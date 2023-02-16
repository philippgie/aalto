#include <x86intrin.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

std::chrono::steady_clock::time_point begin;
std::chrono::steady_clock::time_point end;
typedef float float8_t __attribute__((vector_size(8 * sizeof(float))));

constexpr float8_t f8zero = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
// constexpr int P = 8;

static float8_t *float8_alloc(std::size_t n) {
  void *tmp = 0;
  if (posix_memalign(&tmp, sizeof(float8_t), sizeof(float8_t) * n)) {
    throw std::bad_alloc();
  }
  return (float8_t *)tmp;
}

inline float reduce_float8(float8_t d) {
  float aggr = 0.0;
  for (int i = 0; i < 8; i++) {
    aggr += d[i];
  }
  return aggr;
}

static inline float8_t swap4(float8_t x) {
  return _mm256_permute2f128_ps(x, x, 0b00000001);
}
static inline float8_t swap2(float8_t x) {
  return _mm256_permute_ps(x, 0b01001110);
}
static inline float8_t swap1(float8_t x) {
  return _mm256_permute_ps(x, 0b10110001);
}

// void printrow(float8_t *values, int nx) {
//   for (int i = 0; i < nx; i++) {
//     std::cout << values[i / 4][i % 4] << " ";
//   }
//   std::cout << std::endl;
//   std::cout << std::endl;
// }

float sum(const float8_t *data, const int length);
// float std_dev(const float8_t *data, const int length);
float squared_sum(const float8_t *data, const int length);

void print_vd(float8_t *vd, int dimA, int dimB) {
  for (int i = 0; i < dimA; i++) {
    for (int j = 0; j < 8; j++) {
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
  constexpr int nb = 8;
  // vectors per input column
  int na = (ny + nb - 1) / nb;

  // input data, padded, converted to vectors
  float8_t *vd = float8_alloc(nx * na);
  // input data, transposed, padded, converted to vectors

  // std::chrono::steady_clock::time_point begin;
  // std::chrono::steady_clock::time_point end;

  int nc = nx % 8 ? nx / 8 + 1 : nx / 8;
  float8_t *data_norm = float8_alloc(nc * ny);

#pragma omp parallel for
  for (int y = 0; y < ny; y++) {
    float8_t *values = &data_norm[y * nc];
    values[nc - 1] = f8zero;

    for (int i = 0; i < nx; i++) {
      values[i / 8][i % 8] = data[y * nx + i];
    }

    float mean = sum(values, nc) / nx;
    // std::cout << "Mean of simple norm: " << mean << std::endl;
    for (int i = 0; i < nc; i++) {
      values[i] = data_norm[y * nc + i] - mean;
    }
    if (nx % 8)
      for (int i = nx % 8; i < 8; i++) {
        values[nc - 1][i] = 0.0;
      }

    float s = squared_sum(values, nc) / nx;
    // std::cout << "Standard deviation of simple norm: " << s << std::endl;
    for (int i = 0; i < nc; i++) {
      values[i] /= s;
    }

    float factor = std::sqrt(squared_sum(values, nc));
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

  std::vector<std::tuple<int, int, int>> rows(na * (na + 1) / 2);

  int offsets[na];
  offsets[0] = 0;
  for (int i = 1; i < na; i++) {
    offsets[i] = offsets[i - 1] + na - i + 1;
  }

#pragma omp parallel for collapse(2)
  for (int ia = 0; ia < na; ++ia) {
    for (int ja = 0; ja < na; ++ja) {
      if (ja < ia) continue;
      int ija = _pdep_u32(ia, 0x55555555) | _pdep_u32(ja, 0xAAAAAAAA);
      rows[offsets[ia] + ja - ia] = std::make_tuple(ija, ia, ja);
    }
  }
  std::sort(rows.begin(), rows.end());

//  begin = std::chrono::steady_clock::now();
#pragma omp parallel for
  for (long unsigned int i = 0; i < rows.size(); ++i) {
    auto t = rows[i];
    int ia = std::get<1>(t);
    int ja = std::get<2>(t);
    float8_t vv000 = f8zero;
    float8_t vv001 = f8zero;
    float8_t vv010 = f8zero;
    float8_t vv011 = f8zero;
    float8_t vv100 = f8zero;
    float8_t vv101 = f8zero;
    float8_t vv110 = f8zero;
    float8_t vv111 = f8zero;
    for (int k = 0; k < nx; ++k) {
      constexpr int PF = 40;
      __builtin_prefetch(&vd[nx * ia + k + PF]);
      __builtin_prefetch(&vd[nx * ja + k + PF]);
      float8_t a000 = vd[nx * ia + k];
      float8_t b000 = vd[nx * ja + k];
      float8_t a100 = swap4(a000);
      float8_t a010 = swap2(a000);
      float8_t a110 = swap2(a100);
      float8_t b001 = swap1(b000);
      vv000 += a000 * b000;
      vv001 += a000 * b001;
      vv010 += a010 * b000;
      vv011 += a010 * b001;
      vv100 += a100 * b000;
      vv101 += a100 * b001;
      vv110 += a110 * b000;
      vv111 += a110 * b001;
    }
    float8_t vv[8] = {vv000, vv001, vv010, vv011, vv100, vv101, vv110, vv111};

    for (int kb = 1; kb < 8; kb += 2) {
      vv[kb] = swap1(vv[kb]);
    }
    for (int jb = 0; jb < 8; ++jb) {
      for (int ib = 0; ib < 8; ++ib) {
        int i = ib + ia * 8;
        int j = jb + ja * 8;
        if (j < ny && i < ny && i <= j) {
          result[ny * i + j] = vv[ib ^ jb][jb];
        }
      }
    }
    //}
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

inline float sum(const float8_t *data, const int length) {
  float8_t aggr = f8zero;
  for (int x = 0; x < length; x++) {
    aggr += data[x];
  }
  return reduce_float8(aggr);
}

inline float squared_sum(const float8_t *data, const int length) {
  float8_t aggr = f8zero;
  for (int x = 0; x < length; x++) {
    aggr += data[x] * data[x];
  }
  return reduce_float8(aggr);
}