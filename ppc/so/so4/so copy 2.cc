#include <omp.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>

typedef unsigned long long data_t;

#define PRINT_VAR(_variable)                                                \
  std::cout << "Thread id: " << omp_get_thread_num() << "\t:" << #_variable \
            << " is " << _variable << std::endl;

constexpr int MIN_CHUNK_SIZE = 2 << 14;

static inline int find_index(data_t* arr, int n, data_t K) {
  for (int i = 0; i < n; i++) {
    if (K < arr[i]) {
      return i;
    }
  }
  return n;

  // // Lower and upper bounds
  // int start = 0;
  // int end = n - 1;
  // // Traverse the search space
  // while (start < end) {
  //   int mid = (start + end) / 2;
  //   // If K is found
  //   if (arr[mid] == K)
  //     return mid;
  //   else if (arr[mid] < K)
  //     start = mid + 1;
  //   else
  //     end = mid - 1;
  // }
  // // Return insert position
  // return end + 1;
}

void merge(data_t* A, data_t* B, int size_A, int size_B, data_t* tmp) {
  int m = size_A;
  int n = size_B;
  if (m < MIN_CHUNK_SIZE && n < MIN_CHUNK_SIZE) {
    std::merge(A, A + m, B, B + n, tmp);
  }
  if (std::max(m, n) == 0) return;

  if (m < n) {
    data_t* tmp_data = A;
    A = B;
    B = tmp_data;

    int tmp_size = m;
    m = n;
    n = tmp_size;
  }

  int r = m / 2;
  // int s = find_index(B, n, A[r]);
  int s = std::lower_bound(B, B + n, A[r]) - B;

  int t = r + s;
  // #pragma omp critical
  //   {
  //     PRINT_VAR(m);
  //     PRINT_VAR(n);
  //     PRINT_VAR(r);
  //     PRINT_VAR(s);
  //     PRINT_VAR(t);
  //   }
  tmp[t] = A[r];

#pragma omp taskgroup
  {
#pragma omp task shared(A, B, tmp)
    { merge(A, B, r, s, tmp); }

#pragma omp task shared(A, B, tmp)
    { merge(A + r + 1, B + s, m - r - 1, n - s, tmp + t + 1); }

#pragma omp taskyield
  }
}

void mergesort(data_t* v, int n, data_t* tmp) {
  if (n < MIN_CHUNK_SIZE) {
    std::sort(v, v + n);
    return;
  }

  int mid = n / 2;
#pragma omp taskgroup
  {
#pragma omp task shared(v, tmp)
    mergesort(v, mid, tmp);

#pragma omp task shared(v, tmp)
    mergesort(v + mid, n - mid, tmp + mid);

#pragma omp taskyield
  }
  merge(v, v + mid, mid, n - mid, tmp);
  // std::merge(v, v + mid, v + mid, v + n, tmp);
  std::copy(tmp, tmp + n, v);
}

void psort(int n, data_t* data) {
  // FIXME: Implement a more efficient parallel sorting algorithm for
  // the CPU, using the basic idea of merge sort. std::sort(data, data +
  // n);

  data_t* tmp = new data_t[n];
#pragma omp parallel
  {
#pragma omp single
    mergesort(data, n, tmp);
  }

  delete[] tmp;
}
