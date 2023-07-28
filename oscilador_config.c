/* UNIVERSIDAD DEL VALLE DE GUATEMALA
 * DEPARTAMENTO DE INGENIERIA ELCTRONICA & MECATRONICA
 * CURSO: ELECTRONICA DIGITAL 2
 * LABORATORIO No.1
 * 
 * File:   oscilador_config.h
 * Author: Daniel Gonzalez
 *
 * Created on July 23, 2023
 */

//-----------------------------------------------------------------------------------------------------------------------------------------------
// LIBRERIAS
//-----------------------------------------------------------------------------------------------------------------------------------------------

#include <stdint.h>
#include <pic16f887.h>
#include "oscilador_config.h"

//______________________________________________________________________________
// FUNCION DE SELECCION DE FRECUENCIA E INICIALIZACION DEL OSC �int_osc_MHz()� 
//______________________________________________________________________________

// Parametros: Opcion de frec a usar.


//------------------------ OPCIONES DE CONFIGURACION ---------------------------
/*
 El parametro "frec" realiza la configuracion del reloj interno que son:
  
  0 ----> 1MHz
  1 ----> 2MHz 
  2 ----> 4MHz
  3 ----> 8MHz
  
 */
//------------------------------------------------------------------------------
void int_osc_MHz (uint8_t frec)
{
    switch (frec){
        case 0:
            OSCCONbits.IRCF0 = 0;   // 1 MHz
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 1;
            break;
        case 1:
            OSCCONbits.IRCF0 = 1;   // 2 MHz
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 1;
            break;
        case 2:
            OSCCONbits.IRCF0 = 0;   // 4 MHz
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 1;
            break;
        case 3:
            OSCCONbits.IRCF0 = 1;   // 8 MHz
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 1;
            break;
        default:
            OSCCONbits.IRCF0 = 0;   // 2 MHz
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 1;
    
    }
    OSCCONbits.SCS = 1;             // Se activa reloj interno.

}

//______________________________________________________________________________
//______________________________________________________________________________
