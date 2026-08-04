#include <pybind11/pybind11.h>
#include "../math/quantum/Gate.hpp"
#include "../math/quantum/Pauli.hpp"
#include "../math/quantum/Hadamard.hpp"
#include "../math/quantum/CNOT.hpp"
#include "../math/quantum/Tensor.hpp"

namespace py = pybind11;
using namespace qmath;

// Helper to bind generic QState and QGate
void init_quantum(py::module_ &m) {
    // Complex number binding
    py::class_<Complex<float>>(m, "Complex")
        .def(py::init<float, float>())
        .def_readwrite("real", &Complex<float>::real)
        .def_readwrite("imag", &Complex<float>::imag)
        .def("__mul__", &Complex<float>::operator*, py::is_operator());

    // 2x2 Gate and State
    py::class_<QGate<2, float>>(m, "QGate2")
        .def(py::init<>())
        .def("get", &QGate<2, float>::get)
        .def("set", &QGate<2, float>::set);

    py::class_<QState<2, float>>(m, "QState2")
        .def(py::init<>())
        .def("apply", &QState<2, float>::apply)
        .def("get", &QState<2, float>::get);

    // 4x4 Gate and State (2 qubits)
    py::class_<QGate<4, float>>(m, "QGate4")
        .def(py::init<>())
        .def("get", &QGate<4, float>::get)
        .def("set", &QGate<4, float>::set)
        .def("apply", &QGate<4, float>::apply);

    py::class_<QState<4, float>>(m, "QState4")
        .def(py::init<>())
        .def("apply", &QState<4, float>::apply)
        .def("get", &QState<4, float>::get);

    // Quantum Gate Factories
    m.def("pauli_x", &make_pauli_x<float>);
    m.def("pauli_y", &make_pauli_y<float>);
    m.def("pauli_z", &make_pauli_z<float>);
    m.def("hadamard", &make_hadamard<float>);
    m.def("cnot", &make_cnot<float>);
    
    // Tensor Product
    m.def("tensor_product", &tensor_product<2, 2, float>);
}
