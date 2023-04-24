/*
 John Minnick
 * 4/23/2023
 * ECE 167 Lab 2
 * Ping TOF Sensor stuff
 * repurposed some ece121 code from when i used this ping tof sensor 
 */

// includes 
#include <PING.h>
#include "stdlib.h"
#include "stdio.h"
#include <BOARD.h>
#include <AD.h>
//#include <serial.h>
#include <timers.h>

// notes
// D11 = pin 35 -> this will be my call pin
// D5 = pin 34 -> this is my CN pin, so there is were the response will be picked up
// 60ms -> 37500 tics -> 37500 base 10 -> 0x927C base 16
// 10us -> 16tics -> 0x000F
// PR4 us timer 4 period register 
//#define SYSTEM_CLOCK 80000000L -> need this for calcs
//#define  PB_CLOCK SYSTEM_CLOCK/2 -> and this 

//defines
#define CALL PORTDbits.RD11
#define CALL_CONFIG TRISDbits.TRISD11
#define RESPONSE PORTDbits.RD5
#define RESPONSE_CONFIG TRISDbits.TRISD5
#define MS_60 0x927C 
#define US_10 0x000F

//typedefs

typedef enum {
    TX = 0, // when setting trigger high for call be in TX
    RX = 1 // when waiting for response back be in RX
} PING_STATES;


//init vars 

PING_STATES currentState;

unsigned int callTime;
unsigned int responseTime;
unsigned int timeOfFlight;

//*           The timer peripheral is used to create the required trigger pulses. To do so you will need to modify when
// *          the next interrupt occurs by modifying the PR4 register to create the periodic pulse.
// *          The timer has already been set up with a 64:1 prescaler. The trigger pin has not been set and is up to you.
// *
// *          The change notify is configured to operate on CN14 or pin 34 on the I/O shield.  
// *          Instead of using a hardware timer we will use the TIMERS library to calculate the duration of the pulse
// *          in microseconds.

char PING_Init(void) {
    // following block inits the timer
    T4CON = 0;
    T4CONbits.TCKPS = 0b110;
    PR4 = 0xFFFF; // this is not the timer value wanted
    T4CONbits.ON = 1;
    IFS0bits.T4IF = 0;
    IPC4bits.T4IP = 3;
    IEC0bits.T4IE = 1;

    // following block inits change notify
    CNCONbits.ON = 1; // Change Notify On
    CNENbits.CNEN14 = 1;
    int temp = PORTD; // this is intentional to ensure a interrupt occur immediately upon enabling
    IFS1bits.CNIF = 0; // clear interrupt flag
    IPC6bits.CNIP = 1; //set priority
    IPC6bits.CNIS = 3; // and sub priority
    IEC1bits.CNIE = 1; // enable change notify
    //Anything else that needs to occur goes here
    // okay :) need timer stuff 
    TIMERS_Init(); // start timers, can be done in main but must occur b4 ping init
    CALL_CONFIG = 0; // setting call/pin35 to output
    CALL = 1; // setting ping high ie. CALL
    currentState = RX; // await response 
}

void __ISR(_CHANGE_NOTICE_VECTOR) ChangeNotice_Handler(void) {
    static char readPort = 0;
    readPort = PORTD; // this read is required to make the interrupt work
    IFS1bits.CNIF = 0;
    //Anything else that needs to occur goes here
    // okay :) catch and record time goes here
    if(RESPONSE){ // should trigger for any none zero/null val but can change to !=0 if weird 
        callTime = TIMERS_GetMicroSeconds(); // log start time 
    }else{
        responseTime = TIMERS_GetMicroSeconds(); // log end time 
    }
}

void __ISR(_TIMER_4_VECTOR) Timer4IntHandler(void) {
    IFS0bits.T4IF = 0;
    //Anything else that needs to occur goes here
    // ping TOF state machine here 
    switch (currentState) {
        case RX:
            CALL = 0; // set call low
            PR4 = MS_60; // modify PR4 register to generate a pulse -> pulse needs to be 60ms gonna look up conversion from ece121
            currentState = TX; // bit flip
            break;
        case TX:
            CALL = 1; // set call high
            PR4 = US_10; // for 10 microseconds
            currentState = RX; // bit flip 
            break;
        default: // edge case handling 
            printf("\n ping sensor error state \n");
    }
}

unsigned int PING_GetDistance(void) {
    //function model to convert TOF to distance goes here
}

unsigned int PING_GetTimeofFlight(void) {
    timeOfFlight = responseTime - callTime;
    return timeOfFlight;
}