/* UNIVERSIDAD DEL VALLE DE GUATEMALA
 * DEPARTAMENTO DE INGENIERIA ELCTRONICA & MECATRONICA
 * CURSO: ELECTRONICA DIGITAL 2
 * LABORATORIO No.2
 * 
 * File:   UASART_LIB.c
 * Daniel Gonzalez 
 *
 * Created on July 27, 2023, 4:28 PM
 */

//-----------------------------------------------------------------------------------------------------------------------------------------------
// LIBRERIAS
//-----------------------------------------------------------------------------------------------------------------------------------------------

#include <stdint.h>
#include <pic16f887.h>
#include "UASART_LIB.h"
#include <stdio.h>

//______________________________________________________________________________
// FUNCION DE SELECCION INICIALIZACION DEL RX 
//______________________________________________________________________________

// Parametros: BAUDRATE que se decea usar para la transmision de datos.


//------------------------ OPCIONES DE CONFIGURACION ---------------------------
/*
 El parametro "Baudrate" puede terner algunos valore comunes como los siguientes:
  
  -> 9600
  -> 14400
  -> 19200
 
 */
//------------------------------------------------------------------------------

void UART_RX_CONFIG(uint16_t BAUDRATE){
    
    unsigned char valor =(unsigned char) ((_XTAL_FREQ / (BAUDRATE * 16))-1);
    TXSTAbits.BRGH = 1;       // Alta velocidad.
    
    if (valor > 255){
        valor =(unsigned char) ((_XTAL_FREQ / (BAUDRATE * 64))-1);
        TXSTAbits.BRGH = 0;   // Baja velocidad.
    }
    
    TRISCbits.TRISC7  = 1;    // RX Como entrada.
    TXSTAbits.SYNC    = 0;    // UART (Modo Asincrono).
    BAUDCTLbits.BRG16 = 0;
    TXSTAbits.TX9     = 0;    // 8 Bits.
    
    
    SPBRG  = 51;             // Asignamos el valor para el baudrate deceado.
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;      // UART On.
    RCSTAbits.CREN = 1;      // Habilitamos la recepción de datos (RX On).
}     

//______________________________________________________________________________
// FUNCION DE SELECCION INICIALIZACION DEL TX 
//______________________________________________________________________________

// Parametros: BAUDRATE que se decea usar para la transmision de datos.


//------------------------ OPCIONES DE CONFIGURACION ---------------------------
/*
 El parametro "Baudrate" puede terner algunos valore comunes como los siguientes:
  
  -> 9600
  -> 14400
  -> 19200
 
 */
//------------------------------------------------------------------------------

void UART_TX_CONFIG(uint16_t BAUDRATE){
    
    unsigned char valor =(unsigned char) ((_XTAL_FREQ / (BAUDRATE * 16))-1);
    TXSTAbits.BRGH = 1;      // Alta velocidad.
    
    if (valor > 255){
        valor =(unsigned char) ((_XTAL_FREQ / (BAUDRATE * 64))-1);
        TXSTAbits.BRGH = 0;  // Baja velocidad.
    }
    
    TRISCbits.TRISC6  = 0;    // RX Como Salida.
    TXSTAbits.SYNC    = 0;    // UART (Modo Asincrono).
    BAUDCTLbits.BRG16 = 0;
    TXSTAbits.TX9     = 0;    // 8 Bits.
    
    
    SPBRG  = valor;          // Asignamos el valor para el baudrate deceado.
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;      // UART On.
    TXSTAbits.TXEN = 1;      // Habilitamos la Transmision de datos (TX On).
}     

//______________________________________________________________________________
// FUNCION DE ENVIO CARACTER POR CARACTER
//______________________________________________________________________________

void UART_WRITE_CHAR(char CHARACTER){
    while(!TXIF);                  // ESPERAMOS A QUE SE ENVIE UN DATO.  
    TXREG = CHARACTER;             // ENVIAMOS UN CARACTER. 

}

//______________________________________________________________________________
// FUNCION PARA SEPARACION DEL MENSAJE A ENVIAR, EN CARACTERES INDIVIDUALES
//______________________________________________________________________________

void UART_WRITE_STRING(char* MENSAJE){
    
    while(*MENSAJE  != '\0'){      // VERIFICAMOS SI HAY UN ESPACIO NULO.
        UART_WRITE_CHAR(*MENSAJE); /* MANDAMOS A LA FUNCION DE ENVIO DE CARACTER,  
                                     EL CARACTER EN LA POSICION DEL VALOR DE LA 
                                     VARIABLE "MENSAJE". */ 
        MENSAJE ++;                // INCREMENTAMOS LA VARIABLE "MENSAJE".
    }
}

//______________________________________________________________________________
// FUNCION BASE PARA UTILIZAR LA FUNCION "printf()"
//______________________________________________________________________________

void putch(char daT){
    UART_WRITE_CHAR(daT);    
}

//______________________________________________________________________________
//______________________________________________________________________________

