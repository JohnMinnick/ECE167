/*
 *John Minnick
 * ECE167 Lab 2
 * 4/21/2023
 * Quadrature Encoder stuffs
 */

// including and colluding, remind you of anyone else we know?

#include "stdlib.h"
#include "stdio.h"


#include <BOARD.h>
#include <AD.h>
#include <serial.h>
//#include <timers.h>
#include <QEI.h>
//#include <proc/p32mx340f512h.h>


// defining and reclining, if  you know what i mean?
#define DETENT2DEGREE (15 / 4) // 24 detents in RE singular rotation, hence 15 degree per detent -> lab manual says resolution is 4 times this 

//          NOTE: Encoder A and B must be input to pins 36 and 37

#define ALPHA_PIN PORTDbits.RD6 // pin 36
#define BETA_PIN PORTDbits.RD7 // pin 37
// #define CHARLIE // unused C terminal 
// NOTE : ROTARY ENCODER HAS 24 DETENTS - MARKED AND COUNTED BY HAND
// this means 15 degrees per detent
//NOTE: Encoder A and B must be input to pins 36 and 37


// A is MSB 
// B is LSB 

typedef enum { //typedef state machine states 
    A_LO_B_LO = 0,
    A_LO_B_HI = 1,
    A_HI_B_LO = 2,
    A_HI_B_HI = 3,
    ERRORSTATE = 4 // hopefully this is not needed 
} QEI_STATE;

typedef enum {
    waiting, // dead zone wait, use during init 
    CW, // clockwise
    ACW // anti-clockwise ... easier to differentiate than CCW 
} directionOfRotation;

// count needs to be volatile - stated during class - think has something to do with interupts but dont wanna state assumptions as truth
volatile int QEI_count = 0;

QEI_STATE QEI_state = A_LO_B_LO; // A_LO_B_LO   == 00

int degrees = 0; // may go unused, may just track count and convert to degrees when called
int ALPHA;
int BETA;
directionOfRotation direction;

char QEI_Init(void) {
    // INIT Change notify
    CNCONbits.ON = 1; // Change Notify On
    CNENbits.CNEN15 = 1; //enable one phase
    CNENbits.CNEN16 = 1; //enable other phase
    int temp = PORTD; // this is intentional to ensure a interrupt occur immediately upon enabling
    IFS1bits.CNIF = 0; // clear interrupt flag
    IPC6bits.CNIP = 1; //set priority
    IPC6bits.CNIS = 3; // and sub priority
    IEC1bits.CNIE = 1; // enable change notify

    QEI_count = 0; // init count to zero incase value sustains thru hardware reset || redundancy 
    degrees = 0; // same as above 
    direction = waiting; // init direction to waiting 
    // the rest of the function goes here
    // okay :) 

    //init state machine, read ALPHA and BETA and set intial state 
    ALPHA = ALPHA_PIN; //init reads for init state
    BETA = BETA_PIN; // same as above 

    if (ALPHA == 0 && BETA == 0) {
        QEI_state = A_LO_B_LO; // 00
    } else if (ALPHA == 0 && BETA == 1) {
        QEI_state = A_LO_B_HI; // 01
    } else if (ALPHA == 1 && BETA == 0) {
        QEI_state = A_HI_B_LO; //10
    } else if (ALPHA == 1 && BETA == 1) {
        QEI_state = A_HI_B_HI; //11
    }

     return (SUCCESS); // char init wants a return but doesnt need one 
}

