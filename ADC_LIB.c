
/* UNIVERSIDAD DEL VALLE DE GUATEMALA
 * DEPARTAMENTO DE INGENIERIA ELCTRONICA & MECATRONICA
 * CURSO: ELECTRONICA DIGITAL 2
 * LABORATORIO No.1
 * 
 * File:   ADC_LIB.c
 * Author: Daniel Gonzalez
 *
 * Created on July 25, 2023
 */

//-----------------------------------------------------------------------------------------------------------------------------------------------
// LIBRERIAS
//-----------------------------------------------------------------------------------------------------------------------------------------------

#include <stdint.h>         // AGREGAMOS LIBRERIRAS DE TIPOS DE DATOS ESTANDAR Y OTROS.
#include <pic16f887.h>
#include "ADC_LIB.h"        // AGREGAMOS LA LIBRERIRA DEL ADC.

//-----------------------------------------------------------------------------------------------------------------------------------------------


//***********************************************************************************************************************************************
// Funcion para comenzar conversion ADC
// Seleccion de reloj (FOSC/2, FOSC/8, FOSC/32 Frc), voltajes de referencia. 
// Esta libreria configura las interrupciones del ADC.
// Pagina 99 del manual del PIC.
//***********************************************************************************************************************************************


//______________________________________________________________________________
// FUNCION DE CONFIGURACION INICIAL ¨adc_init()¨ 
//______________________________________________________________________________

// Descripcion de funcion: seleccionar el reloj y voltajes de referencia

// CONFIG adc_cs: Selecciona el reloj con el que se hara la conversion
//********************
// 0 --> FOSC/2 --> 00
// 1 --> FOSC/8 --> 01
// 2 --> FOSC/32--> 10
// 3 --> Frc    --> 11
//********************

// CONFIG vref_plus: Selecciona el voltaje maximo de referencia
//****************
// 0 --> Vdd
// 1 --> Vref+pin
//***************

// CONFIG vref_minus: Selecciona el voltaje minimo de referencia
//****************
// 0 --> VSS
// 1 --> Vref-pin
//***************

// JUSTIFICADO: Por default se configura para justificado a al izquierda.

//------------------------------------------------------------------------------

void adc_init (uint8_t adc_cs, uint8_t vref_plus, uint8_t vref_minus)
{    
    ADCON1bits.ADFM = 0;            // Justificado a la izquierda.
        
//----------------- VOLTAJE DE REFERENCIA MAXIMO (veref_plus) ------------------
    
    switch (vref_plus){
        case 0:
            ADCON1bits.VCFG0 = 0;    // Vdd
            break;
        case 1:
            ADCON1bits.VCFG0 = 1;    // Vref + pin
            break;
        default:
            ADCON1bits.VCFG0 = 0;    // Default Vdd    
    }

//----------------- VOLTAJE DE REFERENCIA MINIMO (veref_minus) -----------------

    switch (vref_minus){
        case 0:
            ADCON1bits.VCFG1 = 0;    // Vss
            break;
        case 1:
            ADCON1bits.VCFG1 = 1;    // Vref-pin
            break;
        default:
            ADCON1bits.VCFG1 = 0;    // Default Vss
    }
      
//------------------------ SELECCION DE RELOJ ADC_cs ---------------------------
    
    switch(adc_cs){
        case 0:
            ADCON0bits.ADCS  = 0b00; // ADC Clock (Fosc/2).
            break;
        case 1:
            ADCON0bits.ADCS  = 0b01; // ADC Clock (Fosc/8).
            break;
        case 2:
            ADCON0bits.ADCS  = 0b10; // ADC Clock (Fosc/32).
            break;
        case 3:
            ADCON0bits.ADCS  = 0b11; // ADC Clock FRC (clock derived from a 
            break;                   // dedicated internal oscillator = 500 kHz max)
        default:
            ADCON0bits.ADCS  = 0b01; // ADC Clock (Fosc/8).        
    }   
}

//______________________________________________________________________________
// FUNCION DE CONFIGURACION INICIAL ¨adc_init()¨ 
//______________________________________________________________________________

/* Descripcion de la funcion: La función adc_start recibe como parámetro el canal 
   que se desea leer, enciende el modulo ADC, activa la interrupcion del modulo 
   ADC, e inicia la conversión. */


//CONFIG channel:
//******************************************************************************
// Los canales corresponden a cada numero es decir si elijo el canal 0, 
// este corresponde al canal analogico AN0, si elijo canal 13 corresponde a AN13
// El canal en al opcion de default es AN0.
//******************************************************************************


void adc_start (uint8_t channel){

//--------------------------- SELECCION DE CANALES -----------------------------
    
    switch(channel){
        case 0:
            ADCON0bits.CHS = 0b0000; // AN0                   
            break;
        case 1:
            ADCON0bits.CHS = 0b0001; // AN1
            break;
        case 2:
            ADCON0bits.CHS = 0b0010; // AN2
            break;
        case 3:
            ADCON0bits.CHS = 0b0011; // AN3
            break;
        case 4:
            ADCON0bits.CHS = 0b0100; // AN4
            break;  
        case 5:
            ADCON0bits.CHS = 0b0101; // AN5
            break;
        case 6:
            ADCON0bits.CHS = 0b0110; // AN6
            break;
        case 7:
            ADCON0bits.CHS = 0b0111; // AN7
            break;
        case 8:
            ADCON0bits.CHS = 0b1000; // AN8
            break;
        case 9:
            ADCON0bits.CHS = 0b1001; // AN9
            break;  
        case 10:
            ADCON0bits.CHS = 0b1010; // AN10
            break;
        case 11:
            ADCON0bits.CHS = 0b1011; // AN11
            break;
        case 12:
            ADCON0bits.CHS = 0b1100; // AN12
            break;
        case 13:
            ADCON0bits.CHS = 0b1101; // AN13
            break;
        default:
            ADCON0bits.CHS = 0b0000; // AN0
            break;
    }

//------------------------ ENCIENDIENDO EL MODULO ADC --------------------------
   
    __delay_us(100);             // Delay de 100uS.
    ADCON0bits.ADON = 1;         // Se enciende el modulo ADC.

//------------------ ACTIVANDO LA INTERRUPCION DEL MODULO ADC ------------------
    
    PIE1bits.ADIE   = 1;         // Activar interrupcion por ADC.
    
//-------------------------- Inicio de conversion ------------------------------
    
    __delay_us(50);              // Delay de 50uS. 
    ADCON0bits.GO   = 1;         // Se inicializa la conversion de valores.    
}

//______________________________________________________________________________
// FUNCION DE LECTURA DEL MODULO ADC ¨adc_reade()¨ 
//______________________________________________________________________________

/* Descripcion de función: devuelve los 8 bits de la conversión, apaga la 
   bandera ADIF y no recibe parámetros. */

uint16_t adc_read (void){
    
    uint8_t val   = 0;             // Creamos una variable.
    val = ADRESH;                // Le asignamos el valor de la conversion.  
  
    PIR1bits.ADIF = 0;           // Bajamos la bandera de interrupcion del ADC.
    return val;                  // Retornamos el valor obtenido.
}

//______________________________________________________________________________
//______________________________________________________________________________

