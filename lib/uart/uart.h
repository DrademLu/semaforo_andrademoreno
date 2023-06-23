#include <Arduino.h>

// posibles estados del semaforo y teclas
typedef enum {
    BU1_BU2_1,
    BU1_BD2_1,
    BD1_BU2_1,
    BD1_BD2_1,
    rebounds_1,
    verde_1, 
    verde_amarillo_1,
    rojo_1,
    no_amarillo_1,
    no_cero_1,
    alarma_rojo_1, 
    alarma_cero_1
} variable_struct;

// muestra estado actual de la maquina de estados
void imprimir(variable_struct variable, float velocidad);

