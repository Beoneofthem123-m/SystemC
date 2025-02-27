#ifndef NAND_H
#define NAND_H

/**
 * @file nand.h
 * @brief Módulo NAND para SystemC.
 *
 * ## 📌 ¿Qué hace este script?
 * - Implementa la compuerta NAND, pudiendo manejar 2 o más entradas.
 * - Salida = NOT (A1 AND A2 AND ... AND An)
 *
 * ## 📌 ¿Dónde se utiliza?
 * - Se instancia dentro de otros módulos (por ejemplo, `exor.h`) que necesiten la funcionalidad de NAND.
 * - También puede ser usado directamente en un archivo main de prueba (e.g., `nand_main.cpp`).
 *
 * ## 📌 ¿En qué orden se ejecuta?
 * 1️⃣ El generador de estímulos (`stim`) establece valores de entrada.  
 * 2️⃣ El módulo NAND procesa las entradas y produce la salida.  
 * 3️⃣ El monitor (`mon`) observa y muestra los resultados.  
 *
 * 🚀 **Uso en el examen:** Si piden más o menos entradas en la NAND, simplemente ajusta el número de entradas en tu `sc_main.cpp` (o en donde instancies este módulo).
 */

#include <systemc.h>
#include <vector>

SC_MODULE(nand_gate) {
public:
    // Entradas de la compuerta NAND, flexibles en número
    std::vector<sc_in<bool>> inputs;
    // Salida única de la compuerta NAND
    sc_out<bool> Z;

    // Constructor
    SC_CTOR(nand_gate) {
        // Proceso de SystemC que se activa cada vez que cambian las entradas
        SC_METHOD(do_nand);
        // No se sabe aún cuántas entradas tendrá, se configuran más tarde
        // con set_inputs(int n).
    }

    // Método para configurar dinámicamente cuántas entradas quieres
    void set_inputs(int n) {
        inputs.resize(n);
        for (auto &inp : inputs) {
            sensitive << inp;  // Añadimos sensibilidad a cada entrada
        }
    }

private:
    // Lógica NAND: Salida = NOT(AND de todas las entradas)
    void do_nand() {
        bool result = true;
        for (auto &inp : inputs) {
            result &= inp.read();
        }
        Z.write(!result);
    }
};

#endif // NAND_H
