/**
 * @file nand_main.cpp
 * @brief Ejemplo de testbench para el módulo NAND en SystemC.
 *
 * ## ¿Qué hace este archivo?
 * - Define un módulo de nivel superior (`toplevel`) que instancia un `nand_gate`
 *   con múltiples entradas, usando `sc_vector<sc_signal<bool>>` para manejarlas.
 * - Muestra cómo un proceso (SC_METHOD) reacciona a los cambios en las señales
 *   y cómo se pueden forzar distintas combinaciones de entradas.
 *
 * ## ¿Dónde se ejecuta y en qué orden?
 * 1. `sc_main` crea la instancia del módulo `toplevel`.
 * 2. El constructor de `toplevel` inicializa las señales y vincula las entradas
 *    al módulo NAND.
 * 3. El proceso `process` (SC_METHOD) se activa al inicio y cada vez que cambian
 *    las señales en su lista de sensibilidad (todas las entradas y la salida).
 * 4. Se imprimen por consola los valores de entradas y salida en cada activación.
 *
 * ## ¿Cómo adaptarlo a más entradas o salidas?
 * - Para más entradas: solo cambia `num_inputs` en el constructor de `toplevel`.
 * - Para múltiples salidas (o varios NAND):
 *   - Declara más instancias de `nand_gate` y señales de salida (ej. `output2`).
 *   - Conecta cada NAND a las señales que correspondan.
 * - Si el examen pide otros escenarios, ajusta este módulo en vez de reescribir
 *   todo el testbench.
 */

#include <systemc.h>
#include "nand.h"  // Módulo NAND con std::vector<sc_in<bool>> y método set_inputs()

SC_MODULE(toplevel) {
public:
    // Número de entradas para la compuerta NAND
    static const int num_inputs = 3;  // ⚡ Ajusta este valor según lo que pida el examen

    // Vector de señales para las entradas
    sc_vector<sc_signal<bool>> inputs;

    // Señal de salida única (podrías añadir más si quieres más salidas)
    sc_signal<bool> output;

    // Instancia del módulo NAND
    nand_gate n1;

    // Contador para saber cuántas veces se activó el proceso
    unsigned int cnt;

    SC_CTOR(toplevel)
    : inputs("inputs", num_inputs)  // Inicializa vector de señales
    , n1("n1")
    , cnt(0)
    {
        // 1. Configuramos el NAND para tener 'num_inputs' entradas
        n1.set_inputs(num_inputs);

        // 2. Vinculamos cada entrada del NAND a las señales 'inputs'
        for (int i = 0; i < num_inputs; ++i) {
            n1.inputs[i](inputs[i]);
        }
        // 3. Vinculamos la salida del NAND a 'output'
        n1.Z(output);

        // 4. Definimos el proceso 'process' sensible a todas las entradas y la salida
        SC_METHOD(process);
        for (int i = 0; i < num_inputs; ++i) {
            sensitive << inputs[i];
        }
        sensitive << output;
    }

private:
    // Método que se ejecuta al inicio y cada vez que cambian las señales sensibles
    void process() {
        cnt++;

        // Ejemplo: Fuerza todas las entradas a false
        for (int i = 0; i < num_inputs; ++i) {
            inputs[i].write(false);
        }

        // Imprime el estado actual (después de ponerlas en false)
        std::cout << "[Process Trigger #" << cnt << "] t=" << sc_time_stamp()
                  << " Δ=" << sc_delta_count() << " | Inputs: ";
        for (int i = 0; i < num_inputs; ++i) {
            std::cout << (inputs[i].read() ? "1 " : "0 ");
        }
        std::cout << "-> Output: " << (output.read() ? "1" : "0") << std::endl;

        // Ahora, pone todas las entradas a true
        for (int i = 0; i < num_inputs; ++i) {
            inputs[i].write(true);
        }

        // Imprime el nuevo estado (después de ponerlas en true)
        std::cout << "[Process Trigger #" << cnt << "] t=" << sc_time_stamp()
                  << " Δ=" << sc_delta_count() << " | Inputs: ";
        for (int i = 0; i < num_inputs; ++i) {
            std::cout << (inputs[i].read() ? "1 " : "0 ");
        }
        std::cout << "-> Output: " << (output.read() ? "1" : "0") << std::endl;

        std::cout << std::endl;
    }
};

int sc_main(int argc, char** argv) {
    // Instanciamos el módulo de nivel superior
    toplevel top("toplevel");

    // Iniciamos la simulación
    sc_start();

    return 0;
}
