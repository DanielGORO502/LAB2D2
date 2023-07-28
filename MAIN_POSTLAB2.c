
/* UNIVERSIDAD DEL VALLE DE GUATEMALA
 * DEPARTAMENTO DE INGENIERIA ELCTRONICA & MECATRONICA
 * CURSO: ELECTRONICA DIGITAL 2
 * LABORATORIO No.2
 * 
 * File:   MAIN_POSTLAB2.c
 * Author: Daniel Gonzalez 
 *
 * Created on July 27, 2023, 1:02 AM
 */

//-----------------------------------------------------------------------------------------------------------------------------------------------
// CONFIG1
//-----------------------------------------------------------------------------------------------------------------------------------------------

#pragma config FOSC  = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE  = OFF      // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP    = OFF      // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD   = OFF      // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO  = OFF      // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP   = OFF      // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

//-----------------------------------------------------------------------------------------------------------------------------------------------
// CONFIG2
//-----------------------------------------------------------------------------------------------------------------------------------------------

#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT   = OFF      // Flash Program Memory Self Write Enable bits (Write protection off)

//-----------------------------------------------------------------------------------------------------------------------------------------------
// LIBRERIAS
//-----------------------------------------------------------------------------------------------------------------------------------------------

#include <xc.h>
#include <stdint.h>
#include "oscilador_config.h"   // LIBRERIA PARA CONFIGURACION DEL OSCILADOR.
#include "ADC_LIB.h"            // AGREGAMOS LA LIBRERIRA DEL ADC.
#include "LCD.h"                // AGREGAMOS LA LIBRERIRA DE LA LCD.
#include <stdio.h>              // AGREGAMOS LA LIBRERIRA "stdio" PARA USAR LA FUNCION printf().
#include "UASART_LIB.h"         // AGREGAMOS LA LIBRERIRA DEL UART.

//-----------------------------------------------------------------------------------------------------------------------------------------------
// DIRECTIVAS DEL COPILADOR
//-----------------------------------------------------------------------------------------------------------------------------------------------

//______________________________________________________________________________
// DECLARACION DE VARIABLES
//______________________________________________________________________________

uint16_t PT1;
uint16_t PT2;
uint16_t VOLT;

uint8_t  UNID;
uint8_t  DECE;
uint8_t  CENT;

uint8_t  NUM_1;
uint8_t  NUM_2;
uint8_t  NUM_3;
uint8_t  NUM_4;
uint8_t  NUM_5;
uint8_t  NUM_6;
uint8_t  S3 = 0;
uint8_t  D3 = 0x30;
//______________________________________________________________________________
// PROTOTIPOS DE FUNCIONES
//______________________________________________________________________________

void SETUP(void);                            // Funcion de configuraciones. 
void DECIMAL(uint8_t V, uint8_t SELEC);      // Funcion de conversion a Decimal.

//______________________________________________________________________________
// FUNCION DE INTERRUPCIONES
//______________________________________________________________________________

void __interrupt() isr(void){
      
//----------------------------- INTERRUPCION ADC -------------------------------     
    
  if(ADIF == 1){               // Revisamos si la bandera del ADC esta levantada   
      if(ADCON0bits.CHS == 0){ // Revisamos si esta en el canal 0. 
         PT1 = adc_read();}    // Asignamos el valor convertido a la variable PT2.
      else{                   
         PT2 = adc_read();}    // Asignamos el valor convertido a la variable PT2.  
        DECIMAL(PT1,0);
        DECIMAL(PT2,1);
      }  
   

//------------------- INTERRUPCION DE RECEPCION DEL UART -----------------------  
        
    if(PIR1bits.RCIF){         
        
        char DATO = RCREG;        // ASIGNAMOS EL VALOR A LA VARIABLE "DATO". 
        
        if(DATO == 43){           // VERIFICAMOS SI EL CARACTER RECIBIDO ES "+".
            S3++;                 // AUMENTAMOS EL CONTADOR.
            D3 = S3 + 0x30;
        }
        if(DATO == 45){           // VERIFICAMOS SI EL CARACTER RECIBIDO ES "-".
            S3--;                 // DISMINUYE EL CONTADOR.
            D3 = S3 + 0x30;
        }
    }
}
//______________________________________________________________________________
// FUNCION PRINCIPAL (MAIN & LOOP)
//______________________________________________________________________________

