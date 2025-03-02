#ifndef STIMULUS_H
#define STIMULUS_H

#include <systemc.h>
#include <sstream>

/**
 * @file stim.h
 * @brief Módulo generador de estímulos para SystemC.
 *
 * ## ¿Qué hace este script?
 * - Genera estímulos para un conjunto de señales de salida utilizando un SC_THREAD.
 * - En este ejemplo, se generan todas las combinaciones binarias posibles para el número
 *   de señales configurado, con un retardo fijo entre cada cambio.
 *
 * ## ¿Dónde se debe ejecutar?
 * - Este módulo se instancia en el testbench (por ejemplo, en `exor_main.cpp`).
 * - Se utiliza para alimentar señales de entrada en otros módulos (como NAND o XOR).
 *
 * ## ¿En qué orden se ejecuta?
 * 1. Al iniciar la simulación, el SC_THREAD `StimGen` se ejecuta.
 * 2. Se generan todas las combinaciones binarias de las señales, esperando 10 ns entre cada cambio.
 * 3. Al finalizar, se detiene la simulación con `sc_stop()`.
 *
 * ## Adaptabilidad
 * - Para cambiar el número de señales, solo se debe modificar el parámetro `num_outputs` al
 *   instanciar el módulo en `sc_main()`.
 * - El módulo generará automáticamente 2^(num_outputs) combinaciones de señales.
 */

SC_MODULE(stim)
{
public:
    // Utilizamos sc_vector para manejar un número variable de salidas.
    sc_vector<sc_out<bool>> outputs;

    // Constructor: se especifica el número de salidas (por defecto 2)
    SC_HAS_PROCESS(stim);
    stim(sc_module_name name, int num_outputs = 2)
    : sc_module(name)
    , outputs("outputs", num_outputs)
    {
         SC_THREAD(StimGen);
    }

private:
    // Función de generación de estímulos: genera todas las combinaciones binarias
    void StimGen()
    {
         // Espera inicial para sincronización
         wait(SC_ZERO_TIME);
         
         int num_signals = outputs.size();
         int total_combinaciones = 1 << num_signals;  // 2^num_signals combinaciones

         // Itera a través de todas las combinaciones binarias
         for (int i = 0; i < total_combinaciones; i++) {
             // Para cada salida, asigna el bit correspondiente de la combinación
             for (int j = 0; j < num_signals; j++) {
                 bool valor = (i >> j) & 1;
                 outputs[j].write(valor);
             }
             // Espera 10 ns entre cada combinación
             wait(10, SC_NS);
         }

         // Opcional: vuelve todas las señales a false y espera antes de detener la simulación
         for (int j = 0; j < num_signals; j++) {
             outputs[j].write(false);
         }
         wait(10, SC_NS);
         sc_stop();
    }
};

#endif // STIMULUS_H
