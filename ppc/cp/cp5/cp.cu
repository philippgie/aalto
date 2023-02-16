#include <chrono>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <cuda_runtime.h>
using namespace std::chrono;


static inline void check(cudaError_t err, const char* context) {
    if (err != cudaSuccess) {
        std::cerr << "CUDA error: " << context << ": "
            << cudaGetErrorString(err) << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

#define CHECK(x) check(x, #x)

static inline int divup(int a, int b) {
    return (a + b - 1)/b;
}

static inline int roundup(int a, int b) {
    return divup(a, b) * b;
}

float average(const float *data, const int length);
float std_dev(const float *data, const int length);
float squared_sum(const float *data, const int length);


/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- correlation between rows i and row j has to be stored in result[i + j*ny]
- only parts with 0 <= j <= i < ny need to be filled
*/
__global__ void mykernel(int nx, int ny, const float *data, const float *data_t, float *result) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    if (i >= ny || j >= ny )
        return;
    if (i<j)
      result[ny * j + i] = 0.0;


    float res=0.0;
    for (int k = 0; k < nx; k++) {
      float x = data[k+ j* nx];
      float y = data_t[i+ k* ny];
      res += x * y;
    }
    result[ny * j + i] = res;
    //printf("%d is %f\n", ny*j+i,res);
}

void correlate(int ny, int nx, const float *data, float *result) {
    // Allocate memory & copy data to GPU
    float* dataGPU = NULL;
    CHECK(cudaMalloc((void**)&dataGPU, nx * ny * sizeof(float)));
    float* dataTGPU = NULL;
    CHECK(cudaMalloc((void**)&dataTGPU, nx * ny * sizeof(float)));
    float* resultGPU = NULL;
    CHECK(cudaMalloc((void**)&resultGPU, ny * ny * sizeof(float)));

    float *data_norm = new float[nx * ny]{0.0};
    auto start = high_resolution_clock::now();

    for (int y = 0; y < ny; y++) {
        float *values = &data_norm[y * nx];

        float mean = average(&data[y * nx], nx);
        // std::cout << "Mean of simple norm: " << mean << std::endl;
        for (int i = 0; i < nx; i++) {
          values[i] = data[y * nx + i] - mean;
        }

        float s = std_dev(values, nx);
        // std::cout << "Standard deviation of simple norm: " << s << std::endl;
        for (int i = 0; i < nx; i++) {
          values[i] /= s;
        }

        float factor = std::sqrt(squared_sum(values, nx));
        // std::cout << "Factor of squared norm: " << factor << std::endl;
        for (int i = 0; i < nx; i++) {
          values[i] /= factor;
        }
    }
        auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);
 
    std::cout << "Time taken by function: "
         << duration.count() << " milliseconds" << std::endl;

    float *data_t = new float[nx * ny]{0.0};
    for(int i = 0; i<nx;i++){
      for(int j = 0; j<ny;j++){
        data_t[j+ny*i]=data_norm[i+nx*j];
      }
    }
    //for(int i = 0; i<nx*ny;i++)
    //  std::cout<<data_t[i]<<std::endl;

    CHECK(cudaMemcpy(dataGPU, data_norm, nx * ny * sizeof(float), cudaMemcpyHostToDevice));
    CHECK(cudaMemcpy(dataTGPU, data_t, nx * ny * sizeof(float), cudaMemcpyHostToDevice));
    delete []data_norm;
    delete []data_t;


    // Run kernel
    dim3 dimBlock(16, 16);
    //dim3 dimGrid(divup(nx, dimBlock.x), divup(ny, dimBlock.y));
    dim3 dimGrid(divup(ny, dimBlock.y), divup(ny, dimBlock.y));
    mykernel<<<dimGrid, dimBlock>>>(nx, ny, dataGPU, dataTGPU, resultGPU);
    CHECK(cudaGetLastError());

    // Copy data back to CPU & release memory
    CHECK(cudaMemcpy(result, resultGPU, ny * ny * sizeof(float), cudaMemcpyDeviceToHost));
    CHECK(cudaFree(dataGPU));
    CHECK(cudaFree(dataTGPU));
    CHECK(cudaFree(resultGPU));


}

inline float average(const float *data, const int length) {
  float aggr = 0.0;
  for (int x = 0; x < length; x++) {
    aggr += static_cast<float>(data[x]);
  }
  return aggr / (float)length;
}

inline float std_dev(const float *data, const int length) {
  float aggr = 0.0;
  for (int x = 0; x < length; x++) {
    aggr += data[x] * data[x];
  }
  return aggr / (float)length;
}

inline float squared_sum(const float *data, const int length) {
  float aggr = 0.0;
  for (int x = 0; x < length; x++) {
    aggr += data[x] * data[x];
  }
  return aggr;
}
