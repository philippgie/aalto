// Copyright (c) 2022, Aalto University, erik.schultheis@aalto.fi
// All rights reserved.
//
// SPDX-License-Identifier: MIT

#ifndef PPC_MOOC_COURSES_EXERCISES_IS_PPCIS_TESTS_H
#define PPC_MOOC_COURSES_EXERCISES_IS_PPCIS_TESTS_H

#include "is.h"
#include "ppc.h"

static constexpr float MINDIFF = 0.001f;

// put everything into an anonymous namespace so it cannot interact with user code.
namespace {

// Instance of a test case, with concrete input and expected output.
struct TestCaseInstance {
    int Ny, Nx;
    std::vector<float> Data;
    Result Expected;
};

// Utility struct for specifying a rectangle
struct Rect {
    int Y0, X0, Y1, X1;
};

// Base class for all test cases. Concrete test cases need to implement the `generate` function.
class ISTestCase {
  public:
    ISTestCase(int ny, int nx) : Ny(ny), Nx(nx){};
    virtual ~ISTestCase() = default;

    TestCaseInstance generate() const {
        ppc::random rng(uint32_t(Ny) * 0x1234567 + uint32_t(Nx));
        return generate(rng);
    }

    virtual TestCaseInstance generate(ppc::random &rng) const = 0;

  protected:
    int Ny, Nx;
};

// some helper functions
inline bool inside(int v, int a, int b) {
    return a <= v && v < b;
}

inline bool is_inside(const Result &r, int y, int x) {
    return inside(y, r.y0, r.y1) && inside(x, r.x0, r.x1);
}

void colours(ppc::random &rng, float a[3], float b[3]) {
    float maxdiff = 0;
    do {
        bool done = false;
        while (!done) {
            for (int k = 0; k < 3; ++k) {
                a[k] = rng.get_double();
                b[k] = rng.get_uint64(0, 1) ? rng.get_double() : a[k];
                if (a[k] != b[k]) {
                    done = true;
                }
            }
        }
        maxdiff = std::max({std::abs(a[0] - b[0]),
                            std::abs(a[1] - b[1]),
                            std::abs(a[2] - b[2])});
    } while (maxdiff < MINDIFF);
}

void find_avgs(int ny, int nx, const float *data, Result &res) {
    double inner[3] = {};
    double outer[3] = {};
    for (int y = 0; y < ny; y++) {
        for (int x = 0; x < nx; x++) {
            double *color;
            if (is_inside(res, y, x)) {
                color = inner;
            } else {
                color = outer;
            }
            for (int c = 0; c < 3; c++) {
                color[c] += (double)data[c + 3 * x + 3 * nx * y];
            }
        }
    }
    double size = (res.y1 - res.y0) * (res.x1 - res.x0);
    for (int c = 0; c < 3; c++) {
        res.inner[c] = inner[c] / (size);
        res.outer[c] = outer[c] / (nx * ny - size);
    }
}

// Rect test case: Generate a monochromatic image with a rectangle in a different color painted on.
// In that case, we know exactly what the solution will be.
class RectTestCase : public ISTestCase {
  public:
    RectTestCase(int ny, int nx, bool binary, bool worstcase) : ISTestCase(ny, nx), Binary(binary), WorstCase(worstcase) {
        if (ny * nx <= 2) {
            throw std::invalid_argument("Invalid dimensions");
        }
    }

    TestCaseInstance generate(ppc::random &rng) const override {
        // set up rectangle location
        Rect loc = choose_location(rng);

        Result result{loc.Y0, loc.X0, loc.Y1, loc.X1, {}, {}};

        // set up rectangle color
        if (Binary) {
            bool flip = rng.get_uint64(0, 1);
            for (int c = 0; c < 3; ++c) {
                result.inner[c] = flip ? 0.0f : 1.0f;
                result.outer[c] = flip ? 1.0f : 0.0f;
            }
        } else {
            if (WorstCase) {
                // Test worst-case scenario
                for (int c = 0; c < 3; ++c) {
                    result.inner[c] = 1.0f;
                    result.outer[c] = 1.0f;
                }
                result.outer[0] -= MINDIFF;
            } else {
                // Random but distinct colours
                colours(rng, result.inner, result.outer);
            }
        }

        // generate image
        std::vector<float> data(3 * Ny * Nx);
        for (int y = 0; y < Ny; ++y) {
            for (int x = 0; x < Nx; ++x) {
                for (int c = 0; c < 3; ++c) {
                    bool inside = loc.Y0 <= y && y < loc.Y1 && loc.X0 <= x && x < loc.X1;
                    data[c + 3 * x + 3 * Nx * y] = inside ? result.inner[c] : result.outer[c];
                }
            }
        }

        return {Ny, Nx, std::move(data), result};
    }

