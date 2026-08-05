#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../math/quantum/Gate.hpp"
#include "../math/quantum/Pauli.hpp"
#include "../math/quantum/Hadamard.hpp"
#include "../math/quantum/CNOT.hpp"
#include "../math/quantum/Tensor.hpp"
#include "core/ComplexMatrix.hpp"
#include "core/QuantumState.hpp"

namespace py = pybind11;
using namespace qmath;

void init_quantum(py::module_ &m) {
    py::class_<Complex<float>>(m, "Complex")
        .def(py::init<float, float>())
        .def_readwrite("real", &Complex<float>::real)
        .def_readwrite("imag", &Complex<float>::imag)
        .def("__mul__", &Complex<float>::operator*, py::is_operator());

    py::class_<ComplexMatrix>(m, "ComplexMatrix", py::buffer_protocol())
        .def(py::init<int, int>())
        .def("set", &ComplexMatrix::set)
        .def("get", &ComplexMatrix::get)
        .def_property_readonly("rows", &ComplexMatrix::get_rows)
        .def_property_readonly("cols", &ComplexMatrix::get_cols)
        .def("multiply", &ComplexMatrix::multiply)
        .def_buffer([](ComplexMatrix &mat) -> py::buffer_info {
            return mat.get_buffer_info();
        });

    py::class_<QuantumState>(m, "QuantumState")
        .def(py::init<int>())
        .def("apply_gate", &QuantumState::apply_gate)
        .def("get_state", &QuantumState::get_state, py::return_value_policy::reference_internal);

    py::class_<QGate<2, float>>(m, "QGate2")
        .def(py::init<>())
        .def("get", &QGate<2, float>::get)
        .def("set", &QGate<2, float>::set);

    py::class_<QState<2, float>>(m, "QState2")
        .def(py::init<>())
        .def("apply", &QState<2, float>::apply)
        .def("get", &QState<2, float>::get);

    py::class_<QGate<4, float>>(m, "QGate4")
        .def(py::init<>())
        .def("get", &QGate<4, float>::get)
        .def("set", &QGate<4, float>::set)
        .def("apply", &QGate<4, float>::apply);

    py::class_<QState<4, float>>(m, "QState4")
        .def(py::init<>())
        .def("apply", &QState<4, float>::apply)
        .def("get", &QState<4, float>::get);

    m.def("pauli_x", &make_pauli_x<float>);
    m.def("pauli_y", &make_pauli_y<float>);
    m.def("pauli_z", &make_pauli_z<float>);
    m.def("hadamard", &make_hadamard<float>);
    m.def("cnot", &make_cnot<float>);
    m.def("tensor_product", &tensor_product<2, 2, float>);
}
