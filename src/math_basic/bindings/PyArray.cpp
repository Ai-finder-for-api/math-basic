#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "../core/NdArray.hpp"
#include "../core/UFunc.hpp"

namespace py = pybind11;
using namespace qmath;

void init_array(py::module_ &m) {
    // Bind NdArray<float> (primary type for NumPy interop)
    py::class_<NdArray<float>>(m, "NdArray", py::buffer_protocol())
        .def(py::init<>())
        .def(py::init<const std::vector<float>&>())
        .def(py::init<const std::vector<size_t>&>())
        
        // Buffer protocol for zero-copy memory sharing with Python/NumPy
        .def_buffer([](NdArray<float> &a) -> py::buffer_info {
            return a.get_buffer_info();
        })

        // Properties
        .def_property_readonly("shape", &NdArray<float>::shape)
        .def_property_readonly("strides", &NdArray<float>::strides)
        .def_property_readonly("ndim", &NdArray<float>::ndim)
        .def_property_readonly("size", &NdArray<float>::size)

        // Math Operators
        .def("__add__", &NdArray<float>::operator+, py::is_operator())
        .def("__mul__", &NdArray<float>::operator*, py::is_operator())
        .def("__add__", [](const NdArray<float>& a, float s) { return a + s; }, py::is_operator())
        .def("__mul__", [](const NdArray<float>& a, float s) { return a * s; }, py::is_operator())

        // Array Manipulation
        .def("reshape", [](const NdArray<float>& a, std::vector<size_t> new_shape) { return a.reshape(new_shape); })
        .def("transpose", &NdArray<float>::transpose)
        
        // Linear Algebra
        .def("matmul", &NdArray<float>::matmul)
        .def("sum", &NdArray<float>::sum)
        .def("max", &NdArray<float>::max)
        .def("min", &NdArray<float>::min)
        .def("mean", &mean<float>)
        .def("std", &std_dev<float>)
        .def("argmax", &argmax<float>)
        .def("argmin", &argmin<float>)

        // UFuncs
        .def("sin", [](const NdArray<float>& a) { return sin(a); })
        .def("cos", [](const NdArray<float>& a) { return cos(a); })
        .def("exp", [](const NdArray<float>& a) { return exp(a); })
        .def("sqrt", [](const NdArray<float>& a) { return sqrt(a); })
        .def("abs", [](const NdArray<float>& a) { return abs(a); })
        .def("square", [](const NdArray<float>& a) { return square(a); })
        .def("pow", [](const NdArray<float>& a, float p) { return pow(a, p); })

        // Item access
        .def("__getitem__", [](const NdArray<float>& a, size_t i) {
            if (i >= a.size()) throw py::index_error();
            return a[i];
        })
        .def("__setitem__", [](NdArray<float>& a, size_t i, float val) {
            if (i >= a.size()) throw py::index_error();
            a[i] = val;
        });
        
    // Module-level UFuncs
    m.def("sin", &sin<float>, py::arg("arr"));
    m.def("cos", &cos<float>, py::arg("arr"));
    m.def("exp", &exp<float>, py::arg("arr"));
    m.def("sqrt", &sqrt<float>, py::arg("arr"));
    m.def("abs", &abs<float>, py::arg("arr"));
    m.def("mean", &mean<float>, py::arg("arr"));
    m.def("std", &std_dev<float>, py::arg("arr"));
    m.def("argmax", &argmax<float>, py::arg("arr"));
}