    // Generate a random location where the rectangle shall be placed.
    Rect choose_location(ppc::random &rng) const {
        int y0, x0, y1, x1;
        do {
            // Random box location
            y0 = rng.get_int32(0, Ny - 1);
            x0 = rng.get_int32(0, Nx - 1);

            y1 = rng.get_int32(y0 + 1, Ny);
            x1 = rng.get_int32(x0 + 1, Nx);

            // Avoid ambiguous cases
            if (y0 == 0 && y1 == Ny && (x0 == 0 || x1 == Nx)) {
            } else if (x0 == 0 && x1 == Nx && (y0 == 0 || y1 == Ny)) {
            } else {
                return {y0, x0, y1, x1};
            }
        } while (true);
    }

  private:
    bool Binary;
    bool WorstCase;
};

// A monochromatic image with a rectangle on it. The rectangle consists of one part in a single color, and one part following a gradient.
// In this case, we can relatively easily try out all candidate rectangles to find the solution.
class GradientTestCase : public ISTestCase {
  public:
    GradientTestCase(int ny, int nx) : ISTestCase(ny, nx) {
        if (ny * nx <= 2) {
            throw std::invalid_argument("Invalid dimensions");
        }
    }

    TestCaseInstance generate(ppc::random &rng) const override {
        bool ok = false;
        int x0, x1, y0, y1, y2;
        while (!ok) {
            // Random box location
            x0 = rng.get_int32(0, Nx - 1);
            x1 = rng.get_int32(x0 + 1, Nx);
            y0 = rng.get_int32(0, Ny - 1);
            y1 = rng.get_int32(y0 + 1, Ny);
            y2 = rng.get_int32(y1, Ny);
            // Avoid ambiguous cases
            if ((x0 > 0 && x1 < Nx && y0 > 0 && y2 < Ny))
                ok = true;
        }

        std::vector<float> bitmap(Nx * Ny * 3);
        const float fact = 1.0f / float(y2 - y1);

        float color_outer;
        float color_inner;
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        do {
            color_outer = rng.get_float(0.0f, 1.0f);
            color_inner = rng.get_float(0.0f, 1.0f);
        } while (std::abs(color_outer - color_inner) < MINDIFF);

        for (int y = 0; y < Ny; ++y) {
            const bool yinside = inside(y, y0, y1);
            const bool yinside_gradient = inside(y, y1, y2);
            for (int x = 0; x < Nx; ++x) {
                const auto pixel_base = (Nx * y + x) * 3;
                const bool xinside = inside(x, x0, x1);
                const bool inside = yinside && xinside;
                const bool inside_gradient = yinside_gradient && xinside;

                if (inside) {
                    for (int c = 0; c < 3; ++c) {
                        bitmap[pixel_base + c] = color_inner;
                    }
                } else if (inside_gradient) {
                    const float val = float(y2 - y) * fact * (color_inner - color_outer) + color_outer;
                    for (int c = 0; c < 3; ++c) {
                        bitmap[pixel_base + c] = val;
                    }
                } else {
                    for (int c = 0; c < 3; ++c) {
                        bitmap[pixel_base + c] = color_outer;
                    }
                }
            }
        }

        Result e = segment_gradient(x0, x1, y0, y1, y2, bitmap.data());
        return {Ny, Nx, std::move(bitmap), e};
    }

