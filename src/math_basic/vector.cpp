#include "Simd.hpp"

// Forward declaration for the matrix initialization function defined in matrix.cpp
void init_matrix(py::module_ &m);

class Vec4 {
private:
    SIMD_TYPE data;

public:
    Vec4() : data(SIMD_SETZERO) {}
    Vec4(float x, float y, float z, float w) : data(SIMD_SET(w, z, y, x)) {}
    Vec4(SIMD_TYPE v) : data(v) {}

    SIMD_TYPE get_data() const { return data; }

    Vec4 operator+(const Vec4& other) const {
        return Vec4(SIMD_ADD(data, other.data));
    }

    Vec4 operator-(const Vec4& other) const {
        // Cross-platform subtraction
        #if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
            return Vec4(_mm_sub_ps(data, other.data));
        #else
            return Vec4(vsubq_f32(data, other.data));
        #endif
    }

    Vec4 operator*(float scalar) const {
        return Vec4(SIMD_MUL(data, SIMD_SET1(scalar)));
    }

    float dot(const Vec4& other) const {
        return simd_hadd(SIMD_MUL(data, other.data));
    }

    float length_sq() const {
        return simd_hadd(SIMD_MUL(data, data));
    }

    py::buffer_info get_buffer_info() {
        return py::buffer_info(
            &data, sizeof(float), py::format_descriptor<float>::format(),
            1, { 4 }, { sizeof(float) }
        );
    }
};

// Batch processing function
py::list batch_process_vectors(py::list vectors) {
    py::list results;
    for (auto item : vectors) {
        Vec4& v1 = item.cast<Vec4&>();
        Vec4 scaled = v1 * 2.0f;
        results.append(scaled.dot(scaled));
    }
    return results;
}

PYBIND11_MODULE(qmath, m) {
    m.doc() = "Bare-metal SIMD-accelerated math library";

    // Bind Vec4
    py::class_<Vec4>(m, "Vec4", py::buffer_protocol())
        .def(py::init<>())
        .def(py::init<float, float, float, float>())
        .def_buffer([](Vec4 &v) -> py::buffer_info {
            return v.get_buffer_info();
        })
        .def("__add__", &Vec4::operator+, py::is_operator())
        .def("__sub__", &Vec4::operator-, py::is_operator())
        .def("__mul__", &Vec4::operator*, py::is_operator())
        .def("dot", &Vec4::dot)
        .def("length_sq", &Vec4::length_sq);

    // Bind batch function
    m.def("batch_process_vectors", &batch_process_vectors, "Processes a list of vectors in C++");

    // ---------------------------------------------------------
    // TIE-IN: Initialize the Matrix bindings from matrix.cpp
    // ---------------------------------------------------------
    init_matrix(m);
}