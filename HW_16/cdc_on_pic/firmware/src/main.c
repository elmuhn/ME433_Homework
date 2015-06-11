/*******************************************************************************
  MPLAB Harmony Project Main Source File
 
  Company:
    Microchip Technology Inc.
   
  File Name:
    main.c
 
  Summary:
    This file contains the "main" function for an MPLAB Harmony project.
 
  Description:
    This file contains the "main" function for an MPLAB Harmony project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state 
    machines of all MPLAB Harmony modules in the system and it calls the 
    "SYS_Tasks" function from within a system-wide "super" loop to maintain 
    their correct operation. These two functions are implemented in 
    configuration-specific files (usually "system_init.c" and "system_tasks.c")
    in a configuration-specific folder under the "src/system_config" folder 
    within this project's top-level folder.  An MPLAB Harmony project may have
    more than one configuration, each contained within it's own folder under
    the "system_config" folder.
 *******************************************************************************/
 
// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.
 
//Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).
 
You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.
 
SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END
 
 
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
 
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>
#include <proc/p32mx250f128b.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"   // SYS function prototypes
 
 
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
 
int main ( void )
{
    /* Initialize all MPLAB Harmony modules, including application(s). */
    ANSELBbits.ANSB15 = 0; //Make it digital
    TRISBbits.TRISB15 = 0;
    TRISBbits.TRISB7 = 0;
 
    __builtin_enable_interrupts();
     
    // set up USER pin as input
    ANSELBbits.ANSB13 = 0;
    TRISBbits.TRISB13 = 1;
 
    //Set phase pins
    //   PH1--> B7
//    RPB7Rbits.RPB7R = 0b001; //Peripheral requires output pin
    TRISBbits.TRISB7 = 0; //Make it an output
    LATBbits.LATB7 = 1; //Set it high- this initializes as the forward direction
 
    //   PH2-->B14
    ANSELBbits.ANSB14 = 0; //Make it digital
    RPB14Rbits.RPB14R = 0b001;
    TRISBbits.TRISB14 = 0; //Make it an output
    LATBbits.LATB14 = 0; //Set it high- this initializes as the forward direction    
 
    //Set enable pins
    //   EN1--> B5 
    //ANSELBbits.ANSB5 = 0; //Make it digital
    RPB5Rbits.RPB5R = 0b0101; //Set to OC2
    TRISBbits.TRISB5 = 0; //Make it an output
 
    //   EN2-->B15    
    ANSELBbits.ANSB15 = 0; //Make it digital
    RPB15Rbits.RPB15R = 0b0101; //Set to OC1
//    TRISBbits.TRISB15 = 0; //Make it an output
 
    //Set timers:
    //This is to set up PWM frequency for OC pin- this will be altered using potentiometer
    T2CONbits.TCKPS = 0;     // Timer2 prescaler N=1 (1:4)
    PR2 = 1999;                 // period = (PR2+1) * N * 12.5 ns = 1 ms, 1 kHz 
    TMR2 = 0;
     
    OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
    OC1CONbits.OCTSEL = 0; //Using Timer 2 for OC1
    OC1RS = 0;               //duty cycle = OC1RS/(PR2+1) 25% duty cycle to begin with
    OC1R = 0;                //initialize before turning OC1 on;        
    T2CONbits.ON = 1;        // turn on Timer2
    OC1CONbits.ON = 1;       // turn on OC1
 
    //This is to set up PWM frequency for OC pin- this will be altered using potentiometer
 
    OC2CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
    OC2CONbits.OCTSEL = 0;     //Using Timer2 for OC2
    OC2RS = 0;               //duty cycle = OC1RS/(PR2+1)
    OC2R = 0;                //initialize before turning OC1 on;        
    OC2CONbits.ON = 1;       // turn on OC2
     
    __builtin_enable_interrupts();
     
    SYS_Initialize ( NULL );
     
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
         
        SYS_Tasks ( );
 
    }
 
    /* Execution should not come here during normal operation */
 
    return ( EXIT_FAILURE );
}
 
 
/*******************************************************************************
 End of File
*/