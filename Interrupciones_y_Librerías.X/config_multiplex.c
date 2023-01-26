/*
 * File:   config_multiplex.c
 * Author: byron
 *
 * Created on January 19, 2023, 10:41 PM
 */


#include <xc.h>
#include "config_multiplex.h"
#define _XTAL_FREQ 4000000

void setupMultiplex (void){
    
    int unidades;
    int decenas;
    int numeros[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};
    
    unidades = ADRESH%10;
    decenas = ADRESH/10;
    
    PORTC = numeros[unidades];  //PORTC = unidades del ADRESH
    PORTEbits.RE0 = 1;          //Encendemos el display de unidades
    __delay_ms(1);
    PORTEbits.RE0 = 0;          //Apagamos el display de unidades
    
    PORTC = numeros[decenas];   //PORTC = decenas del ADRESH
    PORTEbits.RE1 = 1;          //Encendemos el display de decenas
    __delay_ms(1);
    PORTEbits.RE1 = 0;          //Apagamos el display de decenas
    
}