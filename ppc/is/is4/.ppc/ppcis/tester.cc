#include "is.h"
#include "ppc.h"
#include "tests.h"

#include <random>
#include <vector>

static constexpr float RELATIVE_THRESHOLD = 0.000001;
static constexpr float THRESHOLD = 0.0001;

std::unique_ptr<ppc::fdostream> stream;

static double total_cost(int ny, int nx, const float *data, const Result &res) {
    double error[3] = {};
    for (int y = 0; y < ny; y++) {
        for (int x = 0; x < nx; x++) {
            const float *color;
            if (res.x0 <= x && x < res.x1 && res.y0 <= y && y < res.y1) {
                color = res.inner;
            } else {
                color = res.outer;
            }
            for (int c = 0; c < 3; c++) {
                double diff = (double)color[c] - (double)data[c + 3 * x + 3 * nx * y];
                error[c] += diff * diff;
            }
        }
    }
    return error[0] + error[1] + error[2];
}

static void dump(const float (&a)[3]) {
    *stream << std::scientific << a[0] << "," << std::scientific << a[1] << "," << std::scientific << a[2];
}

static void dump(const Result &r) {
    *stream
        << "y0\t" << r.y0 << "\n"
        << "x0\t" << r.x0 << "\n"
        << "y1\t" << r.y1 << "\n"
        << "x1\t" << r.x1 << "\n"
        << "outer\t";
    dump(r.outer);
    *stream << "\n";
    *stream << "inner\t";
    dump(r.inner);
    *stream << "\n";
}

static bool close(float a, float b) {
    return std::abs(a - b) < THRESHOLD;
}

static bool equal(const float (&a)[3], const float (&b)[3]) {
    return close(a[0], b[0]) && close(a[1], b[1]) && close(a[2], b[2]);
}

static void compare(bool is_test, int ny, int nx, const Result &e, const Result &r, const float *data) {
    if (is_test) {
        if (e.y0 == r.y0 && e.x0 == r.x0 && e.y1 == r.y1 && e.x1 == r.x1 && equal(e.outer, r.outer) && equal(e.inner, r.inner)) {
            *stream << "result\tpass\n";
        } else {
            double expected_cost = total_cost(ny, nx, data, e);
            double returned_cost = total_cost(ny, nx, data, r);
            double ub = expected_cost * (1.0 + RELATIVE_THRESHOLD);
            double lb = expected_cost * (1.0 - RELATIVE_THRESHOLD);
            if (lb < returned_cost && returned_cost < ub) {
                *stream << "result\tpass\n";
            } else {
                bool small = ny * nx <= 200;
                stream->precision(std::numeric_limits<float>::max_digits10 - 1);
                *stream
                    << "result\tfail\n"
                    << "threshold\t" << std::scientific << THRESHOLD << '\n'
                    << "ny\t" << ny << "\n"
                    << "nx\t" << nx << "\n"
                    << "what\texpected\n";
                dump(e);
                *stream << "what\tgot\n";
                dump(r);
                *stream << "size\t" << (small ? "small" : "large") << '\n';
                if (small) {
                    for (int y = 0; y < ny; ++y) {
                        for (int x = 0; x < nx; ++x) {
                            const float *p = &data[3 * x + 3 * nx * y];
                            const float v[3] = {p[0], p[1], p[2]};
                            *stream << "triple\t";
                            dump(v);
                            *stream << "\n";
                        }
                    }
                }
            }
        }
    } else {
        *stream << "result\tdone\n";
    }
    *stream << std::flush;
}

static void test(bool is_test, const ISTestCase &test) {
    TestCaseInstance data = test.generate();

    Result r;
    {
        ppc::setup_cuda_device();
        ppc::perf timer;
        timer.start();
        r = segment(data.Ny, data.Nx, data.Data.data());
        timer.stop();
        timer.print_to(*stream);
        ppc::reset_cuda_device();
    }
    compare(is_test, data.Ny, data.Nx, data.Expected, r, data.Data.data());
}

int main(int argc, char **argv) {
    const char *ppc_output = std::getenv("PPC_OUTPUT");
    int ppc_output_fd = 0;
    if (ppc_output) {
        ppc_output_fd = std::stoi(ppc_output);
    }
    if (ppc_output_fd <= 0) {
        ppc_output_fd = 1;
    }
    stream = std::unique_ptr<ppc::fdostream>(new ppc::fdostream(ppc_output_fd));

    argc--;
    argv++;
    if (argc < 1 || argc > 3) {
        std::cerr << "Invalid usage" << std::endl;
        std::exit(1);
    }

    bool is_test = false;
    if (argv[0] == std::string("--test")) {
        is_test = true;
        argc--;
        argv++;
    }

    std::ifstream input_file(argv[0]);
    if (!input_file) {
        std::cerr << "Failed to open input file" << std::endl;
        std::exit(1);
    }

    std::string input_type;
    CHECK_READ(input_file >> input_type);
    if (input_type == "timeout") {
        input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        CHECK_READ(input_file >> input_type);
    }

    auto test_case = make_test_case(input_type, input_file);
    test(is_test, *test_case);

    return 0;
}