    Result segment_gradient(int x0, int x1, int y0, int y1, int y2, const float *data) const {
        // We know all the boundaries, except inside the gradient
        pfloat color_outer;
        pfloat color_inner = data[3 * (Nx * y0 + x0)];

        if (x0 > 0 || y0 > 0) {
            const pfloat gr_color = data[0];
            color_outer = gr_color;
        } else if (x1 < Nx || y2 < Ny) {
            const pfloat gr_color = data[3 * (Nx * Ny) - 1];
            color_outer = gr_color;
        } else {
            throw;
        } // situation should not exist

        const pfloat sumcolor_top = (x1 - x0) * (y1 - y0) * color_inner;
        pfloat min_sqerror = std::numeric_limits<double>::max();
        Result e;

        // calculate all end positions (naively)
        for (int yend = y1; yend <= y2; ++yend) {
            pfloat sumcolor_inside = sumcolor_top;
            for (int y = y1; y < yend; ++y) {
                const int pixel_base = 3 * (Nx * y + x0);
                const pfloat gr_color = data[pixel_base];
                sumcolor_inside += (x1 - x0) * gr_color;
            }

            pfloat sumcolor_outside = (Ny * Nx - (x1 - x0) * (y2 - y0)) * color_outer;
            for (int y = yend; y < y2; ++y) {
                const int pixel_base = 3 * (Nx * y + x0);
                const pfloat gr_color = data[pixel_base];
                sumcolor_outside += (x1 - x0) * gr_color;
            }

            const pfloat pixels_inside = pfloat((yend - y0) * (x1 - x0));
            const pfloat pixels_outside = pfloat(Ny * Nx) - pixels_inside;

            const pfloat color_inside = sumcolor_inside / pixels_inside;
            const pfloat color_outside = sumcolor_outside / pixels_outside;

            pfloat sqerror_inside = (x1 - x0) * (y1 - y0) * (color_inner - color_inside) * (color_inner - color_inside);
            for (int y = y1; y < yend; ++y) {
                const int pixel_base = 3 * (Nx * y + x0);
                const pfloat gr_color = data[pixel_base];
                sqerror_inside += (x1 - x0) * (gr_color - color_inside) * (gr_color - color_inside);
            }

            pfloat sqerror_outside = ((Ny * Nx) - (x1 - x0) * (y2 - y0)) * (color_outer - color_outside) * (color_outer - color_outside);
            for (int y = yend; y < y2; ++y) {
                const int pixel_base = 3 * (Nx * y + x0);
                const pfloat gr_color = data[pixel_base];
                sqerror_outside += (x1 - x0) * (gr_color - color_outside) * (gr_color - color_outside);
            }

            const pfloat sqerror = 3.0 * (sqerror_inside + sqerror_outside);
            if (sqerror < min_sqerror) {
                min_sqerror = sqerror;
                for (int c = 0; c < 3; ++c) {
                    e.outer[c] = color_outside;
                    e.inner[c] = color_inside;
                }
                e.y0 = y0;
                e.y1 = yend;

                e.x0 = x0;
                e.x1 = x1;
            }
        }
        return e;
    }
};

// Takes a pointer to three floats and normalizes them so that
// -1 <= a[c] <= 1 with at least one equality (approx.)
void normalize_uniform(float *a) {
    float m = 1.0 / std::max({std::abs(a[0]), std::abs(a[1]), std::abs(a[2])});
    a[0] *= m;
    a[1] *= m;
    a[2] *= m;
}

// Generates a voronoi pattern as the input. In case of binary images, we use the red channel as probability for drawing the binary values for the
// pixels.
// TODO Document the logic behind this test
class VoronoiTestCase : public ISTestCase {
  public:
    VoronoiTestCase(int ny, int nx, bool binary, int n_points, float color_offset, float rect_strength, Rect target) : ISTestCase(ny, nx),
                                                                                                                       Binary(binary), NPoints(n_points), ColorOffset(color_offset), RectStrength(rect_strength), Target(target) {
        if (ny * nx <= 2) {
            throw std::invalid_argument("Invalid dimensions");
        }
    }

    TestCaseInstance generate(ppc::random &rng) const override {
        auto pixels = generate_voronoi(rng);
        Result res = {Target.Y0, Target.X0, Target.Y1, Target.X1, {}, {}};
        find_avgs(Ny, Nx, pixels.data(), res);
        return {Ny, Nx, std::move(pixels), res};
    }

    std::vector<float> generate_voronoi(ppc::random &rng) const {
        const float voronoi_max = 0.5 - (ColorOffset + RectStrength);
        std::vector<float> image(Nx * Ny * 3, 0.5);
        std::vector<int> ys(NPoints);
        std::vector<int> xs(NPoints);
        std::vector<float> colors(3 * NPoints);
        for (int k = 0; k < NPoints; k++) {
            ys[k] = rng.get_int32(0, Ny - 1);
            xs[k] = rng.get_int32(0, Nx - 1);
            colors[3 * k + 0] = rng.get_float(-1.0, 1.0);
            colors[3 * k + 1] = rng.get_float(-1.0, 1.0);
            colors[3 * k + 2] = rng.get_float(-1.0, 1.0);
            normalize_uniform(&colors[3 * k]);
        }

        offset_rect(rng, image.data());

        for (int y = 0; y < Ny; ++y) {
            for (int x = 0; x < Nx; ++x) {
                int dx0 = xs[0] - x;
                int dy0 = ys[0] - y;
                int dsq = dx0 * dx0 + dy0 * dy0;
                int closest = 0;
                for (int k = 1; k < NPoints; k++) {
                    int dx = xs[k] - x;
                    int dy = ys[k] - y;
                    int ndsq = dx * dx + dy * dy;
                    if (ndsq < dsq) {
                        dsq = ndsq;
                        closest = k;
                    }
                }
                image[0 + 3 * x + 3 * Nx * y] += colors[3 * closest + 0] * voronoi_max;
                image[1 + 3 * x + 3 * Nx * y] += colors[3 * closest + 1] * voronoi_max;
                image[2 + 3 * x + 3 * Nx * y] += colors[3 * closest + 2] * voronoi_max;
            }
        }

        // for bitmaps, use the red channel as the probability P(color == 1.0)
        if (Binary) {
            for (int w = 0; w < Nx * Ny; w++) {
                float p = image[3 * w];
                float val = rng.get_float(0.0, 1.0) < p ? 1.0 : 0.0;
                image[3 * w] = val;
                image[3 * w + 1] = val;
                image[3 * w + 2] = val;
            }
        }

        return image;
    }

