#include <algorithm>
#include <iostream>
#include <vector>
/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in in[x + y*nx]
- for each pixel (x, y), store the median of the pixels (a, b) which satisfy
  max(x-hx, 0) <= a < min(x+hx+1, nx), max(y-hy, 0) <= b < min(y+hy+1, ny)
  in out[x + y*nx].
*/
void mf(int ny, int nx, int hy, int hx, const float *in, float *out) {
  // std::cout << "hx, hy: " << hx << ", " << hy << std::endl;
  // std::cout << "nx, ny: " << nx << ", " << ny << std::endl;

  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      int x0 = i - hx < 0 ? 0 : i - hx;
      int x1 = i + hx >= nx ? nx : i + hx + 1;
      int y0 = j - hy < 0 ? 0 : j - hy;
      int y1 = j + hy >= ny ? ny : j + hy + 1;
      std::vector<float> window;
      for (int m = y0; m < y1; m++) {
        window.insert(window.end(), &in[x0 + nx * m], &in[x1 + nx * m]);
      }
      // for (float i : window) std::cout << i << ' ';

      // window.insert(window.end(), &in[i + nx * y0],&in[i + nx * y1]);

      // std::cout << "i, j: " << i << ", " << j << std::endl;
      // std::cout << "Vector size: " << window.size() << std::endl;
      // std::cout << "x0, x1: " << x0 << ", " << x1 << std::endl;
      // std::cout << "y0, y1: " << y0 << ", " << y1 << std::endl;
      int nth = window.size();
      // nth = nth % 2 ? nth / 2 + 1 : nth / 2;

      if (nth % 2) {
        nth /= 2;
        std::nth_element(window.begin(), window.begin() + nth, window.end());
        out[i + nx * j] = window[nth];
      } else {
        nth /= 2;
        std::nth_element(window.begin(), window.begin() + nth, window.end());
        float k = window[nth];
        std::nth_element(window.begin(), window.begin() + --nth, window.end());
        // std::cout << "k: " << window[nth] << std::endl << std::endl;
        out[i + nx * j] = (k + window[nth]) / 2;
      }
    }
  }
}