#pragma once
#include "Matrix.hpp"

class QuantumState {
private:
    ComplexMatrix state;

public:
    QuantumState(int qubits) : state(1 << qubits, 1) {
        state.set(0, 0, 1.0f, 0.0f);
    }

    void apply_gate(const ComplexMatrix& gate, int target_qubit, int num_qubits) {
        state = state.multiply(gate);
    }

    ComplexMatrix& get_state() { return state; }
};
