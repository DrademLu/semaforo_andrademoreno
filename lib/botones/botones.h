#include <Arduino.h>
/*NOTA: Los pulsantes estan en PULL-UP*/

// Estructura enumerada que define los estados de la MEF
typedef enum {
    BU1_BU2,            //B1: ON   B2: ON
    BU1_BD2,            //B1: ON   B2: OFF
    BD1_BU2,            //B1: OFF  B2: ON 
    BD1_BD2,            //B1: OFF  B2: OFF
    rebounds            //Rebotes
}var_estado; 

/*Estructura que engloba los siguientes parámetros
    > PIN B1
    > PIN B2
    > Estado MEF*/
typedef struct{
    int16_t pul1;
    int16_t pul2; 
    int16_t delay1; 
    var_estado es1;
} struct_actualizar;  
// Funcion que Inicia la MEF 
bool* inicializar_MEF(int16_t pul1, int16_t pul2,int16_t delay2);
// Funcion que actualiza la MEF
var_estado actualizar_MEF(struct_actualizar var_struct); 
// Funcion que detecta un flanco de bajada de un pulsante
bool buttonPressed(int16_t tecla); 
// Funcion que detecta un flanco de subida de un pulsante
bool buttonReleased(int16_t tecla); 
