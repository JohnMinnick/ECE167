/*
 *John Minnick
 * ECE167 Lab 2
 * 4/21/2023
 * Quadrature Encoder stuffs
 */
// pin 3 red
// pin 6 blue
// pin 5 green 
// including and colluding, remind you of anyone else we know?

#include "stdlib.h"
#include "stdio.h"


#include <BOARD.h>
#include <AD.h>
#include <serial.h>
//#include <timers.h>
#include <stdio.h>
#include <QEI.h>
#include <pwm.h>
//#include <proc/p32mx340f512h.h>
int RED_dutyCycle;
int GREEN_dutyCycle;
int BLUE_dutyCycle;
int degree;
int pos = 0;

#define DC_CONVERSIONFACTOR 2.7

int main(void) {
    BOARD_Init();
    QEI_Init();
    PWM_Init();
    PWM_AddPins(PWM_PORTZ06); // red
    PWM_AddPins(PWM_PORTY10); // green
    PWM_AddPins(PWM_PORTY12); // blue

    printf("\n \n Starting QEI Main w/ Color \n \n");

    while (1) {
        for (int i = 0; i < 183000; i++) {

        }
        pos = QEI_GetPosition();

        if (pos <= 0) {
            pos = -1 * pos;
        }
        pos = pos % 360; // normalizing to 0 to 360 

        if (pos <= 120) {
            BLUE_dutyCycle = pos * DC_CONVERSIONFACTOR;
            RED_dutyCycle = 1000 - (pos * DC_CONVERSIONFACTOR);
            GREEN_dutyCycle = 0;
        } else if (pos <= 240) {
            RED_dutyCycle = pos * DC_CONVERSIONFACTOR;
            GREEN_dutyCycle = 1000 - (pos * DC_CONVERSIONFACTOR);
            BLUE_dutyCycle = 0;
        } else if (pos <= 360) {
            GREEN_dutyCycle = pos * DC_CONVERSIONFACTOR;
            BLUE_dutyCycle = 1000 - (pos * DC_CONVERSIONFACTOR);
            RED_dutyCycle = 0;
        } 


        PWM_SetDutyCycle(PWM_PORTZ06, 1000 - RED_dutyCycle);
        PWM_SetDutyCycle(PWM_PORTY10, 1000 - GREEN_dutyCycle);
        PWM_SetDutyCycle(PWM_PORTY12, 1000 - BLUE_dutyCycle);

        printf("\n %d\n", pos);
    }

    BOARD_End(); // safety catch 
}




