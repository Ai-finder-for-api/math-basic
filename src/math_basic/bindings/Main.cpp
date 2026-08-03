#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_vec(py::module_ &m);
void init_matrix(py::module_ &m);
void init_quantum(py::module_ &m);

PYBIND11_MODULE(qmath, m) {
    m.doc() = "Deep-architecture SIMD math library";
    
    init_vec(m);
    init_matrix(m);
    init_quantum(m);
}
