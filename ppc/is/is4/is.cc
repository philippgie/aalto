struct Result {
  int y0;
  int x0;
  int y1;
  int x1;
  float outer[3];
  float inner[3];
};

static inline int color(int y, int x, int c, int nx, int ny,
                        const float* data) {
  return data[c + 3 * x + 3 * nx * y];
}
static inline int error_inside(int y, int x, int c) {}
static inline int error_inside(int y, int x, int c) {}

/*
This is the function you need to implement. Quick reference:
- x coordinates: 0 <= x < nx
- y coordinates: 0 <= y < ny
- color components: 0 <= c < 3
- input: data[c + 3 * x + 3 * nx * y]
*/
Result segment(int ny, int nx, const float* data) {
  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      /* code */
    }
  }

  Result result{0, 0, 0, 0, {0, 0, 0}, {0, 0, 0}};

  return result;
}