void main(void){
    
    SETUP();                          // Llamamos a la funcion de configuracion.    
     
    while(1){
        
        if(ADCON0bits.GO == 0){       // Revisamos si ya termino la conversion.
            if(ADCON0bits.CHS == 0){  // Revisamos si esta en el canal 0.
                ADCON0bits.CHS = 1;}  // Cambiamos al canal 1. 
            else{ 
                ADCON0bits.CHS = 0;}  // Cambiamos al canal 0. 
            __delay_us(50);
            ADCON0bits .GO = 1;       // Encendemos la conversion nuevamente.     
        }
        
//------------------------ ESCRITURA EN PANTALLA LCD ---------------------------         
        
        Lcd_Set_Cursor(1,1);    
        Lcd_Write_String("S1:");      // Colocamos el nombre del sensor "S1".   
        Lcd_Set_Cursor(2,1);
        Lcd_Write_Char(NUM_1);        // Numero entero POT1. 
        Lcd_Write_Char(46);           // "."
        Lcd_Write_Char(NUM_2);        // Primer Decimal POT1. 
        Lcd_Write_Char(NUM_3);        // Segundo Decimal POT1.
        
        Lcd_Set_Cursor(1,7);
        Lcd_Write_String("S2:");      // Colocamos el nombre del sensor "S2".
        Lcd_Set_Cursor(2,7);          
        Lcd_Write_Char(NUM_4);        // Numero entero POT2.
        Lcd_Write_Char(46);           // "."
        Lcd_Write_Char(NUM_5);        // Primer Decimal POT2. 
        Lcd_Write_Char(NUM_6);        // Segundo Decimal POT2.
        
        Lcd_Set_Cursor(1,13);
        Lcd_Write_String("S3:");      // Colocamos el nombre del sensor "S2".
        Lcd_Set_Cursor(2,13);          
        Lcd_Write_Char(D3);           // Mostramos el valor del contador.    

//------------------- DESPLIEGUE DE VALORES EN LA CONSOLA ----------------------    
    
        __delay_ms(1000); 
        printf("POT1 = %i.%i%i V \r\r\r\r POT2 = %i.%i%i V \r\n ", NUM_1 - 0x30, NUM_2 - 0x30, NUM_3 - 0x30, NUM_4 - 0x30, NUM_5 - 0x30, NUM_6 - 0x30);

    }
    return;
}        
                
//______________________________________________________________________________
// FUNCION DE SEPARACION DE DIGITOS Y CONVERSION ASCII.
//______________________________________________________________________________

void DECIMAL(uint8_t V, uint8_t SELEC ){

//-------------------------- SEPARCION DE DIGITOS ------------------------------
     
    VOLT = (uint16_t)(V*1.961);           // Valor ADC entre el rango de 0V a 5V  
    CENT = VOLT/100;                      // Separamos el primer digito.
    DECE = (VOLT - CENT*100)/10;          // Separamos el segundo digito.
    UNID = (VOLT - CENT*100 - DECE*10)/1; // Separamos el tercer digito.

//--------------- CONVERSION ASCII & SELECCION DE POTENCIOMETRO ----------------
     
    if(SELEC == 0){
        NUM_1 = CENT + 0x30;
        NUM_2 = DECE + 0x30;
        NUM_3 = UNID + 0x30;  
    }
    
    if(SELEC == 1){
        NUM_4 = CENT + 0x30;
        NUM_5 = DECE + 0x30;
        NUM_6 = UNID + 0x30;  
    }
}        

//______________________________________________________________________________
// FUNCION DE CONFIGURACION
//______________________________________________________________________________

void SETUP(void){
    
//------------------- CONFIGURACION DE ENTRADAS Y SALIDAS ----------------------
    
    ANSEL  = 0b00000000;     // Seleccionamos las entradas AN1 & AN2.   
    ANSELH = 0b00000000;     
 
    TRISB = 0x00;            // Declaramos el PORTB como salidas.
    TRISC = 0b10000000;      // Declaramos el PORTC.bit7 como entrada.
    TRISA = 0x3;             // Declaramos el pin 0 y 1 del PORTA como entradas. 
    TRISD = 0x00;            // Declaramos el PORTD com salidas.
    TRISE = 0x00;            // Declaramos el PORTE como salidas.
    
    PORTB = 0x00;            // Limpiamos los puertos.
    PORTC = 0x00;
    PORTA = 0x00;
    PORTD = 0x00;  
    PORTE = 0x00;
    
//---------------------- CONFIGURACION DE RELOJ A 8MHZ -------------------------

    int_osc_MHz(3);          // 8 MHz 

//-------------------------- CONFIGURACION DE ADC ------------------------------
    
    adc_init(2,0,0);         // (Fosc/32), VDD, VSS. 
    adc_start(0);            // CHANEL 0, Encendemos ADC, Iniciamos conversion.

//-------------------------- CONFIGURACION DE LCD ------------------------------

    unsigned int a;          // Inicicalizacion de la pantalla LCD.
    Lcd_Init();

//--------------------------- CONFIGURACION DE TX & RX -------------------------
    
    UART_TX_CONFIG(9600);    // Configuracion y activacion del TX.
    UART_RX_CONFIG(9600);    // Configuracion y activacion del RX.

//---------------------- CONFIGURACION DE INTERRUPCIONES -----------------------

    INTCONbits.GIE  = 1;     // Habilitamos las interrupciones globales.
    INTCONbits.PEIE = 1;     // Habilitamos las interrupciones perifericas.
    PIE1bits.RCIE   = 1;
    PIR1bits.ADIF   = 0;     // Limpiamos la bandera del ADC.            
}
//______________________________________________________________________________
//______________________________________________________________________________
