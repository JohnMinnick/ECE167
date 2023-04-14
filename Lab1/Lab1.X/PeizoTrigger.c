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
#define TRIGGERTIME 200

//double tempDouble1, tempDouble2;
int tempInt1, tempInt2;

int piezoInput;
int timer;

int main(void) {
    // inits and other stuff that should only occur once per run
    BOARD_Init();
    //SERIAL_Init();
    AD_Init();
    //SERIAL_Init();
    ToneGeneration_Init();
    ToneGeneration_SetFrequency(TONE_196);
    AD_AddPins(AD_A2);
     timer = 0; // settin init val 
    
    while(1){ // primary while(1) loop that running code should exist inside
        for(int i = 0; i < 100; i++) { // delay  
           i++ ; 
        }
        piezoInput = AD_ReadADPin(AD_A2); // read in flex 
        
        // this come from peizo sensor guide but NOT needed for this application, it does work tho
        // peizoInput = peizoInput / 1023; // normalizing flex ADC reading 
        // peizoInput = peizoInput * 3.3; // map normalized adc reading to 3.3V 
        if(PIEZOLIMIT < piezoInput){ // piezo limit check
            ToneGeneration_ToneOn(); // turn on tone
            timer = 0; // reset timer while over trigger limit
        }else if(TRIGGERTIME < timer){ // continue playing for TRIGGERTIME while above limit
            ToneGeneration_ToneOff(); // turn off tone
        }
        timer++; // timer increment
        
        printf("%d \n",piezoInput); // print out for serial debugging 
       
       
    }
    
    BOARD_End(); // board end for redundancy 
}

