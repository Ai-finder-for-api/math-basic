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
        // Fix: Must be gate * state (2x2 * 2x1 = 2x1), not state * gate
        state = gate.multiply(state);
    }

    ComplexMatrix& get_state() { return state; }
};
