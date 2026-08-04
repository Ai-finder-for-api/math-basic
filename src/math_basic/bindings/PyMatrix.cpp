#include <pybind11/pybind11.h>
#include "../math/3d/Mat4.hpp"
#include "../math/3d/Mat3.hpp"
#include "../math/3d/Quat.hpp"
#include "../math/linalg/Matrix.hpp"
#include "../math/linalg/LU.hpp"
#include "../math/linalg/Inv.hpp"
#include "../math/linalg/Det.hpp"
#include "../math/linalg/Eigen.hpp"
#include "../math/linalg/Solve.hpp"
#include "../math/linalg/SVD.hpp"

namespace py = pybind11;
using namespace qmath;

void init_matrix(py::module_ &m) {
    py::class_<Mat4f>(m, "Mat4")
        .def(py::init<>())
        .def(py::init<float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float>())
        .def("__mul__", &Mat4f::operator*, py::is_operator())
        .def("transposed", &Mat4f::transposed)
        .def("determinant", &Mat4f::determinant)
        .def_static("identity", &Mat4f::identity)
        .def_static("translation", &Mat4f::translation)
        .def_static("scaling", &Mat4f::scaling)
        .def_static("rotation_x", &Mat4f::rotation_x)
        .def_static("rotation_y", &Mat4f::rotation_y)
        .def_static("rotation_z", &Mat4f::rotation_z)
        .def_static("look_at", &Mat4f::look_at)
        .def_static("perspective", &Mat4f::perspective)
        .def_static("ortho", &Mat4f::ortho);

    py::class_<Mat3f>(m, "Mat3")
        .def(py::init<>())
        .def("__mul__", &Mat3f::operator*, py::is_operator())
        .def("determinant", &Mat3f::determinant)
        .def("inverse", &Mat3f::inverse);

    py::class_<Quatf>(m, "Quat")
        .def(py::init<>())
        .def(py::init<float, float, float, float>())
        .def("__mul__", &Quatf::operator*, py::is_operator())
        .def("normalized", &Quatf::normalized)
        .def("conjugate", &Quatf::conjugate)
        .def("inverse", &Quatf::inverse)
        .def("rotate", &Quatf::rotate)
        .def("to_mat3", &Quatf::to_mat3)
        .def("to_mat4", &Quatf::to_mat4)
        .def_static("from_axis_angle", &Quatf::from_axis_angle)
        .def_static("from_euler", &Quatf::from_euler)
        .def_static("slerp", &Quatf::slerp);

    py::class_<DMatrix<float>>(m, "DMatrix")
        .def(py::init<size_t, size_t>())
        .def("__call__", &DMatrix<float>::operator())
        .def_property_readonly("rows", &DMatrix<float>::rows)
        .def_property_readonly("cols", &DMatrix<float>::cols)
        .def("transposed", &DMatrix<float>::transposed)
        .def("__mul__", &DMatrix<float>::operator*, py::is_operator());

    m.def("lu_decompose", &lu_decompose<float>);
    m.def("solve", &solve<float>);
    m.def("inverse", &inverse<float>);
    m.def("determinant", &determinant<float>);
    m.def("jacobi_eigen", &jacobi_eigen<float>);
    m.def("svd_jacobi", &svd_jacobi<float>);
}
