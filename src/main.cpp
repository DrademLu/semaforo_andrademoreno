#include <Arduino.h>
#include <botones.h>
#include <semaforo.h>

// ********************************** Integrantes *****************************************
// Nombres: Luis Andrade, Josué Moreno 
// Fecha: 23-06-2023
// ****************************************************************************************


// ****************************** Inputs/Outputs ******************************************
int16_t pul[2]={3,5};                                 //pulsante{1,2}
int16_t led[3]={9,10,11};                             //led {verde,amarillo,rojo} 
// ****************************************************************************************

// ******************************* Programa Principal *************************************
void setup() {
  pinMode(pul[0],INPUT);
  pinMode(pul[1],INPUT);
  pinMode(led[0],OUTPUT);
  pinMode(led[1],OUTPUT); 
  digitalWrite(led[0],false);
  digitalWrite(led[1],false);
  Serial.begin(115200);
}
void loop() {
  bool* aux = inicializar_MEF(pul[0], pul[1],40);
  inicializar_semaforo(aux[0], aux[1], led[0], led[1], led[2]); 
  free(aux); 
}