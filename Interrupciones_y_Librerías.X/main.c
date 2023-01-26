/*
 * File:   main.c
 * Author: byron
 *
 * Created on October 14, 2022, 6:37 AM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits 
                                            // (INTOSC oscillator: I/O function 
                                            // on RA6/OSC2/CLKOUT pin, I/O 
                                            // function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and 
                                // can be enabled by SWDTEN bit of the WDTCON 
                                // register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR 
                                // pin function is digital input, MCLR 
                                // internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code 
                                // protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code 
                                // protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit 
                                // (Internal/External Switchover mode disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
                                // (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin 
                                // has digital I/O, HV on MCLR must be used for 
                                // programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out 
                                // Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
                                // (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "config_adc.h"
#include "config_multiplex.h"

#define _XTAL_FREQ 4000000

void setup(void);

void __interrupt() ISR (void) {
    
    if (INTCONbits.RBIF == 1) // Interrupción del PORTB
    {
        if(!PORTBbits.RB0)
        {
            __delay_ms(20); //Esperar 20ms
            if(!PORTBbits.RB0) //Si el botón sigue presionado después de 20ms
            {
                PORTD ++;           //Incrementamos en 1 el valor del PORTD   
            }
        }
        else if(!PORTBbits.RB1)
        {
            __delay_ms(20); //Esperar 20ms
            if(!PORTBbits.RB1) //Si el botón sigue presionado después de 20ms
            {
                PORTD --;           //Decrementamos en 1 el valor del PORTD   
            }
        }
        INTCONbits.RBIF = 0; // Bajamos la bandera de interrupción del PORTB
    }
    return;
}

//******************************************************************************
// Código Principal
//******************************************************************************
void main(void) {
    
    setup();
    setupADC();
    
    while(1){
        
        //Lab
        
        ADCON0bits.GO = 1;      //Iniciamos la conversión en el ADC
        while (ADCON0bits.GO == 1){};
        ADIF = 0;               //Bajamos la bandera del ADC
        setupMultiplex();
        //PORTC = ADRESH;         //El PORTC adquiere el valor de la conversión
        __delay_ms(10);
        
        //PostLab
        
        if(ADRESH > PORTD){
            PORTEbits.RE2 = 1;      //Encendemos la alarma
        }
        else
        {
            PORTEbits.RE2 = 0;      //Apagamos la alarma
        }
    }
    return;
}

//******************************************************************************
// Función para configurar GPIOs
//******************************************************************************
void setup (void){
    
    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0b00000011;     //Configuración del PORTB como input
    TRISC = 0;              //Configuración del PORTC como output
    TRISD = 0;              //Configuración del PORTD como output
    TRISE = 0;              //Configuración del PORTE como output
    
    OPTION_REGbits.nRBPU = 0;   //Habilitamos los pull-ups del PORTB
    WPUBbits.WPUB0 = 1;         //Habilitamos el pull-up del RB0
    WPUBbits.WPUB1 = 1;         //Habilitamos el pull-up del RB1
    IOCB = 0b00000011;
    
    PORTB = 0;              //Limpiamos el PORTB
    PORTC = 0;              //Limpiamos el PORTC
    PORTD = 0;              //Limpiamos el PORTD
    PORTE = 0;              //Limpiamos el PORTD
    
    //Configuración del Oscilador Interno a 4MHz
    
    OSCCONbits.IRCF = 0b0110 ;
    OSCCONbits.SCS = 1;
    
    // Configuración del TMR0 y su Interrupción
    
    OPTION_REGbits.T0CS = 0;        // Fosc/4
    OPTION_REGbits.PSA = 0;         // Prescaler para el TMR0
    OPTION_REGbits.PS = 0b011;      // Prescaler 1:16
    
    INTCONbits.GIE = 1;     //Habilitamos las interrupciones globales (GIE)
    INTCONbits.PEIE = 1;    //Habilitamos las interrupción del PEIE
    INTCONbits.RBIE = 1;    //Habilitamos las interrupciones del PORTB (RBIE)
    INTCONbits.RBIF = 0;    //Bajamos la bandera de interrupción del PORTB (RBIF)
}
