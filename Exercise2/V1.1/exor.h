#ifndef EXOR_H
#define EXOR_H

#include <systemc.h>
#include <vector>
#include "nand.h"

SC_MODULE(exor_gate) {
public:
    std::vector<sc_in<bool>> inputs;  // Permite cualquier número de entradas
    sc_out<bool> Z;  // Salida del XOR

    std::vector<sc_signal<bool>> signals;  // Señales auxiliares para los NANDs
    std::vector<nand_gate*> nand_gates;  // Módulos NAND dinámicos

    int num_inputs;  // Número de entradas configurables

    // Constructor configurable
    SC_CTOR(exor_gate) {}

    // Método para inicializar la estructura según el número de entradas
    void configure(int n) {
        num_inputs = n;
        inputs.resize(n);
        signals.resize(n-1);
        nand_gates.resize(n-1);

        for (int i = 0; i < n-1; ++i) {
            nand_gates[i] = new nand_gate(("NAND" + std::to_string(i)).c_str());
            nand_gates[i]->set_inputs(2);
            
            if (i == 0) {
                nand_gates[i]->inputs[0](inputs[0]);
                nand_gates[i]->inputs[1](inputs[1]);
            } else {
                nand_gates[i]->inputs[0](signals[i-1]);
                nand_gates[i]->inputs[1](inputs[i+1]);
            }
            nand_gates[i]->Z(signals[i]);
        }

        nand_gates[n-2]->Z(Z);  // Última conexión al resultado
    }
};

#endif