    void offset_rect(ppc::random &rng, float *image) const {
        float a[3];
        float b[3];
        for (int c = 0; c < 3; ++c) {
            a[c] = rng.get_float(-1.0, 1.0);
            b[c] = rng.get_float(-1.0, 1.0);
        }
        if (Binary) {
            // for binary inputs we only care about the first channel, so the
            // directional effect needs to be in that direction
            b[0] = b[0] > 0 ? 1 : -1;
        }
        normalize_uniform(b);

        for (int c = 0; c < 3; ++c) {
            float bt = b[c];
            b[c] = ColorOffset * a[c] + RectStrength * bt;
            a[c] = ColorOffset * a[c] - RectStrength * bt;
        }

        int h = rng.get_int32(Ny / 3, Ny);
        int w = rng.get_int32(Nx / 3, Nx);
        int y0 = rng.get_int32(0, Ny - h);
        int x0 = rng.get_int32(0, Nx - w);

        for (int y = 0; y < Ny; ++y) {
            for (int x = 0; x < Nx; ++x) {
                const float *color;
                if (x0 <= x && x < x0 + w && y0 <= y && y < y0 + h) {
                    color = a;
                } else {
                    color = b;
                }
                for (int c = 0; c < 3; ++c) {
                    image[c + 3 * x + 3 * Nx * y] += color[c];
                }
            }
        }
    }

  private:
    bool Binary;
    int NPoints;
    float ColorOffset, RectStrength;
    Rect Target;
};

// A test case that loads a fixed input image from the test case file.
class FixedTest : public ISTestCase {
  public:
    FixedTest(int ny, int nx, std::istream &source) : ISTestCase(ny, nx), Data(3 * ny * nx) {
        // This test case loads the expected correct position and the contents of the image
        // from the test definition file. The first four numbers that are read are y0 x0 y1 y2,
        // then ny * nx * 3 numbers are read for the image data.
        int x0, y0, x1, y1;
        CHECK_READ(source >> y0);
        CHECK_READ(source >> x0);
        CHECK_READ(source >> y1);
        CHECK_READ(source >> x1);

        for (int y = 0; y < Ny; ++y) {
            for (int x = 0; x < Nx; ++x) {
                for (int c = 0; c < 3; ++c) {
                    double value;
                    CHECK_READ(source >> value);
                    Data[c + 3 * x + 3 * Nx * y] = value;
                }
            }
        }
        Expected.y0 = y0;
        Expected.x0 = x0;
        Expected.y1 = y1;
        Expected.x1 = x1;

        find_avgs(ny, nx, Data.data(), Expected);
    }

    TestCaseInstance generate(ppc::random &) const override {
        return {Ny, Nx, Data, Expected};
    }

  private:
    std::vector<float> Data;
    Result Expected;
};

// Given the type and the spec file, generate a test case.
std::unique_ptr<ISTestCase> make_test_case(const std::string &input_type, std::istream &input_file) {
    int ny;
    int nx;
    CHECK_READ(input_file >> ny >> nx);

    if (input_type == "structured-color") {
        return std::make_unique<RectTestCase>(ny, nx, false, false);
    } else if (input_type == "structured-worstcase") {
        return std::make_unique<RectTestCase>(ny, nx, false, true);
    } else if (input_type == "structured-binary") {
        return std::make_unique<RectTestCase>(ny, nx, true, false);
    } else if (input_type == "gradient") {
        return std::make_unique<GradientTestCase>(ny, nx);
    } else if (input_type == "precomputed-voronoi" || input_type == "precomputed-voronoi-binary") {
        bool binary = input_type == "precomputed-voronoi-binary";
        int n_points;
        float color_offset, rect_strength;
        CHECK_READ(input_file >> n_points >> color_offset >> rect_strength);
        std::string label;
        CHECK_READ(input_file >> label);
        if (label == "expected-rectangle") {
            Rect target;
            CHECK_READ(input_file >> target.Y0 >> target.X0 >> target.Y1 >> target.X1);
            return std::make_unique<VoronoiTestCase>(ny, nx, binary, n_points, color_offset, rect_strength, target);
        } else {
            std::cerr << "Invalid test syntax for given type" << std::endl;
            std::exit(1);
        }
    } else if (input_type == "fixed") {
        return std::make_unique<FixedTest>(ny, nx, input_file);
    } else {
        std::cerr << "Invalid input type" << std::endl;
        std::exit(1);
    }
}

} // namespace

#endif // PPC_MOOC_COURSES_EXERCISES_IS_PPCIS_TESTS_H
