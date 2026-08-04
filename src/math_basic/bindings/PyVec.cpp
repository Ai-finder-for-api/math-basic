#include <pybind11/pybind11.h>
#include <pybind11/buffer_info.h>
#include "../math/3d/Vec3.hpp"
#include "../math/3d/Vec4.hpp"

namespace py = pybind11;
using namespace qmath;

// RESTORED: Batch Processing Function for massive speedup
py::list batch_process_vectors(py::list vectors) {
    py::list results;
    for (auto item : vectors) {
        // Cast Python object to our C++ Vec4f
        Vec4f& v1 = item.cast<Vec4f&>();
        
        // Do heavy math: multiply by 2.0, then dot product with itself
        Vec4f scaled = v1 * 2.0f;
        float dot = scaled.dot(scaled);
        
        results.append(dot);
    }
    return results;
}

void init_vec(py::module_ &m) {
    // Bind Vec3f
    py::class_<Vec3f>(m, "Vec3", py::buffer_protocol())
        .def(py::init<>())
        .def(py::init<float, float, float>())
        .def_buffer([](Vec3f &v) -> py::buffer_info { 
            return py::buffer_info(&v, sizeof(float), py::format_descriptor<float>::format(), 1, { 3 }, { sizeof(float) }); 
        })
        .def("__add__", &Vec3f::operator+, py::is_operator())
        .def("__sub__", &Vec3f::operator-, py::is_operator())
        .def("__mul__", &Vec3f::operator*, py::is_operator())
        .def("dot", &Vec3f::dot)
        .def("cross", &Vec3f::cross)
        .def("length", &Vec3f::length)
        .def("normalized", &Vec3f::normalized);

    // Bind Vec4f
    py::class_<Vec4f>(m, "Vec4", py::buffer_protocol())
        .def(py::init<>())
        .def(py::init<float, float, float, float>())
        .def_buffer([](Vec4f &v) -> py::buffer_info { 
            return py::buffer_info(&v, sizeof(float), py::format_descriptor<float>::format(), 1, { 4 }, { sizeof(float) }); 
        })
        .def("__add__", &Vec4f::operator+, py::is_operator())
        .def("__sub__", &Vec4f::operator-, py::is_operator())
        .def("__mul__", &Vec4f::operator*, py::is_operator())
        .def("dot", &Vec4f::dot)
        .def("length", &Vec4f::length)
        .def("normalized", &Vec4f::normalized);

    // RESTORED: Bind batch processing
    m.def("batch_process_vectors", &batch_process_vectors, "Processes a list of Vec4s in C++");
}
