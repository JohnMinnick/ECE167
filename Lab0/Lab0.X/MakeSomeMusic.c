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
#include "math.h"
#include "Buttons.h"


/*
 * 
 */

int pot =0;
int prev = 0;
int limitCheck = 0;
int audioSignal = 0;
int audioDiff = 0;


int main(void) {
    BOARD_Init();
    AD_Init();
    AD_AddPins(AD_A0);
    ButtonsInit();
    ToneGeneration_Init();
   // ToneGeneration_ToneOn(); // mabe in loop 
    //ToneGeneration_SetFrequency(TONE_196);
    //ToneGeneration_ToneOn();
    // tone pwm pin is J2-03

    while (1) {
        
        pot = AD_ReadADPin(AD_A0);
        
        if(abs(prev-pot) < 30){ // hystersis , 50 might be too big, maybe 10? 
            pot = prev;
        }else{
            prev=pot;
        }
        
       pot = pot /2; // scale to 5V to match supply
        
        if (BTN1) {
            audioSignal = TONE_196 + pot;
        }
        if (BTN2) {
            audioSignal = TONE_293 + pot;
        }
        if (BTN3) {
            audioSignal = TONE_440 + pot;
        }
        if (BTN4) {
            audioSignal = TONE_659 + pot;
        }
        if(audioSignal == 0){ // may need more explicity conditional 
            ToneGeneration_ToneOff();
        }
        if(audioSignal > 1000){
            audioSignal = 1000;
        }
        if(audioSignal != 0){
        ToneGeneration_ToneOn();
        audioSignal = audioSignal;
        ToneGeneration_SetFrequency(audioSignal);
        }
        audioSignal = 0; // reset audio signal
        
    }
    // low pas filter specs - 0.1uF and 330ohm




    BOARD_End();
    while (1);
}

