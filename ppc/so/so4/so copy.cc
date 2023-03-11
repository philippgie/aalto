#include <omp.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>

typedef unsigned long long data_t;

#define PRINT_VAR(_variable)                                                \
  std::cout << "Thread id: " << omp_get_thread_num() << "\t:" << #_variable \
            << " is " << _variable << std::endl;

void parallelMergesort(int n, data_t* A, data_t* tmp) {
  if (n < 2) return;
    // PRINT_VAR(n_threads);

#pragma omp task  // firstprivate(A, n, tmp)
  parallelMergesort(n / 2, A, tmp);

#pragma omp task  // firstprivate(A, n, tmp)
  parallelMergesort(n - n / 2, A + n / 2, tmp + n / 2);

#pragma omp taskwait

  /* merge sorted halves into sorted list */
  std::merge(A, A + n / 2, A + n / 2, A + n, tmp);
  std::copy(tmp, tmp + n, A);
}

void parallelMerge(data_t* A, data_t* B, data_t* C, int size_A, int size_B,
                   int basecase) {}

void psort(int n, data_t* data) {
  // FIXME: Implement a more efficient parallel sorting algorithm for
  // the CPU, using the basic idea of merge sort. std::sort(data, data +
  // n);

  data_t* tmp = new data_t[n];

  int n_threads = omp_get_max_threads();
  int n_per_thread = n_threads > n ? 1 : n / n_threads;
#pragma omp parallel
  {
    int tid = omp_get_thread_num();
#pragma omp for
    for (int i = 0; i < n_threads; i++) {
      if (tid == n_threads - 1) {
        int t = (n_threads - 1) * n_per_thread;
        std::sort(data + t, data + n);
      } else {
        std::sort(data + tid * n_per_thread, data + (tid + 1) * n_per_thread);
      }
    }
  }

#pragma omp parallel
  {
    for (int i = n_threads / 2; i < ceil(log2(n_threads)); i++) {
      if () std::merge()
    }
  }

  // #pragma omp parallel
  //{
  //   #pragma omp single
  //
  // }
  delete[] tmp;
}
