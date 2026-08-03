#pragma once
#include "Complex.hpp"
#include "Allocator.hpp"
#include <vector>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class ComplexMatrix {
private:
    int rows, cols;
    std::vector<float, AlignedAllocator<float>> data;

public:
    ComplexMatrix(int r, int c) : rows(r), cols(c), data(r * c * 2, 0.0f) {}

    void set(int r, int c, float real, float imag) {
        int idx = (r * cols + c) * 2;
        data[idx] = real;
        data[idx + 1] = imag;
    }

    py::tuple get(int r, int c) const {
        int idx = (r * cols + c) * 2;
        return py::make_tuple(data[idx], data[idx + 1]);
    }

    int get_rows() const { return rows; }
    int get_cols() const { return cols; }

    ComplexMatrix multiply(const ComplexMatrix& B) const {
        if (cols != B.rows) throw std::runtime_error("Dim mismatch");
        ComplexMatrix C(rows, B.cols);
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < B.cols; ++c) {
                Complex sum(0,0);
                for (int k = 0; k < cols; ++k) {
                    Complex a(data[(r*cols+k)*2], data[(r*cols+k)*2+1]);
                    Complex b(B.data[(k*B.cols+c)*2], B.data[(k*B.cols+c)*2+1]);
                    sum = sum + (a * b);
                }
                C.set(r, c, sum.real, sum.imag);
            }
        }
        return C;
    }

    py::buffer_info get_buffer_info() {
        return py::buffer_info(data.data(), sizeof(float), py::format_descriptor<float>::format(), 2, { rows, cols * 2 }, { sizeof(float) * cols * 2, sizeof(float) });
    }
};
