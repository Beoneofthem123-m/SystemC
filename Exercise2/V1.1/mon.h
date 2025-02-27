#ifndef MONITOR_H
#define MONITOR_H

/**
 * @file mon.h
 * @brief Módulo Monitor para SystemC.
 * 
 * ## 📌 ¿Qué hace este script?
 * - Captura y muestra la evolución de las señales de entrada y salida del sistema.
 * - Se encarga de imprimir el estado de las señales con su marca de tiempo (`sc_time_stamp()`).
 * - Esencial para depuración y validación del circuito.
 * 
 * ## 📌 ¿Dónde se debe ejecutar?
 * - Se instancia y conecta dentro de `exor_main.cpp` junto con el estímulo (`stim.h`).
 * - Necesita estar vinculado al módulo que se desea analizar (ejemplo: `exor_gate`).
 * - Se compila junto con los demás archivos en la ejecución de `sc_main()`.
 * 
 * ## 📌 ¿En qué orden se ejecuta?
 * 1️⃣ Primero, `stim.h` genera los valores de entrada.  
 * 2️⃣ Luego, `exor.h` (o cualquier módulo) procesa las entradas.  
 * 3️⃣ Finalmente, `mon.h` monitorea los cambios en las señales de entrada y salida, mostrando los resultados en consola.  
 * 
 * 🚀 **Uso en el examen:** Si cambian el número de entradas, `mon.h` debe adaptarse para imprimir correctamente las nuevas señales.
 */

#include <iostream>
#include <systemc.h>
#include <vector>

SC_MODULE(Monitor) {
public:
    std::vector<sc_in<bool>> inputs;  // Entradas dinámicas
    sc_in<bool> Z;  // Salida del sistema

    SC_CTOR(Monitor) {
        std::cout << "\nTime\t";
        for (size_t i = 0; i < inputs.size(); ++i) {
            std::cout << "In" << i << "\t";
        }
        std::cout << "Out\n";

        SC_METHOD(monitor);
        for (auto &in : inputs) sensitive << in;
        sensitive << Z;
        dont_initialize();
    }

private:
    void monitor() {
        std::cout << sc_time_stamp() << "\t";
        for (const auto &in : inputs) {
            std::cout << in.read() << "\t";
        }
        std::cout << Z.read() << std::endl;
    }
};

#endif // MONITOR_H
