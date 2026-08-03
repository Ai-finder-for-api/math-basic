#include <pybind11/pybind11.h>
#include "../core/Matrix.hpp"

namespace py = pybind11;

void init_matrix(py::module_ &m) {
    py::class_<ComplexMatrix>(m, "ComplexMatrix", py::buffer_protocol())
        .def(py::init<int, int>())
        .def("set", &ComplexMatrix::set)
        .def("get", &ComplexMatrix::get)
        .def_property_readonly("rows", &ComplexMatrix::get_rows)
        .def_property_readonly("cols", &ComplexMatrix::get_cols)
        .def("multiply", &ComplexMatrix::multiply)
        .def_buffer([](ComplexMatrix &m) -> py::buffer_info { return m.get_buffer_info(); });
}
