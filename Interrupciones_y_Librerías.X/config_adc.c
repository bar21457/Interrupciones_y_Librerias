/*
 * File:   config_adc.c
 * Author: byron
 *
 * Created on January 19, 2023, 10:28 PM
 */

#include <xc.h>
#include "config_adc.h"

#define _XTAL_FREQ 4000000

void setupADC (void){
    
    //Paso 1: Selección del puerto de entrada
    
    TRISAbits.TRISA0 = 1;       //Configuración del RBA0 como input
    ANSELbits.ANS0 = 1;         //Configuración del pin RBA0 como análogo (AN0)
    
    //Paso 2: Configuración del módulo ADC
    
    ADCON0bits.ADCS0 = 1;
    ADCON0bits.ADCS1 = 0;       //Fosc/8
    
    ADCON1bits.VCFG0 = 0;       //Ref VDD
    ADCON1bits.VCFG1 = 0;       //Ref VSS
    
    ADCON1bits.ADFM = 0;        //Justificado hacia la izquierda
    
    ADCON0bits.CHS0 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS3 = 0;        //Selección del canal análogo AN0
    
    ADCON0bits.ADON = 1;        //Habilitamos el ADC
    
    __delay_us(100);            //Delay para adquirir la lectura
}