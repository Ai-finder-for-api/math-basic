#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "core/NdArray.hpp"
#include "core/UFunc.hpp"

namespace py = pybind11;
using namespace qmath;

void init_array(py::module_ &m) {
    py::class_<NdArray<float>>(m, "NdArray", py::buffer_protocol())
        .def(py::init<>())
        .def(py::init<const std::vector<float>&>())
        .def(py::init<const std::vector<size_t>&>())
        .def_buffer([](NdArray<float> &a) -> py::buffer_info {
            return a.get_buffer_info();
        })
        .def_property_readonly("shape", &NdArray<float>::shape)
        .def_property_readonly("strides", &NdArray<float>::strides)
        .def_property_readonly("ndim", &NdArray<float>::ndim)
        .def_property_readonly("size", &NdArray<float>::size)
        .def("__add__", [](const NdArray<float>& a, const NdArray<float>& b) { return a + b; }, py::is_operator())
        .def("__mul__", [](const NdArray<float>& a, const NdArray<float>& b) { return a * b; }, py::is_operator())
        .def("__add__", [](const NdArray<float>& a, float s) { return a + s; }, py::is_operator())
        .def("__mul__", [](const NdArray<float>& a, float s) { return a * s; }, py::is_operator())
        .def("reshape", [](const NdArray<float>& a, std::vector<size_t> new_shape) { return a.reshape(new_shape); })
        .def("transpose", &NdArray<float>::transpose)
        .def("matmul", &NdArray<float>::matmul)
        .def("sum", &NdArray<float>::sum)
        .def("max", &NdArray<float>::max)
        .def("min", &NdArray<float>::min)
        // Fix: Class methods must take 'self' as the first argument in the lambda
        .def("mean", [](const NdArray<float>& self) { return mean<float>(self); })
        .def("std", [](const NdArray<float>& self) { return std_dev<float>(self); })
        .def("argmax", [](const NdArray<float>& self) { return argmax<float>(self); })
        .def("sin", [](const NdArray<float>& self) { return sin<float>(self); })
        .def("cos", [](const NdArray<float>& self) { return cos<float>(self); })
        .def("exp", [](const NdArray<float>& self) { return exp<float>(self); })
        .def("sqrt", [](const NdArray<float>& self) { return sqrt<float>(self); })
        .def("abs", [](const NdArray<float>& self) { return abs<float>(self); })
        .def("__getitem__", [](const NdArray<float>& a, size_t i) {
            if (i >= a.size()) throw py::index_error();
            return a[i];
        })
        .def("__setitem__", [](NdArray<float>& a, size_t i, float val) {
            if (i >= a.size()) throw py::index_error();
            a[i] = val;
        });
        
    // Free functions for module level
    m.def("sin", [](const NdArray<float>& arr){ return sin<float>(arr); }, py::arg("arr"));
    m.def("cos", [](const NdArray<float>& arr){ return cos<float>(arr); }, py::arg("arr"));
    m.def("exp", [](const NdArray<float>& arr){ return exp<float>(arr); }, py::arg("arr"));
    m.def("sqrt", [](const NdArray<float>& arr){ return sqrt<float>(arr); }, py::arg("arr"));
    m.def("abs", [](const NdArray<float>& arr){ return abs<float>(arr); }, py::arg("arr"));
    m.def("mean", [](const NdArray<float>& arr){ return mean<float>(arr); }, py::arg("arr"));
    m.def("std", [](const NdArray<float>& arr){ return std_dev<float>(arr); }, py::arg("arr"));
    m.def("argmax", [](const NdArray<float>& arr){ return argmax<float>(arr); }, py::arg("arr"));
}
