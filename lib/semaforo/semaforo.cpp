#include <Arduino.h>
#include <semaforo.h>
#include <uart.h>

float tmps[] = {2,1,0.5};//Ajustes de velocidad
float velocidad = 0; 
//Funcion de retardo no bloqueante
bool esperar(int16_t delay_1){
    //Bandera que cambia su estado cuando el tiempo fue cumplido
    static bool es = true;
    //Almacena tiempo anterior
    static unsigned long time_pass = 0;
    //Almacena el tiempo presente
    static unsigned long time_pres = 0;
    //Salida 
    bool res = false; 
    if (es == true){es = false; time_pass = millis();}
    time_pres = millis();
    //Condicional de comparacion
    if (time_pres-time_pass >delay_1){res = true; es = true;} 
    else {res = false; es=false;}
    return res; 
}

//Iniciar la máquina de estados
void inicializar_semaforo(bool pul1, bool pul2, int16_t LEDV, int16_t LEDA, int16_t LEDR){
    //Inicializacion de variables
    static int16_t pos_tmps = 0;
    static estados state1 = verde;
    struct_var data;
    //Vector de LEDs
    data = {pul1,LEDV,LEDA,LEDR,state1};
    //Cambio de velocidad
    if (pul2) pos_tmps=(pos_tmps+1)%3;
    velocidad = tmps[pos_tmps];
    state1 = actualizar_semaforo(data);
}
//Actualizar el estado de la máquina
estados actualizar_semaforo(struct_var var_struct){
    estados es1; //Variable de estado
    switch (var_struct.es){
    case verde:
    //Encender el LED verde durante 3 s
        if (var_struct.pul1) es1 = no_amarillo; 
        else{
            if (esperar((int16_t)(3000*velocidad))) es1 = verde_amarillo; 
            else es1 = verde;
        } 
        digitalWrite(var_struct.LEDV,HIGH);
        digitalWrite(var_struct.LEDA,LOW);
        digitalWrite(var_struct.LEDR,LOW);
        imprimir(verde_1,velocidad);
        break;
    case verde_amarillo:
    //Encender el LEDs verde y amarillo durante 500 ms
        if (var_struct.pul1) es1 = no_amarillo; 
        else{
            if (esperar((int16_t)(500*velocidad))) es1 = rojo; 
            else es1 = verde_amarillo;
        } 
        digitalWrite(var_struct.LEDV,HIGH);
        digitalWrite(var_struct.LEDA,HIGH);
        digitalWrite(var_struct.LEDR,LOW);
        imprimir(verde_amarillo_1,velocidad);
        break;
    case rojo:
    //Encender el LED rojo durante 2 segundos
        if (var_struct.pul1) es1 = no_amarillo; 
        else{
            if (esperar((int16_t)(2000*velocidad))) es1 = verde; 
            else es1 = rojo;
        } 
        digitalWrite(var_struct.LEDV,LOW);
        digitalWrite(var_struct.LEDA,LOW);
        digitalWrite(var_struct.LEDR,HIGH);
        imprimir(rojo_1,velocidad);
        break;
    case no_amarillo:
    //Led amarillo intermitente cada 500 ms
        if (var_struct.pul1) es1 = alarma_rojo; 
        else{
            if (esperar((int16_t)(500*velocidad))) es1 = no_cero; 
            else es1 = no_amarillo;
        } 
        digitalWrite(var_struct.LEDV,LOW);
        digitalWrite(var_struct.LEDA,HIGH);
        digitalWrite(var_struct.LEDR,LOW);
        imprimir(no_amarillo_1,velocidad);
        break;
    case no_cero:
    //Apaga todos los Led cuando esta en estado no
        if (var_struct.pul1) es1 = alarma_rojo; 
        else{
            if (esperar((int16_t)(500*velocidad))) es1 = no_amarillo; 
            else es1 = no_cero;
        } 
        digitalWrite(var_struct.LEDV,LOW);
        digitalWrite(var_struct.LEDA,LOW);
        digitalWrite(var_struct.LEDR,LOW);
        imprimir(no_cero_1,velocidad);
        break;
    case alarma_rojo: 
    //Estado de alarma cambia al siguiente estado en caso de cumplir la condicion
        if (var_struct.pul1) es1 = verde; 
        else{
            if (esperar((int16_t)(500*velocidad))) es1 = alarma_cero; 
            else es1 = alarma_rojo;
        } 
        digitalWrite(var_struct.LEDV,LOW);
        digitalWrite(var_struct.LEDA,LOW);
        digitalWrite(var_struct.LEDR,HIGH);
        imprimir(alarma_rojo_1,velocidad);
        break;
    case alarma_cero:
    //Apaga todos los Led cuando estra en estado de alarma
        if (var_struct.pul1) es1 = verde; 
        else{
            if (esperar((int16_t)(500*velocidad))) es1 = alarma_rojo; 
            else es1 = alarma_cero;
        } 
        digitalWrite(var_struct.LEDV,LOW);
        digitalWrite(var_struct.LEDA,LOW);
        digitalWrite(var_struct.LEDR,LOW);
        imprimir(alarma_cero_1,velocidad);
        break;
    default:
    //Valores por defecto en caso de falla
        es1 = verde;
        digitalWrite(var_struct.LEDV,HIGH);
        digitalWrite(var_struct.LEDA,LOW);
        digitalWrite(var_struct.LEDR,LOW);
        break;
    }
    return es1;
}