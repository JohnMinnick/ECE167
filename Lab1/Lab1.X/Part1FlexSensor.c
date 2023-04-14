/* 
 * File:   Part1FlexSensor.c
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

/*
 * 
 */
#define PIEZOLIMIT 100
#define NOISELIMIT 20


double tempDouble1, tempDouble2;
int tempInt1, tempInt2;

double flexInput;
double flexRegressed;
int flexPrev;



int main(void) {
    // inits and other stuff that should only occur once per run
    BOARD_Init();
    //SERIAL_Init();
    AD_Init();
    ToneGeneration_Init();
    AD_AddPins(AD_A1);
    
    flexPrev = 0; // settin init val 
    
    while(1){ // primary while(1) loop that running code should exist inside
        flexInput = AD_ReadADPin(AD_A1); // read in flex 
        
        if(abs(flexInput - flexPrev) < NOISELIMIT){ // hystersis filter from lab 0
            flexInput = flexPrev;
        }else{
            flexPrev = flexInput;
        }
        tempDouble1 = flexInput / 1023; // normalizing flex ADC reading 
        flexInput = flexInput * 3.3; // scaling to 3.3V 
    }
    
    BOARD_End(); // board end for redundancy 
}

