#include <Arduino.h>
#include <botones.h>
#include <uart.h>
// Variables Globales 
bool flanco_pul1 = false;                                //Flanco de bajada B1  
bool flanco_pul2 = false;                                //Flanco de bajada B2


bool buttonPressed(int16_t tecla){ 
    static bool prev_state = true; 
    bool act_state = digitalRead(tecla);                
    bool result = false;
    //c
    if (prev_state == true && act_state == false){      
        result = true;                                  //Detectar flanco de bajada del pulsante
    }
    else result = false;                               
    prev_state = act_state;                          
    return result; 
}

bool buttonReleased(int16_t tecla){
    static bool prev_state = true;                      
    bool act_state = digitalRead(tecla);
    bool result = true;
    //Comparacion del estado lógico actual y anterior
    if (prev_state == false && act_state == true){
        result = true;                                 //Detectar flanco de subida del pulsante
    }
    else result = false;
    prev_state = act_state; 
    return result; 
}

bool* inicializar_MEF(int16_t pul1, int16_t pul2, int16_t delay2){
    static var_estado es = BU1_BU2; 
    struct_actualizar datos;                            
    bool* vector = (bool*)malloc(2*sizeof(bool));       //Reserva dinámica de memoria
    datos = {pul1,pul2,delay2,es};                        
    es = actualizar_MEF(datos);                         //Actualizar estado de la maquina
    
    //Devolver el resultade 
    if (flanco_pul1) vector[0] = flanco_pul1;
    else vector[0] = false;

    if (flanco_pul2) vector[1] = flanco_pul2; 
    else vector[1] = false;

    flanco_pul1 = false; flanco_pul2 = false;
    return vector;
}

var_estado actualizar_MEF(struct_actualizar var_struct){
    static int16_t time_pass = 0;
    static int16_t time_present = 0;
    static bool es_time = false; 
    static bool es_pul1 = true; 
    static bool es_pul2 = true;

    switch (var_struct.es1){
        case BU1_BU2:
            imprimir(BU1_BU2_1,0); 
            if (buttonPressed(var_struct.pul1) || buttonPressed(var_struct.pul2)){
                es_pul1 = false; 
                es_pul2 = false;
                return rebounds;
            }
            else {
                es_pul1 = true; 
                es_pul2 = true; 
                return BU1_BU2; 
            }
            break;
        case BU1_BD2:
            imprimir(BU1_BD2_1,0); 
            if (buttonPressed(var_struct.pul1) || buttonReleased(var_struct.pul2)){
                es_pul1 = false; 
                es_pul2 = true;
                return rebounds;
            }
            else {
                es_pul1 = true; 
                es_pul2 = false; 
                return BU1_BD2; 
            }
            break;
        case BD1_BU2:
            imprimir(BD1_BU2_1,0); 
            if (buttonReleased(var_struct.pul1) || buttonPressed(var_struct.pul2)){
                es_pul1 = true; 
                es_pul2 = false;
                return rebounds;
            }
            else {
                es_pul1 = false; 
                es_pul2 = true; 
                return BD1_BU2; 
            }
            break;
        case BD1_BD2:
            imprimir(BD1_BD2_1,0);
            if (buttonReleased(var_struct.pul1) || buttonReleased(var_struct.pul2)){
                es_pul1 = true; 
                es_pul2 = true;
                return rebounds;
            }
            else {
                es_pul1 = false; 
                es_pul2 = false; 
                return BD1_BD2; 
            } 
            break;
        case rebounds:
            imprimir(rebounds_1,0); 
            if (es_time == false) {time_pass = millis(); es_time = true;}
            time_present = millis();
            if (time_present-time_pass > var_struct.delay1){
                es_time = false;
                bool es_act_pul1, es_act_pul2;
            
                if (es_pul1 == digitalRead(var_struct.pul1)) {
                    if (es_pul1 == false) flanco_pul1 = true;
                    es_act_pul1 = es_pul1;
                }
                else es_act_pul1 = !es_pul1;
            
                if (es_pul2 == digitalRead(var_struct.pul2)) {
                    if (es_pul2 == false) flanco_pul2 = true;
                    es_act_pul2 = es_pul2;
                }
                else es_act_pul2 = !es_pul2;

                if (es_act_pul1 == false){
                    if (es_act_pul2 == false) return BD1_BD2;
                    else return BD1_BU2;
                }
                else{
                    if (es_act_pul2 == false) return BU1_BD2;
                    else return BU1_BU2;
                } 
            }
            else return rebounds;
            break;       
        default:
            imprimir(BU1_BU2_1,0); 
            es_pul1 = true;
            es_pul2 = true;
            return BU1_BU2; 
            break;
        }
    }