/* 
 * File:   CheckOff.c
 * Author: 97min
 *
 * Created on April 13, 2023, 5:37 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <BOARD.h>
#include <AD.h>
#include <ToneGeneration.h>
#include <math.h>
#include <serial.h>
#include <xc-pic32m.h>

/*
 * 
 */
#define PIEZOLIMIT 100
#define NOISELIMIT 15


//double tempDouble1, tempDouble2;
int tempInt1, tempInt2;

int flexInput;
int flexRegressed;
int flexPrev;

int main(void) {
    // inits and other stuff that should only occur once per run
    BOARD_Init();
    //SERIAL_Init();
    AD_Init();
    //SERIAL_Init();
    ToneGeneration_Init();
    AD_AddPins(AD_A1);
    ToneGeneration_ToneOn();
    flexPrev = 0; // settin init val 

    while (1) { // primary while(1) loop that running code should exist inside
        for (int i = 0; i < 100; i++) { // delay  
            i++;
        }
        flexInput = AD_ReadADPin(AD_A1); // read in flex 

        if (abs(flexInput - flexPrev) < NOISELIMIT) { // hystersis filter from lab 0
            flexInput = flexPrev;
        } else {
            flexPrev = flexInput;
        }
        // this DSP is dependent on the resistor value used within the voltage divider for the flex 
        // flexInput = flexInput / 1023; // normalizing flex ADC reading 
        // flexInput = flexInput * 3.3; // map normalized adc reading to 3.3V 
        // flexInput =  -40.0 * (flexInput * flexInput ) + (55.8 * flexInput) + 70.1; // map 3.3v to degrees
        //  flexInput = (1000/90) * flexInput; // map degrees to frequency 
        flexInput = (-flexInput + 660) * 4;
        if (flexInput < 1) {
            flexInput = 0;
        }
        if(flexInput > 1022){
            flexInput = 1022;
        }

        //tempInt1 = flexInput;
        //printf("%d \n",flexInput);
        // printf("hello world \n");
        //printf("%f \n",flexInput);
        printf("%d \n", flexInput);
        ToneGeneration_SetFrequency(flexInput);

    }

    BOARD_End(); // board end for redundancy 
}

