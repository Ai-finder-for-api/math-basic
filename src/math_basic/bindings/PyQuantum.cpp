#include <pybind11/pybind11.h>
#include "../core/Quantum.hpp"

namespace py = pybind11;

void init_quantum(py::module_ &m) {
    py::class_<QuantumState>(m, "QuantumState")
        .def(py::init<int>())
        .def("apply_gate", &QuantumState::apply_gate)
        .def("get_state", &QuantumState::get_state, py::return_value_policy::reference_internal);
}
