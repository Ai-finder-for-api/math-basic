#include "Simd.hpp"

class ComplexMatrix {
private:
    int rows, cols;
    std::vector<float> data; // Interleaved [r00, i00, r01, i01, ...]

public:
    ComplexMatrix(int r, int c) : rows(r), cols(c), data(r * c * 2, 0.0f) {}

    // Setters/Getters
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

    // Standard Matrix Multiplication (A * B)
    ComplexMatrix multiply(const ComplexMatrix& B) const {
        if (cols != B.rows) throw std::runtime_error("Matrix dimensions mismatch");
        ComplexMatrix C(rows, B.cols);
        
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < B.cols; ++c) {
                Complex sum(0.0f, 0.0f);
                for (int k = 0; k < cols; ++k) {
                    Complex a(data[(r * cols + k) * 2], data[(r * cols + k) * 2 + 1]);
                    Complex b(B.data[(k * B.cols + c) * 2], B.data[(k * B.cols + c) * 2 + 1]);
                    sum = sum + (a * b);
                }
                C.set(r, c, sum.real, sum.imag);
            }
        }
        return C;
    }

    // Kronecker Product (Tensor Product) - A ⊗ B
    ComplexMatrix tensor(const ComplexMatrix& B) const {
        ComplexMatrix C(rows * B.rows, cols * B.cols);
        
        for (int r1 = 0; r1 < rows; ++r1) {
            for (int c1 = 0; c1 < cols; ++c1) {
                Complex a(data[(r1 * cols + c1) * 2], data[(r1 * cols + c1) * 2 + 1]);
                
                for (int r2 = 0; r2 < B.rows; ++r2) {
                    for (int c2 = 0; c2 < B.cols; ++c2) {
                        Complex b(B.data[(r2 * B.cols + c2) * 2], B.data[(r2 * B.cols + c2) * 2 + 1]);
                        Complex res = a * b;
                        
                        int target_r = r1 * B.rows + r2;
                        int target_c = c1 * B.cols + c2;
                        C.set(target_r, target_c, res.real, res.imag);
                    }
                }
            }
        }
        return C;
    }

    // Zero-copy buffer protocol for Python/NumPy
    py::buffer_info get_buffer_info() {
        return py::buffer_info(
            data.data(),                           // Pointer to memory
            sizeof(float),                         // Size of one scalar
            py::format_descriptor<float>::format(),// Format descriptor
            2,                                     // Number of dimensions (rows, cols)
            { rows, cols * 2 },                    // Buffer dimensions
            { sizeof(float) * cols * 2, sizeof(float) } // Strides (in bytes)
        );
    }
};

// We expose this as a separate submodule or add it to the main module
void init_matrix(py::module_ &m) {
    py::class_<ComplexMatrix>(m, "ComplexMatrix", py::buffer_protocol())
        .def(py::init<int, int>())
        .def("set", &ComplexMatrix::set)
        .def("get", &ComplexMatrix::get)
        .def_property_readonly("rows", &ComplexMatrix::get_rows)
        .def_property_readonly("cols", &ComplexMatrix::get_cols)
        .def("multiply", &ComplexMatrix::multiply)
        .def("tensor", &ComplexMatrix::tensor)
        .def_buffer([](ComplexMatrix &m) -> py::buffer_info {
            return m.get_buffer_info();
        });
}