#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../core/NdArray.hpp"
#include "../core/UFunc.hpp"

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
        .def("__add__", &NdArray<float>::operator+, py::is_operator())
        .def("__mul__", [](const NdArray<float>& a, const NdArray<float>& b) { return a * b; }, py::is_operator())
        .def("__mul__", [](const NdArray<float>& a, float s) { return a * s; }, py::is_operator())
        .def("__rmul__", [](const NdArray<float>& a, float s) { return a * s; }, py::is_operator())
        .def("__add__", [](const NdArray<float>& a, float s) { return a + s; }, py::is_operator())
        .def("reshape", [](const NdArray<float>& a, std::vector<size_t> new_shape) { return a.reshape(new_shape); })
        .def("transpose", &NdArray<float>::transpose)
        .def("matmul", &NdArray<float>::matmul)
        .def("sum", &NdArray<float>::sum)
        .def("max", &NdArray<float>::max)
        .def("min", &NdArray<float>::min)
        .def("mean", [](const NdArray<float>& a) { return mean<float>(a); })
        .def("std", [](const NdArray<float>& a) { return std_dev<float>(a); })
        .def("argmax", [](const NdArray<float>& a) { return argmax<float>(a); })
        .def("argmin", [](const NdArray<float>& a) { return argmin<float>(a); })
        .def("sin", [](const NdArray<float>& a) { return sin<float>(a); })
        .def("cos", [](const NdArray<float>& a) { return cos<float>(a); })
        .def("exp", [](const NdArray<float>& a) { return exp<float>(a); })
        .def("sqrt", [](const NdArray<float>& a) { return sqrt<float>(a); })
        .def("abs", [](const NdArray<float>& a) { return abs<float>(a); })
        .def("square", [](const NdArray<float>& a) { return square<float>(a); })
        .def("pow", [](const NdArray<float>& a, float p) { return pow<float>(a, p); })
        .def("__getitem__", [](const NdArray<float>& a, size_t i) {
            if (i >= a.size()) throw py::index_error();
            return a[i];
        })
        .def("__setitem__", [](NdArray<float>& a, size_t i, float val) {
            if (i >= a.size()) throw py::index_error();
            a[i] = val;
        });
        
    m.def("sin", [](const NdArray<float>& arr){ return sin<float>(arr); }, py::arg("arr"));
    m.def("cos", [](const NdArray<float>& arr){ return cos<float>(arr); }, py::arg("arr"));
    m.def("exp", [](const NdArray<float>& arr){ return exp<float>(arr); }, py::arg("arr"));
    m.def("sqrt", [](const NdArray<float>& arr){ return sqrt<float>(arr); }, py::arg("arr"));
    m.def("abs", [](const NdArray<float>& arr){ return abs<float>(arr); }, py::arg("arr"));
    m.def("mean", [](const NdArray<float>& arr){ return mean<float>(arr); }, py::arg("arr"));
    m.def("std", [](const NdArray<float>& arr){ return std_dev<float>(arr); }, py::arg("arr"));
    m.def("argmax", [](const NdArray<float>& arr){ return argmax<float>(arr); }, py::arg("arr"));
}
