/* 
 * File:   helloworld.c
 * Author: 97min
 *
 * Created on April 6, 2023, 3:26 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"
#include "SERIAL.h"
#include "AD.h"
#include "ToneGeneration.h"


/*
 * 
 */

int pot;

int main(void) {
    BOARD_Init();
    ToneGeneration_Init();
    ToneGeneration_SetFrequency(TONE_196);
    ToneGeneration_ToneOn(); 
    // tone pwm pin is J2-03
    
    


    BOARD_End();
    while (1);
}

