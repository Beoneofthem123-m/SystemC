#include <systemc.h>
#include <vector>
#include "stim.h"
#include "exor.h"
#include "mon.h"

int sc_main(int argc, char** argv)
{
    int num_inputs = 3;  // ⚡ Cambia este valor si el enunciado lo indica (ej. 2, 4, 5 entradas)

    std::vector<sc_signal<bool>> signals(num_inputs);
    sc_signal<bool> sigZ;  // Salida XOR
    sc_clock Clk("Clk", 10, SC_NS);  // Clock de 10ns

    // Generador de estímulos
    stim Stim1("Stimulus");
    Stim1.set_inputs(num_inputs);
    for (int i = 0; i < num_inputs; ++i) {
        Stim1.inputs[i](signals[i]);
    }

    // Módulo XOR generalizado
    exor_gate DUT("exor");
    DUT.configure(num_inputs);
    for (int i = 0; i < num_inputs; ++i) {
        DUT.inputs[i](signals[i]);
    }
    DUT.Z(sigZ);

    // Monitor para verificar la salida
    Monitor mon("Monitor");
    for (int i = 0; i < num_inputs; ++i) {
        mon.inputs[i](signals[i]);
    }
    mon.Z(sigZ);

    // Simulación
    sc_start();  

    return 0;
}
