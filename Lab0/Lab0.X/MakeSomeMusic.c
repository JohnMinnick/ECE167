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

int pot;
int prev = 0;
int limitCheck;
int audioSignal;

int main(void) {
    BOARD_Init();
    AD_Init();
    AD_AddPins(AD_A0);
    ButtonsInit();
    
    ToneGeneration_Init();
    ToneGeneration_SetFrequency(TONE_196);
    ToneGeneration_ToneOn();
    // tone pwm pin is J2-03

    while (1) {
        pot = AD_ReadADPin(AD_A0);
        
        //gonna ignore minor pot changes and only  update if pot change is greater than 50
        limitCheck = abs(prev - pot);
        
        if(limitCheck > 50 ){
            audioSignal = pot;
        }else{
            audioSignal=prev;
             prev = pot;
        
       //  audioSignal = audioSignal * audioSignal; // bad
        audioSignal = (audioSignal * audioSignal) / 2 ;  // better 
        
        if(BTN1){
            audioSignal = audioSignal + TONE_196;
        }
          if(BTN2){
            audioSignal = audioSignal + TONE_293;
        }
          if(BTN3){
            audioSignal = audioSignal + TONE_440;
        }
          if(BTN4){
            audioSignal = audioSignal + TONE_659;
        }
        
        
        ToneGeneration_SetFrequency(audioSignal);
        }
      
        // low pas filter specs - 0.1uF and 330ohm
    }

    

    BOARD_End();
    while (1);
}

