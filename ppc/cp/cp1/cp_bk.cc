#include <iostream>
#include <cmath>
#include <numeric>
#include <vector>
#include <algorithm>

/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- correlation between rows i and row j has to be stored in result[i + j*ny]
- only parts with 0 <= j <= i < ny need to be filled
*/
void correlate(int ny, int nx, const float *data, float *result)
{
    std::vector<double> means(ny);
    std::vector<double> std_devs(ny);
    std::vector<double> values(nx * ny);
    for (int y = 0, o=0; y < ny; o+=nx, y++)
    {
        means[y] = std::accumulate(&data[o], &data[o + nx], 0.0) / nx;
        std::transform(&data[o], &data[o + nx], values.begin() + o, [mean = means[y]](double x)
                       { return x - mean; });

        std_devs[y] = std::inner_product(values.begin() + o, values.begin() + o + nx, values.begin() + o, 0.0) / nx;
        std::transform(values.begin() + o, values.begin() + o + nx, values.begin() + o, [std_dev = std_devs[y]](double x)
                       { return x / std_dev; });


        double squared_sum = std::sqrt(std::inner_product(values.begin() + o, values.begin() + o + nx, values.begin() + o, 0.0));

        std::transform(values.begin() + o, values.begin() + o + nx, values.begin() + o, [squared_sum = squared_sum](double x)
                       { return x / squared_sum; });
    }

    for (int i = 0; i < ny; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            result[i + j * ny] = std::inner_product(values.begin() + i * nx, values.begin() + i * nx + nx, values.begin() + j * nx, 0.0);
            //result[j + i * ny] = result[i + j * ny];
        }
    }
    return;
}