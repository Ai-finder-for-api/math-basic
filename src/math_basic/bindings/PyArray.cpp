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
        .def("__getitem__", [](const NdArray<float>& a, size_t i) {
            if (i >= a.size()) throw py::index_error();
            return a[i];
        })
        .def("__setitem__", [](NdArray<float>& a, size_t i, float val) {
            if (i >= a.size()) throw py::index_error();
            a[i] = val;
        });
}
