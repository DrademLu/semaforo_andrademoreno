#include <Arduino.h>
//Variable enum que contienen los posibles estados de la MEF
typedef enum{
    verde, 
    verde_amarillo,
    rojo,
    no_amarillo,
    no_cero,
    alarma_rojo, 
    alarma_cero
}estados;
//Estructura que incluye los Led disponibles y la variable de estado
typedef struct {
    bool pul1; 
    int16_t LEDV;
    int16_t LEDA;
    int16_t LEDR;
    estados es; 
} struct_var;
//Prototipos de funciones
void inicializar_semaforo(bool pul1, bool pul2, int16_t LEDV, int16_t LEDA, int16_t LEDR);
estados actualizar_semaforo(struct_var var_struct);
bool esperar(int16_t delay_1);