void __ISR(_CHANGE_NOTICE_VECTOR) ChangeNotice_Handler(void) {
    static char readPort = 0;
    readPort = PORTD; // this read is required to make the interrupt work
    IFS1bits.CNIF = 0;
    //anything else that needs to happen goes here
    // okay :)
    // read pins and update internal variables to match pins
    ALPHA = ALPHA_PIN; // port read to internal var
    BETA = BETA_PIN; // same as above 

    //state machine running
    switch (QEI_state) { // can move outside ISR if too slow but 6 of one || half dozen of the other is what i think 
        case A_LO_B_LO:
            if (ALPHA == 1 && BETA == 0) { // 00 -> 10 == moving right to left == ACW == minus count == minus degrees 
                QEI_count--;
                degrees = degrees - DETENT2DEGREE;
                direction = ACW;
                QEI_state = A_HI_B_LO;
            } else if (ALPHA == 0 && BETA == 1) { // 00 -> 01 == moving left to right == CW == plus count == plus degrees 
                QEI_count++;
                degrees = degrees + DETENT2DEGREE;
                direction = CW;
                QEI_state = A_LO_B_HI;
            } else if (ALPHA == 1 && BETA == 1) { // error case 
                QEI_state = ERRORSTATE;
            }
            break;
        case A_LO_B_HI:
            if (ALPHA == 1 && BETA == 1) {
                QEI_count--;
                degrees = degrees - DETENT2DEGREE;
                direction = ACW;
                QEI_state = A_LO_B_LO;
            } else if (ALPHA == 0 && BETA == 0) {
                QEI_count++;
                degrees = degrees + DETENT2DEGREE;
                direction = CW;
                QEI_state = A_HI_B_HI;
            } else if (ALPHA == 1 && BETA == 0) {
                QEI_state = ERRORSTATE;
            }
            break;
        case A_HI_B_HI:
            if (ALPHA == 0 && BETA == 1) {
                QEI_count--;
                degrees = degrees - DETENT2DEGREE;
                direction = ACW;
                QEI_state = A_LO_B_HI;
            } else if (ALPHA == 1 && BETA == 0) {
                QEI_count++;
                degrees = degrees + DETENT2DEGREE;
                direction = CW;
                QEI_state = A_HI_B_LO;
            } else if (ALPHA == 0 && BETA == 0) {
                QEI_state = ERRORSTATE;
            }
            break;
        case A_HI_B_LO:
            if (ALPHA == 0 && BETA == 0) {
                QEI_count--;
                degrees = degrees - DETENT2DEGREE;
                direction = ACW;
                QEI_state = A_HI_B_HI;
            } else if (ALPHA == 1 && BETA == 1) {
                QEI_count++;
                degrees = degrees + DETENT2DEGREE;
                direction = CW;
                QEI_state = A_LO_B_LO;
            } else if (ALPHA == 0 && BETA == 1) {
                QEI_state = ERRORSTATE;
            }
            break;
        case ERRORSTATE: // gonna handle error state like init and just re-init
            // IF PROBLEMS -> CHECK HERE FIRST 
            printf("ENTERED ERRORSTATE - QEI RE-INIT-ING - RESETING PARAMS");
           // QEI_count = 0; // init count to zero incase value sustains thru hardware reset || redundancy 
          //  degrees = 0; // same as above 
          //  direction = waiting; // init direction to waiting 
            // the rest of the function goes here
            // okay :) 

            //init state machine, read ALPHA and BETA and set intial state 
            ALPHA = ALPHA_PIN; //init reads for init state
            BETA = BETA_PIN; // same as above 

            if (ALPHA == 0 && BETA == 0) {
                QEI_state = A_LO_B_LO; // 00
            } else if (ALPHA == 0 && BETA == 1) {
                QEI_state = A_LO_B_HI; // 01
            } else if (ALPHA == 1 && BETA == 0) {
                QEI_state = A_HI_B_LO; //10
            } else if (ALPHA == 1 && BETA == 1) {
                QEI_state = A_HI_B_HI; //11
            }
            break;

        default:
            printf("How did you end up here? \n A Null Error perhaps? \n Or was it something more sinister? \n");
    }


}

void QEI_ResetPosition() { // reset func
    QEI_state = A_LO_B_LO; // state to 00
    QEI_count = 0; // count tp 0
    degrees = 0; // reset degrees 
    direction = waiting; // reset direction 
    IFS1bits.CNIF = 0; // clear interrupt flag MAYBE NOT NEEDED
}

int QEI_GetPosition(void) { // degree getter
    return degrees;
}