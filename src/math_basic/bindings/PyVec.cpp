#include <pybind11/pybind11.h>
#include "../core/Vec4.hpp"

namespace py = pybind11;

// The missing batch function!
py::list batch_process_vectors(py::list vectors) {
    py::list results;
    for (auto item : vectors) {
        Vec4& v1 = item.cast<Vec4&>();
        Vec4 scaled = v1 * 2.0f;
        results.append(scaled.dot(scaled));
    }
    return results;
}

void init_vec(py::module_ &m) {
    py::class_<Vec4>(m, "Vec4", py::buffer_protocol())
        .def(py::init<>())
        .def(py::init<float, float, float, float>())
        .def_buffer([](Vec4 &v) -> py::buffer_info { return v.get_buffer_info(); })
        .def("__add__", &Vec4::operator+, py::is_operator())
        .def("__sub__", &Vec4::operator-, py::is_operator())
        .def("__mul__", &Vec4::operator*, py::is_operator())
        .def("dot", &Vec4::dot)
        .def("length_sq", &Vec4::length_sq);

    // Bind it to the module
    m.def("batch_process_vectors", &batch_process_vectors, "Processes a list of vectors in C++");
}
